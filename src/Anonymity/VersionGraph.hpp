#ifndef VERSIONGRAPH_HPP
#define VERSIONGRAPH_HPP

#include "VersionNode.hpp"

namespace Dissent{
namespace Anonymity{

class VersionGraphData : public QSharedData{
public:
    VersionGraphData(const uint current_version,
                     const QHash<uint, VersionNode> version_db):
        CurrentVersion(current_version),
        VersionDB(version_db)
    {
    }

    const uint CurrentVersion;
    QHash<uint, VersionNode> VersionDB;
};

class VersionGraph
{
public:
    VersionGraph();
    VersionGraph(const VersionNode &_version);
    VersionGraph(const uint &_current_version,
                 const QHash<uint, VersionNode> &_version_db);

    /**
     * Loads a version graph from a file on disk
     */
    int load(const QString &filename);

    /**
     * Saves version graph to a file on disk
     */
    int save(const QString &filename);

    /**
     * Returns the current active version of the group
     */
    const uint &getCurrentVersion() const;

    /**
     * Returns the binary represenation of the version graph
     */
    const QHash<uint, VersionNode> &getCurrentVersionDb() const;

    /**
     * Change the current active version
     */
    int setCurrent(const VersionNode vn);

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
    int addNew(QVector<VersionNode> parents, VersionNode &vn);

    static VersionNode Zero;

private:
    QSharedDataPointer<VersionGraphData> _data;

};

QDataStream &operator << (QDataStream &out, const VersionGraph graph);
QDataStream &operator >> (QDataStream &in, VersionGraph &graph);

}
}
#endif // VERSIONGRAPH_HPP
