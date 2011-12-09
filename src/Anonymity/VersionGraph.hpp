#ifndef VERSIONGRAPH_HPP
#define VERSIONGRAPH_HPP

#include "VersionNode.hpp"

#define FROM_NODE   QByteArray // Edge start node
#define TO_NODE     QByteArray // Edge end node
#define PUK         QByteArray // Public Key
#define NODE_ID     QByteArray // Cryptographic version node hash

namespace Dissent{
namespace Anonymity{

/**
 * Private data structure for VersionGraph storage.
 */
class VersionGraphData : public QSharedData{
public:
    VersionGraphData(const NODE_ID current_version,
                     const QHash<NODE_ID, VersionNode> version_db,
                     const QHash<NODE_ID, QVector<NODE_ID> > children_db,
                     const QHash<QPair<FROM_NODE, TO_NODE>, QList <PUK > > confirm_db):
        CurrentVersion(current_version),
        VersionDB(version_db),
        ChildrenDB(children_db),
        ConfirmDB(confirm_db)
    {
    }

    NODE_ID                              CurrentVersion; // The hash of the current active version
    QHash<NODE_ID, VersionNode>          VersionDB;      // Nodes and edges towards parents
    QHash<NODE_ID, QVector<NODE_ID> >    ChildrenDB;     // Edges toward children
    QHash<QPair<FROM_NODE, TO_NODE>, QList <PUK > > ConfirmDB; // Versions confirms list
};

class VersionGraph
{
public:

    /**
     * Constructor - creates an instance of a graph, where every state is set to 0 or empty;
     */
    VersionGraph();

    /**
     * Constructor - used for initialization
     * @param _version from which the graph is to be initialized
     */
    VersionGraph(const VersionNode &_version);

    /**
     * Constructor - used for loading a graph from a file
     * @param filename of the file containing the graph data
     */
    VersionGraph(const QString &filename);

    /**
     * Constructor - used mainly for deserialization
     * @param _current_version the hash of the currently active version
     * @param _version_db the version database
     * @param _children_db the edges to the children nodes
     * @param _confirm_db the versions confirmations list
     */
    VersionGraph(const QByteArray &_current_version,
                 const QHash<NODE_ID, VersionNode> &_version_db,
                 const QHash<NODE_ID, QVector<NODE_ID> > &_children_db,
                 const QHash<QPair<FROM_NODE, TO_NODE>, QList <QByteArray > > &_confirm_db);


    /**
     * Saves version graph to a file on disk
     * @param filename under which the graph will be saved on disk
     */
    bool save(const QString &filename);

    /**
     * Returns the current active version of the group
     */
    const QByteArray &getCurrentVersion() const;

    /**
     * Returns the version nodes database
     */
    const QHash<NODE_ID, VersionNode> &getCurrentVersionDb() const;

    /**
     * Returns the chilren edges database
     */
    const  QHash<NODE_ID, QVector<NODE_ID> > &getCurrentChildrenDb() const;

    /**
     * Returns the versions confirmations list database
     */
    const QHash<QPair<FROM_NODE, TO_NODE>, QList <QByteArray > > &getConfirmDb() const;

    /**
     * Returns the number of confirms the edge in the graph has received
     * @param edge connecting a from and to node
     */
    int getConfirmCount(const QPair<FROM_NODE, TO_NODE> edge) const;

    /**
     *  Confirms the version on the to side of the edge({from, to} pair), by signign it with
     *  the given public key. Returns the number of confirmations an edge has received
     *  @param edge representing a {from, to} pair, where to is the hash of the version to be
     *              confirmed
     *  @param puk the public key of the member who wishes to confirm the edge
     */
    int confirm(const QPair<FROM_NODE, TO_NODE> &edge, const PUK puk);

    /**
     * Change the current active version
     * @param vn the VersionNode, to the version hash of which we will set the current version
     */
    QByteArray setCurrentVersion(const VersionNode vn);

    /**
     * Returns the version associated with the given hash key
     * @param version_hash the hash key of the version to be returned
     */
    const VersionNode& getVersion(NODE_ID version_hash);

    /**
     *  Returns a vector of versions that succeed the current one
     *  @param version_hash the hash key of the version whose children we want to get
     */
    const QVector<QByteArray> getChildren(const NODE_ID version_hash) const;

    /**
     *  Adds the given children to a node in the version graph
     *  @param version_hash of the version to which we want to append children
     *  @param children that we waht to append
     */
    void addChildren(const NODE_ID version_hash, const QVector<VersionNode *> &children);

    /**
     *  Adds the given children to a node in the version graph
     *  @param version_hash of the version to which we want to append children
     *  @param children that we waht to append
     */
    void addChildren(const NODE_ID version_hash, QVector<QByteArray> const &children);

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
    void getHeads(QVector<NODE_ID> &heads, NODE_ID v_hash);

    /**
     *  Sets intersection to the intersection and symmetric_difference the symmetric difference
     *  of the sets of members represented by the head version nodes starting,
     *  descending from v_hash.
     *  @param intersection the intersection of the graph heads
     *  @param symmetric_difference the symmetric difference of the graph heads
     */
    void getHeadsIaSD(Group &intersection, Group &symmetric_difference, NODE_ID v_hash);

    static VersionNode Zero;

private:

    /**
     * Loads a version graph from a file on disk
     * @param filename of the file to be loaded
     * @param data where the data from the file is to be stored
     */
    bool initFromFile(const QString &filename, QByteArray &data);

    QSharedDataPointer<VersionGraphData> _data;

};

QDataStream &operator << (QDataStream &out, const VersionGraph graph);
QDataStream &operator >> (QDataStream &in, VersionGraph &graph);

}
}
#endif // VERSIONGRAPH_HPP
