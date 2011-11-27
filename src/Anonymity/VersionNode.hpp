#ifndef VERSIONNODE_HPP
#define VERSIONNODE_HPP

#include "Group.hpp"
#include <QHash>


namespace Dissent{
namespace Anonymity{

class VersionNodeData : public QSharedData{
public:
    VersionNodeData(const uint group_hash,
                    const QVector<uint> pkeys,
                    const QVector<uint> ckeys,
                    const QByteArray b_group_data):
        GroupHash(group_hash),
        PKeys(pkeys),
        CKeys(ckeys),
        BGroupData(b_group_data)
    {
    }

    uint GroupHash;         // Hash value of this group
    QVector<uint> PKeys;    // Parent hash keys
    QVector<uint> CKeys;    // Children hash keys
    const QByteArray BGroupData;  // Binary representation of this group
};

class VersionNode
{
public:
    VersionNode();

    VersionNode(QByteArray _b_group_data);

    VersionNode(const uint _group_hash, const QVector<uint> _pkeys,
                const QVector<uint> _ckeys, const QByteArray _b_group_data);



    /**
     *  Returns the group represented by the version's hash value
     */
    const Group getGroup(Group &group) const;

    /**
     *  Sets the hash value of current version. For testing purposes.
     */
    inline void setHash(uint x){ _data->GroupHash = x; }

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

    /**
     *  Add the given parents to a node in the version graph
     *  @param version vector, whose hash keys will be added to the child node
     */
    void addParents(const QVector<VersionNode *> &parents);

    void addParents(QVector<uint> &parents);

    /**
     *  Adds the given children to a node in the version graph
     *  @param version vectors, whose hash keys will be added to the parent node
     */
    void addChildren(const QVector<VersionNode *> &children);

    void addChildren(QVector<uint> const &children);

    static const VersionNode Zero;
private:
    QSharedDataPointer<VersionNodeData> _data;
};

QDataStream &operator << (QDataStream &out, const VersionNode node);
QDataStream &operator >> (QDataStream &in, VersionNode &node);

}
}

#endif // VERSIONNODE_HPP
