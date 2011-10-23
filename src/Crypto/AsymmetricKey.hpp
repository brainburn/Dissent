#ifndef DISSENT_CRYPTO_ASYMMETRIC_KEY_H_GUARD
#define DISSENT_CRYPTO_ASYMMETRIC_KEY_H_GUARD

#include <QByteArray>
#include <QString>
#include <iostream>
namespace Dissent {
namespace Crypto {
  /**
   * Stores an asymmetric (public or private key), derived classes should
   * implement these functions.  For public keys, private operations should
   * return false / null / empty values
   */
  class AsymmetricKey {
    public:
      /**
       * Default recommended key size
       */
      static const int KeySize = 2048;

      /**
       * Retrieves the PublicKey, if this is already a public key, return a new
       * copy of this object, otherwise return a new copy of the public material
       * of the private key
       */
      virtual AsymmetricKey *GetPublicKey() = 0;
      
      /**
       * Saves the key to a file
       * @param filename the file to save the key into, will overwrite the file
       */
      virtual bool Save(const QString &filename) = 0;

      /**
       * Returns the key in a byte array format
       */
      virtual QByteArray GetByteArray() = 0;

      /**
       * Signs the data, returning the signature
       * @param data the data to sign
       */
      virtual QByteArray Sign(const QByteArray &data) = 0;

      /**
       * Verify a signature, returns true if signature matches the data
       * @param data the data to verify
       * @param sig the signature used to verify the data
       */
      virtual bool Verify(const QByteArray &data, const QByteArray &sig) = 0;

      /**
       * Returns an encrypted data block of the form: Pr[AES Key], IV, AES[data]
       * @param data data to encrypt
       */
      virtual QByteArray Encrypt(const QByteArray &data) = 0;

      /**
       * Returns the decrypts data 
       * @param data encrypted data in the form: Pr[AES Key], IV, AES[data]
       */
      virtual QByteArray Decrypt(const QByteArray &data) = 0;

      /**
       * Returns true if private key or private / public key pair
       */
      virtual bool IsPrivateKey() = 0;

      /**
       * Verify the two keys are related private / public key pairs
       */
      virtual bool VerifyKey(AsymmetricKey &key) = 0;

      /**
       * Returns true if the key loaded is a valid key
       */
      virtual bool IsValid() = 0;
  };
}
}

#endif
