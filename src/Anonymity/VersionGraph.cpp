#include "VersionGraph.hpp"

namespace Dissent {
namespace Anonymity {
    VersionNode VersionGraph::Zero = VersionNode();

VersionGraph::VersionGraph()
{
    _data = new VersionGraphData(QByteArray(0), QHash<QByteArray, VersionNode>());
}


VersionGraph::VersionGraph(const VersionNode & _version)
{
    QHash<QByteArray, VersionNode> qh;
    qh[_version.getHash()] = _version;
    _data = new VersionGraphData(_version.getHash(),qh);
}

VersionGraph::VersionGraph(const QString &filename)
{
    QByteArray data;
    QDataStream stream (&data, QIODevice::ReadOnly);
    QHash<QByteArray, VersionNode> qh;
    QByteArray current_version;

    if(!initFromFile(filename, data))
    {
        qWarning() << "Failed to restore graph from file " << filename;
    }


    stream >> current_version;
    stream >> qh;

    _data = new VersionGraphData(current_version, qh);
}


VersionGraph::VersionGraph(const QByteArray &_current_version,
             const QHash<QByteArray, VersionNode> &_version_db)
{
    _data = new VersionGraphData(_current_version, _version_db);
}

bool VersionGraph::initFromFile(const QString &filename, QByteArray &data)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
      qWarning() << "Error (" << file.error() << ") reading file: " << filename;
      return false;
    }

    data = file.readAll();
    return true;
}

bool VersionGraph::save(const QString &filename)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QFile file(filename);

    stream << *this;

    if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
      qWarning() << "Error (" << file.error() << ") saving file: " << filename;
      return false;
    }

    file.write(data);
    file.flush();
    file.close();
    return true;
}

const QHash<QByteArray, VersionNode> &VersionGraph::getCurrentVersionDb() const
{
    return _data->VersionDB;
}

const QByteArray &VersionGraph::getCurrentVersion() const
{
    return _data->CurrentVersion;
}

VersionNode &VersionGraph::getVersion(QByteArray hash_key)
{
    if(!_data->VersionDB.contains(hash_key)){
        return VersionGraph::Zero;
    }
    return _data->VersionDB[hash_key];
}


QByteArray VersionGraph::setCurrentVersion(const VersionNode vn)
{
    _data->CurrentVersion = vn.getHash();
    return this->getCurrentVersion();
}

/*
void VersionGraph::addNew(QVector<uint> &parents, VersionNode &vn){
    QVector<VersionNode *> new_child;
    uint hash_key = vn.getHash();
    new_child += &vn;

    _data->VersionDB.insert(vn.getHash(), vn);

    for(int idx = 0; idx < parents.size(); idx++){
        this->getVersion(parents[idx]).addChildren(new_child);
    }

    this->getVersion(hash_key).addParents(parents);

}
*/
/*
void VersionGraph::getHeads(QHash<uint, uint> &heads, uint v_hash){
    VersionNode &vn = this->getVersion(v_hash);
    const QVector<uint> &children = vn.getChildren();
    if(children.size() == 0){
        if(!heads.contains(vn.getHash())){
            heads.insert(vn.getHash(),vn.getHash());
        }
        return;
    }
    else{
        for(int idx = 0; idx < children.size(); idx++){
            VersionGraph::getHeads(heads,children[idx]);
        }
    }
}
*/

QDataStream &operator << (QDataStream &out, const VersionGraph graph)
{
    out << graph.getCurrentVersion() << graph.getCurrentVersionDb();
    return out;
}

QDataStream &operator >> (QDataStream &in, VersionGraph &graph)
{
    QByteArray _hash_key;
    QHash<QByteArray, VersionNode> _vdb;

    in >> _hash_key;
    in >> _vdb;

    graph = VersionGraph(_hash_key, _vdb);
    return in;
}

}
}
