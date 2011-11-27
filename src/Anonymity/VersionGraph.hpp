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
    VersionGraphData(const uint current_version,
                     const QHash<uint, VersionNode> version_db):
        CurrentVersion(current_version),
        VersionDB(version_db)
    {
    }

    uint CurrentVersion;
    QHash<uint, VersionNode> VersionDB;
};

class VersionGraph
{
public:
    VersionGraph();
    VersionGraph(const VersionNode &_version);
    VersionGraph(const QString &filename);
    VersionGraph(const uint &_current_version,
                 const QHash<uint, VersionNode> &_version_db);


    /**
     * Saves version graph to a file on disk
     */
    bool save(const QString &filename);

    /**
     * Returns the current active version of the group
     */
    const uint &getCurrentVersion() const;

    /**
     * Returns the version graph
     */
    const QHash<uint, VersionNode> &getCurrentVersionDb() const;

    /**
     * Change the current active version
     */
    uint setCurrentVersion(const VersionNode vn);

    /**
     * Returns the version associated with the given hash key
     */
    VersionNode& getVersion(uint hash_key);

    /**
     * Confirms the given version ??
     */
    int confirm(VersionNode vn);

    /**
     * Adds a new version as a child to the given parents
     */
    void addNew(QVector<uint> &parents, VersionNode &vn);

    /**
     *  Returns the heads of the subgraph starting from vn
     *  !! Does not detect cycles !!
     */

    void getHeads(QHash<uint, uint> &heads,uint v_hash);

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
