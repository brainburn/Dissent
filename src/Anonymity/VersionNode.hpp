#ifndef VERSIONNODE_HPP
#define VERSIONNODE_HPP

#include "Group.hpp"
#include "Crypto/CppHash.hpp"
#include <QHash>
#include <QVariant>
#include <QMap>


namespace Dissent{
namespace Anonymity{

/**
 * Private data structure for VersionNode storage.
 */
class VersionNodeData : public QSharedData{
public:
    VersionNodeData(const QByteArray version_hash,
                    const QVector<QByteArray> pkeys,
                    const QByteArray b_group_data,
                    const QMap<QString, QVariant> group_policy):
        VersionHash(version_hash),
        PKeys(pkeys),
        BGroupData(b_group_data),
        GroupPolicy(group_policy)
    {
    }
    const QByteArray                VersionHash; // Hash value of this group
    const QVector<QByteArray>       PKeys;       // Parent hash keys
    const QByteArray                BGroupData;  // Binary representation of this group
    const QMap<QString, QVariant>   GroupPolicy; // Contains the group policy of node
};

class VersionNode
{
public:
    /**
     * Constructor - create an empty VersionNode, where each state is either 0
     *               or empty.
     */
    VersionNode();

    /**
     * Constructor - used mainly for graph initialization
     * @param _b_group_data the binary representation of a group
     */
    VersionNode(const QByteArray _b_group_data);

    /**
     * Constructor - create a node that will be added to a graph
     * @param _b_group_data the binary representation fo a group
     * @param _pkeys a QVector containing the hash keys of all of the node's parents
     * @param _group_policy the group policy of the group represented by the node
     */
    VersionNode(const QByteArray _b_group_data,
                const QVector<QByteArray> _pkeys,
                const QMap<QString, QVariant> _group_policy);

    /**
     * Constructor - used mainly for deserialization
     * @param _version_hash the version hash of the current version
     * @param _b_group_data the binary representation fo a group
     * @param _pkeys a QVector containing the hash keys of all of the node's parents
     * @param _group_policy the group policy of the group represented by the node
     */
    VersionNode(const QByteArray _version_hash,
                const QByteArray _b_group_data,
                const QVector<QByteArray> _pkeys,
                const QMap<QString, QVariant> _group_policy);



    /**
     *  @param group to be set to the one represented by the version's hash value
     */
    const Group getGroup(Group &group) const;

    /**
     *  Returns a QMap of the policy of the current version
     */
    const QMap<QString, QVariant> &getPolicy() const;


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
