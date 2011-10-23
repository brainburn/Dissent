#include "Group.hpp"

namespace Dissent {
namespace Anonymity {
  Group::Group(const QVector<Id> &group, const QVector<AsymmetricKey *> &keys)
  {
    QHash<const Id, int> id_to_int;
    for(int idx = 0; idx < group.count(); idx++) {
      id_to_int[group[idx]] = idx;
    }
    _data = new GroupData(group, id_to_int, keys);
  }

  const Id &Group::GetId(int idx) const
  {
    if(idx >= _data->Size || idx < 0) {
      return Id::Zero;
    }
    return _data->GroupVector[idx];
  }

  const Id &Group::Next(const Id &id) const
  {
    return GetId(GetIndex(id) + 1);
  }

  const Id &Group::Previous(const Id &id) const
  {
    return GetId(GetIndex(id) - 1);
  }

  bool Group::Contains(const Id &id) const
  {
    return _data->IdtoInt.contains(id);
  }

  int Group::GetIndex(const Id &id) const
  {
    if(_data->IdtoInt.contains(id)) {
      return _data->IdtoInt[id];
    }
    return -1;
  }

  AsymmetricKey *Group::GetKey(const Id &id) const
  {
    int idx = GetIndex(id);
    if(idx == -1) {
      return 0;
    }
    return GetKey(idx);
  }

  AsymmetricKey *Group::GetKey(int idx) const
  {
    if(idx >= _data->Keys.count() || idx < 0) {
      return 0;
    }
    return _data->Keys[idx];
  }

  QByteArray Group::keysToByteArray() const
  {
      QByteArray bkey_data;

      int v_size = this->_data->Keys.count();

      if(v_size > 0){
          for(int i = 0;  i< v_size; i++){
              bkey_data.append(this->_data->Keys[i]->GetByteArray());
          }
      }
      return bkey_data;

  }

  //Serialize enough info to recreate Group, not GroupData...
  QDataStream &operator<<(QDataStream &out, const Group group)
  {
      QByteArray bgroup_data = group.keysToByteArray();

      out << group.GetIds();
      out.writeRawData(bgroup_data.data(), bgroup_data.size());

      return out;
  }

  //Use group to initialize class...
  QDataStream &operator>>(QDataStream &in, Group &group)
  {
      QVector<Id> t_group_vector;
      QVector<AsymmetricKey *> t_keyp_vector;
      Crypto::CppPrivateKey *key_0 = new Crypto::CppPrivateKey();
      Crypto::AsymmetricKey *key_1 = key_0->GetPublicKey();
      int key_size = key_1->GetByteArray().count();
      int group_size;
      char *buf;


      in >> t_group_vector;

      group_size = t_group_vector.count();

      buf = new char[key_size*group_size];
      in.readRawData(buf, key_size*group_size);

      QByteArray bkey_container(buf,key_size*group_size);

      for(int i = 0; i < group_size; i++){
          Crypto::CppPublicKey *t_key = new Crypto::CppPublicKey(bkey_container);
          t_keyp_vector.append(t_key);
          bkey_container = bkey_container.remove(0, key_size);
      }

      delete [] buf;

      group = Group(t_group_vector, t_keyp_vector);

      return in;

  }
}
}
