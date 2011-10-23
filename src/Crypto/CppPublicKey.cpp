#include "CppPublicKey.hpp"

namespace Dissent {
namespace Crypto {
  CppPublicKey::CppPublicKey(const QString &filename) :
    _public_key(new RSA::PublicKey())
  {
    _valid = InitFromFile(filename);
  }

  CppPublicKey::CppPublicKey(const QByteArray &data) :
    _public_key(new RSA::PublicKey())
  {
    _valid = InitFromByteArray(data);
  }

  CppPublicKey::CppPublicKey(RSA::PublicKey *key) :
    _public_key(new RSA::PublicKey(*key))
  {
    _valid = true;
  }

  CppPublicKey::~CppPublicKey()
  {
    if(_public_key) {
      delete _public_key;
    }
  }

  AsymmetricKey *CppPublicKey::GetPublicKey()
  {
    if(!_valid) {
      return 0;
    }
    return new CppPublicKey(_public_key);
  }

  bool CppPublicKey::InitFromFile(const QString &filename)
  {
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
      qWarning() << "Error (" << file.error() << ") reading file: " << filename;
      return false;
    }

    return InitFromByteArray(file.readAll());
  }

  bool CppPublicKey::InitFromByteArray(const QByteArray &data)
  {
    ByteQueue queue;
    queue.Put2(reinterpret_cast<const byte *>(data.data()), data.size(), 0, true);
    try {
      _public_key->Load(queue);
    } catch (std::exception &e) {
      std::cout << "In CppPublicKey::Decrypt: " << e.what();
      return false;
    }
    return true;
  }

  bool CppPublicKey::Save(const QString &filename)
  {
    if(!_valid) {
      return false;
    }

    QByteArray data = GetByteArray();
    QFile file(filename);
    if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
      qWarning() << "Error (" << file.error() << ") saving file: " << filename;
      return false;
    }

    file.write(data);
    file.close();
    return true;
  }

  QByteArray CppPublicKey::GetByteArray()
  {
    if(!_valid) {
      return QByteArray();
    }

    ByteQueue queue;
    _public_key->Save(queue);
    QByteArray data(queue.CurrentSize(), 0);
    queue.Get(reinterpret_cast<byte *>(data.data()), data.size());
    return data;
  }

  QByteArray CppPublicKey::Sign(const QByteArray &)
  {
    qWarning() << "In CppPublicKey::Sign: Attempting to sign with a public key";
    return QByteArray();
  }

  bool CppPublicKey::Verify(const QByteArray &data, const QByteArray &sig)
  {
    if(!_valid) {
      return false;
    }

    RSASS<PKCS1v15, SHA>::Verifier verifier(*_public_key);
    return verifier.VerifyMessage(reinterpret_cast<const byte *>(data.data()),
        data.size(), reinterpret_cast<const byte *>(sig.data()), sig.size());
  }

  QByteArray CppPublicKey::Encrypt(const QByteArray &data)
  {
    if(!_valid) {
      return QByteArray();
    }

    RSAES<OAEP<SHA> >::Encryptor encryptor(*_public_key);
    int clength = ((data.size() / AES::BLOCKSIZE) + 1) * AES::BLOCKSIZE;
    int data_start = encryptor.FixedCiphertextLength() + AES::BLOCKSIZE;
    QByteArray ciphertext(data_start + clength, 0);

    CryptoPP::AutoSeededX917RNG<CryptoPP::DES_EDE3> rng;

    SecByteBlock skey(AES::DEFAULT_KEYLENGTH);
    rng.GenerateBlock(skey, skey.size());

    byte *iv = reinterpret_cast<byte *>(ciphertext.data() + encryptor.FixedCiphertextLength());
    rng.GenerateBlock(iv, AES::BLOCKSIZE);

    CBC_Mode<AES>::Encryption enc;
    enc.SetKeyWithIV(skey, skey.size(), iv);

    StringSource(reinterpret_cast<const byte *>(data.data()), data.size(), true,
        new StreamTransformationFilter(enc,
          new ArraySink(reinterpret_cast<byte *>(ciphertext.data() + data_start), clength)));

    encryptor.Encrypt(rng, reinterpret_cast<const byte *>(skey.data()),
        skey.size(), reinterpret_cast<byte *>(ciphertext.data()));

    return ciphertext;
  }

  QByteArray CppPublicKey::Decrypt(const QByteArray &)
  {
    qWarning() << "In CppPublicKey::Decrypt: Attempting to decrypt with a public key";
    return QByteArray();
  }

  bool CppPublicKey::VerifyKey(AsymmetricKey &key)
  {
    if(!IsValid() || !key.IsValid() || (IsPrivateKey() == key.IsPrivateKey())) {
      return false;
    }

    CppPublicKey *other = dynamic_cast<CppPublicKey *>(&key);
    if(!other) {
      return false;
    }

    return (other->_public_key->GetModulus() == this->_public_key->GetModulus()) &&
      (other->_public_key->GetPublicExponent() == this->_public_key->GetPublicExponent());
  }
}
}

