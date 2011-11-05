#include "VersionNode.hpp"


namespace Dissent {
namespace Anonymity {
  namespace {
    using Dissent::Connections::Id;
    using Dissent::Crypto::AsymmetricKey;
  }
  VersionNode::VersionNode():
      group_hash(0),
      pkeys(QVector<uint>(0)),
      ckeys(QVector<uint>(0)),
      b_group_data (QByteArray(0))
  {
  }

  VersionNode::VersionNode(const QByteArray _b_group_data):
      group_hash(qHash(_b_group_data)),
      pkeys(QVector<uint>(0)),
      ckeys(QVector<uint>(0)),
      b_group_data (_b_group_data)
  {
  }

VersionNode::VersionNode(const uint _group_hash,
                         const QVector<uint> _pkeys,
                         const QVector<uint> _ckeys,
                         const QByteArray _b_group_data):
    group_hash(_group_hash),
    pkeys(_pkeys),
    ckeys(_ckeys),
    b_group_data (_b_group_data)
{
}

//This doesn't look well...
const Group VersionNode::getGroup() const {
    QByteArray temp_b_group_data(this->b_group_data);
    QDataStream stream(&temp_b_group_data, QIODevice::ReadOnly);
    Group group(QVector<Id>(0));
    stream  >> group;
    return group;
}

const QVector<uint> &VersionNode::getChildren() const {
    return this->ckeys;
}

const QVector<uint> &VersionNode::getParents() const {
    return this->pkeys;
}

const uint &VersionNode::getHash() const {
    return this->group_hash;
}

const QByteArray &VersionNode::getGroupByteArray() const {
    return this->b_group_data;
}

int VersionNode::addChild(VersionNode &child){

}

QDataStream &operator << (QDataStream &out, const VersionNode node){

    out << node.getHash() << node.getParents() << node.getChildren() << node.getGroupByteArray();
    return out;
}

QDataStream &operator >> (QDataStream &in, VersionNode &node){
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
