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
                                  QByteArray(0),
                                  QMap<QString, QVariant>());
  }

  VersionNode::VersionNode(const QByteArray _b_group_data)
  {
      QByteArray _byte_buffer;
      QDataStream stream(&_byte_buffer, QIODevice::WriteOnly);
      Crypto::CppHash h;

      stream << _b_group_data;
      stream << QByteArray(0);
      //stream << QMap<QString, QVariant>();

      _data = new VersionNodeData(h.ComputeHash(_byte_buffer),
                                  QVector<QByteArray>(0),
                                  _b_group_data,
                                  QMap<QString, QVariant>());
  }

VersionNode::VersionNode(const QByteArray _b_group_data,
                         const QVector<QByteArray> _pkeys,
                         const QMap<QString, QVariant> _group_policy)
{
    QByteArray _byte_buffer;
    QDataStream stream(&_byte_buffer, QIODevice::WriteOnly);
    Crypto::CppHash h;

    stream << _b_group_data;
    stream << _pkeys;
    stream << _group_policy;

    _data = new VersionNodeData(h.ComputeHash(_byte_buffer),
                                _pkeys,
                                _b_group_data,
                                _group_policy);
}

VersionNode::VersionNode(const QByteArray _version_hash,
            const QVector<QByteArray> _pkeys,
            const QByteArray _b_group_data,
            const QMap<QString, QVariant> _group_policy)
{
    _data = new VersionNodeData(_version_hash,
                                _pkeys,
                                _b_group_data,
                                _group_policy);
}

const Group VersionNode::getGroup(Group &group) const
{
    QByteArray temp_b_group_data(_data->BGroupData);
    QDataStream stream(&temp_b_group_data, QIODevice::ReadOnly);
    stream >> group;
    return group;
}

const QMap<QString, QVariant> &VersionNode::getPolicy() const
{
    return _data->GroupPolicy;
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

    out << node.getHash() << node.getParents() << node.getGroupByteArray() << node.getPolicy();
    return out;
}

QDataStream &operator >> (QDataStream &in, VersionNode &node)
{
    QByteArray              _version_hash;
    QVector<QByteArray>     _pkeys;
    QByteArray              _b_group_data;
    QMap<QString, QVariant> _group_policy;

    in >> _version_hash;
    in >> _pkeys;
    in >> _b_group_data;
    in >> _group_policy;

    node = VersionNode(_version_hash, _pkeys, _b_group_data, _group_policy);


    return in;
}

}
}
