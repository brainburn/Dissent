#ifndef VERSIONGRAPH_HPP
#define VERSIONGRAPH_HPP

#include "VersionNode.hpp"

namespace Dissent{
namespace Anonymity{
class VersionGraph
{
public:
    VersionGraph();
    VersionGraph(const uint &_current_version,
                 const QHash<uint, VersionNode> &_version_db):
        current_version(_current_version),
        version_db(_version_db)
    {
    }

    uint current_version;
    QHash<uint, VersionNode> version_db;

    int load(const QString &filename);
    int save(const QString &filename);
    const uint &getCurrentVersion() const;
    const QHash<uint, VersionNode> &getCurrentVersionDb() const;
    int setCurrent(const VersionNode vn);
    VersionNode &getVersion(uint version_key);
    int confirm(VersionNode vn);
    int addNew(QVector<VersionNode> parents, VersionNode &vn);
    QVector<VersionNode> getHeads(VersionNode &vn);
    QVector<VersionNode> getHeads();
    QByteArray getBytes();
    int createFromByteArray(QByteArray bvg);


};

QDataStream &operator << (QDataStream &out, const VersionGraph graph);
QDataStream &operator >> (QDataStream &in, VersionGraph &graph);

}
}
#endif // VERSIONGRAPH_HPP
