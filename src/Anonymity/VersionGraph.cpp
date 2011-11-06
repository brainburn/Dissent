#include "VersionGraph.hpp"

namespace Dissent {
namespace Anonymity {
    VersionNode VersionGraph::Zero = VersionNode(0);

VersionGraph::VersionGraph()
{
    _data = new VersionGraphData(0, QHash<uint, VersionNode>());
}


VersionGraph::VersionGraph(const VersionNode &_version)
{
    QHash<uint, VersionNode> qh;
    std::cout << "CONSTR: hash assigned: " << qHash(_version.getGroupByteArray()) << std::endl;
    qh[qHash(_version.getGroupByteArray())] = _version;
    _data = new VersionGraphData(qHash(_version.getGroupByteArray()),qh);
}


VersionGraph::VersionGraph(const uint &_current_version,
             const QHash<uint, VersionNode> &_version_db)
{
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

VersionNode &VersionGraph::getVersion(uint hash_key)
{
    if(!_data->VersionDB.contains(hash_key)){
        return VersionGraph::Zero;
    }
    return _data->VersionDB[hash_key];
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
