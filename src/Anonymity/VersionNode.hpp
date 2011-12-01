#ifndef VERSIONNODE_HPP
#define VERSIONNODE_HPP

#include "Group.hpp"
#include "Crypto/CppHash.hpp"
#include <QHash>


namespace Dissent{
namespace Anonymity{

class VersionNodeData : public QSharedData{
public:
    VersionNodeData(const QByteArray version_hash,
                    const QVector<QByteArray> pkeys,
                    const QByteArray b_group_data):
        VersionHash(version_hash),
        PKeys(pkeys),
        BGroupData(b_group_data)
    {
    }
    const QByteArray                VersionHash; // Hash value of this group
    const QVector<QByteArray>       PKeys;       // Parent hash keys
    const QByteArray                BGroupData;  // Binary representation of this group
};

class VersionNode
{
public:
    VersionNode();

    VersionNode(const QByteArray _b_group_data);

    VersionNode(const QByteArray _b_group_data,
                const QVector<QByteArray> _pkeys);

    VersionNode(const QByteArray _version_hash,
                const QVector<QByteArray> _pkeys,
                const QByteArray _b_group_data);



    /**
     *  Returns the group represented by the version's hash value
     */
    const Group getGroup(Group &group) const;

    /**
     *  Returns a vector of the parents of the current version
     */
    const QVector<QByteArray> &getParents() const;

    /**
     *  Returns the hash value of the current version
     */
    const QByteArray &getHash() const;

    /**
     *  Returns the binary representation of the group related to the current version
     */
    const QByteArray &getGroupByteArray() const;

    static const VersionNode Zero;
private:
    QSharedDataPointer<VersionNodeData> _data;
};

QDataStream &operator << (QDataStream &out, const VersionNode node);
QDataStream &operator >> (QDataStream &in, VersionNode &node);

}
}

#endif // VERSIONNODE_HPP
