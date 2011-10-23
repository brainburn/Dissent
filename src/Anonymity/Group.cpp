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
      QByteArray t_data;
      QDataStream stream(&bkey_data, QIODevice::WriteOnly);
      QDataStream t_stream(&t_data, QIODevice::WriteOnly);

      int v_size = this->_data->Keys.count();

      if(v_size == 0){
         stream << 0;
       }else{
          for(int i = 0;  i< v_size; i++){
              t_stream << this->_data->Keys[i]->GetByteArray();
              stream << t_data.count() << t_data;
              t_data.clear();
          }
      }
      return bkey_data;
  }

  //Serialize enough info to recreate Group, not GroupData...
  QDataStream &operator<<(QDataStream &out, const Group group)
  {
      QByteArray bgroup_data;
      QDataStream stream(&bgroup_data, QIODevice::ReadWrite);

      out << group.GetIds();
      out << group.keysToByteArray();

      //out << bgroup_data;

      return out;
  }

  //Use group to initialize class...
  QDataStream &operator>>(QDataStream &in, Group &group)
  {
      QVector<Id> t_group_vector;
      QVector<AsymmetricKey *> t_keyp_vector;


      in >> t_group_vector;

      /*
      int g_size;
      uint k_size;
      char *t_bytes;
      g_size = t_group_vector.count();


      for(int i = 0; i < g_size; i++){
          in >> k_size;
          std::cout<<"ksize::" << k_size << std::endl;
          if(k_size <= 4){
              break;
          }
          t_bytes = new char[k_size];
          in.readBytes(t_bytes, k_size);
          Crypto::CppPublicKey *t_key = new Crypto::CppPublicKey(QByteArray(t_bytes));
          t_keyp_vector.append(t_key);
          delete [] t_bytes;
      }
      */

      group = Group(t_group_vector);

      return in;

  }
}
}
