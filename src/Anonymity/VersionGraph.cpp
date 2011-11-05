#include "VersionGraph.hpp"

namespace Dissent {
namespace Anonymity {
VersionGraph::VersionGraph()
{
    this->current_version = 0;
    this->version_db = QHash<uint, VersionNode>();
}

const uint &VersionGraph::getCurrentVersion() const{
    return this->current_version;
}

const QHash<uint, VersionNode> &VersionGraph::getCurrentVersionDb() const{
    return this->version_db;
}

QDataStream &operator << (QDataStream &out, const VersionGraph graph){
    out << graph.getCurrentVersion() << graph.getCurrentVersionDb();
    return out;
}

QDataStream &operator >> (QDataStream &in, VersionGraph &graph){
    uint _hash_key;
    QHash<uint, VersionNode> _vdb;

    in >> _hash_key;
    in >> _vdb;

    graph = VersionGraph(_hash_key, _vdb);
    return in;
}
}
}
