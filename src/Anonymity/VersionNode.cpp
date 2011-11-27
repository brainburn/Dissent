#include "VersionNode.hpp"


namespace Dissent {
namespace Anonymity {
  namespace {
    using Dissent::Connections::Id;
    using Dissent::Crypto::AsymmetricKey;
  }
  const VersionNode VersionNode::Zero = VersionNode();

  VersionNode::VersionNode()
  {
      _data = new VersionNodeData(0,
                                  QVector<uint>(0),
                                  QVector<uint>(0),
                                  QByteArray(0));
  }

  VersionNode::VersionNode(const QByteArray _b_group_data)
  {
      _data = new VersionNodeData(qHash(_b_group_data),
                                  QVector<uint>(0),
                                  QVector<uint>(0),
                                  _b_group_data);
  }

VersionNode::VersionNode(const uint _group_hash,
                         const QVector<uint> _pkeys,
                         const QVector<uint> _ckeys,
                         const QByteArray _b_group_data)
{
    _data = new VersionNodeData(_group_hash,
                                _pkeys,
                                _ckeys,
                                _b_group_data);
}

const Group VersionNode::getGroup(Group &group) const
{
    QByteArray temp_b_group_data(_data->BGroupData);
    QDataStream stream(&temp_b_group_data, QIODevice::ReadOnly);
    stream >> group;
    return group;
}

const QVector<uint> &VersionNode::getChildren() const
{
    return _data->CKeys;
}

const QVector<uint> &VersionNode::getParents() const
{
    return _data->PKeys;
}

const uint &VersionNode::getHash() const
{
    return _data->GroupHash;
}

const QByteArray &VersionNode::getGroupByteArray() const
{
    return _data->BGroupData;
}

void VersionNode::addParents(const QVector<VersionNode *> &parents)
{
    for(int idx = 0; idx < parents.size(); idx++){
        _data->PKeys.append(parents[idx]->getHash());
    }
}

void VersionNode::addParents(QVector<uint> &parents)
{
    _data->PKeys += parents;
}


void VersionNode::addChildren(const QVector<VersionNode *> &children)
{
    for(int idx = 0; idx < children.size(); idx++){
        _data->CKeys.append(children[idx]->getHash());
    }
}

void VersionNode::addChildren(QVector<uint> const &children)
{
    _data->CKeys += children;
}

QDataStream &operator << (QDataStream &out, const VersionNode node)
{

    out << node.getHash() << node.getParents() << node.getChildren() << node.getGroupByteArray();
    return out;
}

QDataStream &operator >> (QDataStream &in, VersionNode &node)
{
    uint _group_hash;
    QVector<uint> _pkeys;
    QVector<uint> _ckeys;
    QByteArray _b_group_data;

    in >> _group_hash;
    in >> _pkeys;
    in >> _ckeys;
    in >> _b_group_data;

    node = VersionNode(_group_hash, _pkeys, _ckeys, _b_group_data);


    return in;
}

}
}
