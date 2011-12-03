#ifndef VERSIONGRAPH_HPP
#define VERSIONGRAPH_HPP

#include "VersionNode.hpp"

namespace Dissent{
namespace Anonymity{

/**
 * Private data structure for VersionGraph storage.
 */
class VersionGraphData : public QSharedData{
public:
    VersionGraphData(const QByteArray current_version,
                     const QHash<QByteArray, VersionNode> version_db,
                     const QHash<QByteArray, QVector<QByteArray> > children_db):
        CurrentVersion(current_version),
        VersionDB(version_db),
        ChildrenDB(children_db)
    {
    }

    QByteArray                              CurrentVersion;
    QHash<QByteArray, VersionNode>          VersionDB;
    QHash<QByteArray, QVector<QByteArray> > ChildrenDB;

};

class VersionGraph
{
public:
    VersionGraph();

    VersionGraph(const VersionNode &_version);

    VersionGraph(const QString &filename);

    VersionGraph(const QByteArray &_current_version,
                 const QHash<QByteArray, VersionNode> &_version_db,
                 const QHash<QByteArray, QVector<QByteArray> > &children_db);


    /**
     * Saves version graph to a file on disk
     */
    bool save(const QString &filename);

    /**
     * Returns the current active version of the group
     */
    const QByteArray &getCurrentVersion() const;

    /**
     * Returns the version nodes database
     */
    const QHash<QByteArray, VersionNode> &getCurrentVersionDb() const;

    /**
     * Returns the chilren edges database
     */
    const  QHash<QByteArray, QVector<QByteArray> > &getCurrentChildrenDb() const;

    /**
     * Change the current active version
     */
    QByteArray setCurrentVersion(const VersionNode vn);

    /**
     * Returns the version associated with the given hash key CONST ME!
     */
    VersionNode& getVersion(QByteArray hash_key);

    /**
     *  Returns a vector of versions that succeed the current one
     */
    QVector<QByteArray> getChildren(const QByteArray version_hash) const;

    /**
     *  Adds the given children to a node in the version graph
     *  @param version vectors, whose hash keys will be added to the parent node
     */
    void addChildren(const QByteArray version_hash ,const QVector<VersionNode *> &children);

    void addChildren(const QByteArray version_hash, QVector<QByteArray> const &children);

    /**
     *  Confirms the given version ??
     *  @param vn VersionNode to be confirmed
     */
    int confirm(VersionNode vn);

    /**
     *  Adds a new version as a child to the given parents
     *  @param vn VersionNode to be added
     */
    void addNew(VersionNode &vn);

    /**
     *  Returns the heads of the subgraph starting from vn
     *  @param the heads descending from version v_hash
     *  @param v_hash the version, the heads descending from which we are looking for
     */
    void getHeads(QVector<QByteArray> &heads,QByteArray v_hash);

    /**
     *  Sets intersection to the intersection and symmetric_difference the symmetric difference
     *  of the sets of members represented by the head version nodes starting,
     *  descending from v_hash.
     *  @param intersection the intersection of the graph heads
     *  @param symmetric_difference the symmetric difference of the graph heads
     */
    void getHeadsIaSD(Group &intersection, Group &symmetric_difference, QByteArray v_hash);

    static VersionNode Zero;

private:

    /**
     * Loads a version graph from a file on disk
     */
    bool initFromFile(const QString &filename, QByteArray &data);

    QSharedDataPointer<VersionGraphData> _data;

};

QDataStream &operator << (QDataStream &out, const VersionGraph graph);
QDataStream &operator >> (QDataStream &in, VersionGraph &graph);

}
}
#endif // VERSIONGRAPH_HPP
