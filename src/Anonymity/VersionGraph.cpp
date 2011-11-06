#include "VersionGraph.hpp"

namespace Dissent {
namespace Anonymity {
VersionGraph::VersionGraph()
{
    _data = new VersionGraphData(0, QHash<uint, VersionNode>());
}

VersionGraph::VersionGraph(const uint &_current_version,
             const QHash<uint, VersionNode> &_version_db){
    _data = new VersionGraphData(_current_version, _version_db);
}

const uint &VersionGraph::getCurrentVersion() const
{
    return _data->CurrentVersion;
}

const QHash<uint, VersionNode> &VersionGraph::getCurrentVersionDb() const
{
    return _data->VersionDB;
}

QDataStream &operator << (QDataStream &out, const VersionGraph graph)
{
    out << graph.getCurrentVersion() << graph.getCurrentVersionDb();
    return out;
}

QDataStream &operator >> (QDataStream &in, VersionGraph &graph)
{
    uint _hash_key;
    QHash<uint, VersionNode> _vdb;

    in >> _hash_key;
    in >> _vdb;

    graph = VersionGraph(_hash_key, _vdb);
    return in;
}
}
}
