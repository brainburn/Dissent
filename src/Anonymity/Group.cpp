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
}
}
