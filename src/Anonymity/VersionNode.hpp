#ifndef VERSIONNODE_HPP
#define VERSIONNODE_HPP

#include "Group.hpp"
#include <QHash>


namespace Dissent{
namespace Anonymity{

class VersionNode
{
public:
    VersionNode();

    VersionNode(QByteArray _b_group_data);

    VersionNode(const uint _group_hash, const QVector<uint> _pkeys,
                const QVector<uint> _ckeys, const QByteArray _b_group_data);

    uint group_hash;        // Hash value of this group
    QVector<uint> pkeys;    // Parent hash keys
    QVector<uint> ckeys;    // Children hash keys
    QByteArray b_group_data;// Binary representation

    /**
     *  Returns the group represented by the version's hash value
     */
    const Group getGroup() const;

    /**
     *  Returns a vector of versions that succeed the current one
     */
    const QVector<uint> &getChildren() const;

    /**
     *  Returns a vector of the parents of the current version
     */
    const QVector<uint> &getParents() const;

    /**
     *  Returns the hash value of the current version
     */
    const uint &getHash() const;

    /**
     *  Returns the binary representation of the group related to the current version
     */
    const QByteArray &getGroupByteArray() const;

    int addChild(VersionNode &child);
};

QDataStream &operator << (QDataStream &out, const VersionNode node);
QDataStream &operator >> (QDataStream &in, VersionNode &node);

}
}

#endif // VERSIONNODE_HPP
