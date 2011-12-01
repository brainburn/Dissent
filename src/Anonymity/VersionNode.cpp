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
      _data = new VersionNodeData(QByteArray(0),
                                  QVector<QByteArray>(0),
                                  QByteArray(0));
  }

  VersionNode::VersionNode(const QByteArray _b_group_data)
  {
      QByteArray _byte_buffer;
      QDataStream stream(&_byte_buffer, QIODevice::WriteOnly);
      Crypto::CppHash h;

      stream << _b_group_data;
      stream << QByteArray(0);

      _data = new VersionNodeData(h.ComputeHash(_byte_buffer),
                                  QVector<QByteArray>(0),
                                  _b_group_data);
  }

VersionNode::VersionNode(const QByteArray _b_group_data,
                         const QVector<QByteArray> _pkeys)
{
    QByteArray _byte_buffer;
    QDataStream stream(&_byte_buffer, QIODevice::WriteOnly);
    Crypto::CppHash h;

    stream << _b_group_data;
    stream << _pkeys;

    _data = new VersionNodeData(h.ComputeHash(_byte_buffer),
                                _pkeys,
                                _b_group_data);
}

VersionNode::VersionNode(const QByteArray _version_hash,
            const QVector<QByteArray> _pkeys,
            const QByteArray _b_group_data)
{
    _data = new VersionNodeData(_version_hash,
                                _pkeys,
                                _b_group_data);
}

const Group VersionNode::getGroup(Group &group) const
{
    QByteArray temp_b_group_data(_data->BGroupData);
    QDataStream stream(&temp_b_group_data, QIODevice::ReadOnly);
    stream >> group;
    return group;
}

const QVector<QByteArray> &VersionNode::getParents() const
{
    return _data->PKeys;
}

const QByteArray &VersionNode::getHash() const
{
    return _data->VersionHash;
}

const QByteArray &VersionNode::getGroupByteArray() const
{
    return _data->BGroupData;
}


QDataStream &operator << (QDataStream &out, const VersionNode node)
{

    out << node.getHash() << node.getParents() << node.getGroupByteArray();
    return out;
}

QDataStream &operator >> (QDataStream &in, VersionNode &node)
{
    QByteArray _version_hash;
    QVector<QByteArray> _pkeys;
    QByteArray _b_group_data;

    in >> _version_hash;
    in >> _pkeys;
    in >> _b_group_data;

    node = VersionNode(_version_hash, _pkeys, _b_group_data);


    return in;
}

}
}
