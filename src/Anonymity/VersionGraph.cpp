#include "VersionGraph.hpp"

namespace Dissent {
namespace Anonymity {
    VersionNode VersionGraph::Zero = VersionNode();

VersionGraph::VersionGraph()
{
    _data = new VersionGraphData(QByteArray(0),
                                 QHash<QByteArray, VersionNode>(),
                                 QHash<QByteArray, QVector<QByteArray> >());
}


VersionGraph::VersionGraph(const VersionNode & _version)
{
    QHash<QByteArray, VersionNode> qh;
    QHash<QByteArray, QVector<QByteArray> > ch;

    ch[_version.getHash()] = QVector<QByteArray>();
    qh[_version.getHash()] = _version;

    _data = new VersionGraphData(_version.getHash(),
                                 qh,
                                 ch);
}

VersionGraph::VersionGraph(const QString &filename)
{
    QByteArray data;
    QDataStream stream (&data, QIODevice::ReadOnly);
    QHash<QByteArray, VersionNode> qh;
    QHash<QByteArray, QVector<QByteArray> > ch;
    QByteArray current_version;

    if(!initFromFile(filename, data))
    {
        qWarning() << "Failed to restore graph from file " << filename;
    }


    stream >> current_version;
    stream >> qh;
    stream >> ch;

    _data = new VersionGraphData(current_version,
                                 qh,
                                 ch);
}


VersionGraph::VersionGraph(const QByteArray &_current_version,
                           const QHash<QByteArray, VersionNode> &_version_db,
                           const QHash<QByteArray, QVector<QByteArray> > &_children_db)
{
    _data = new VersionGraphData(_current_version,
                                 _version_db,
                                 _children_db);
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

const  QHash<QByteArray, QVector<QByteArray> > &VersionGraph::getCurrentChildrenDb() const
{
    return _data->ChildrenDB;
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

QVector<QByteArray> VersionGraph::getChildren(const QByteArray version_hash) const
{
    return _data->ChildrenDB[version_hash];
}

void VersionGraph::addChildren(const QByteArray version_hash,
                               const QVector<VersionNode *> &children)
{
    for(int idx = 0; idx < children.size(); idx++){
        _data->ChildrenDB[version_hash].append(children[idx]->getHash());
    }
}

void VersionGraph::addChildren(const QByteArray version_hash,QVector<QByteArray> const &children)
{
    _data->ChildrenDB[version_hash] += children;
}

void VersionGraph::addNew(VersionNode &vn){
    QVector<VersionNode *> new_child;
    QVector<QByteArray> parents = vn.getParents();
    //QByteArray hash_key = vn.getHash();
    new_child += &vn;

    _data->VersionDB.insert(vn.getHash(), vn);

    for(int idx = 0; idx < parents.size(); idx++){
        VersionGraph::addChildren(parents[idx], new_child);
    }
}


void VersionGraph::getHeads(QVector<QByteArray> &heads, QByteArray v_hash){
    QList<QByteArray>       children_queue;
    QVector <QByteArray>    children_vector;
    QHash<QByteArray, bool> visited;
    QByteArray current_hash;

    children_queue.push_back(v_hash);

    while(!children_queue.isEmpty()){

        current_hash = children_queue[0];
        children_vector = VersionGraph::getChildren(current_hash);

        if(children_vector.count() == 0){
            heads.append(current_hash);
        }
        else{
            for(int idx = 0; idx < children_vector.count(); idx++){
                if(!children_queue.contains(children_vector[idx])){
                    children_queue.push_back(children_vector[idx]);
                }
            }
        }

        if(visited.contains(current_hash)){
            std::cerr << "Loop detected in version graph. Graph is corrupt." << std::endl;
            break;
        }

        visited.insert(current_hash, true);
        children_queue.pop_front();
    }
}

void VersionGraph::getHeadsIaSD(Group &intersection,
                                Group &symmetric_difference, QByteArray v_hash){
    QVector<QByteArray> heads;
    QHash<Id, int> counter_list;

    QVector<Id>                 inter_group_ids(0);
    QVector<Id>                 symdiff_group_ids(0);

    QVector<AsymmetricKey *>    inter_group_keys(0);
    QVector<AsymmetricKey *>    symdiff_group_keys(0);

    VersionGraph::getHeads(heads, v_hash);

    for(int idx = 0; idx < heads.count(); idx++){
        VersionNode t_vn   = VersionGraph::getVersion(heads[idx]);
        Group t_group(QVector<Id>(0));

        t_vn.getGroup(t_group);

        QVector<Id> t_ids  = t_group.GetIds();

        for(int idx2 = 0; idx2 < t_ids.count(); idx2++){
            if(counter_list.contains(t_ids[idx2])){
                counter_list[t_ids[idx2]] += 1;
            }
            else{
                counter_list[t_ids[idx2]] = 1;
            }
        }


    }

    std::cout << "List Count : " << counter_list.count() << std::endl;
    QList<Id> hkeys = counter_list.uniqueKeys();


    for(int idx = 0; idx < hkeys.count(); idx++){
        Crypto::AsymmetricKey *key0 = new Crypto::CppPrivateKey();
        if(counter_list[hkeys[idx]] == heads.count()){
            inter_group_ids.append(hkeys[idx]);
            inter_group_keys.append(key0->GetPublicKey());

        }
        else{
            symdiff_group_ids.append(hkeys[idx]);
            symdiff_group_keys.append(key0->GetPublicKey());
        }
        delete key0;
    }
    std::cout << "Matching Ids : " << inter_group_ids.count() << std::endl;

    intersection            = Group(inter_group_ids, inter_group_keys);
    symmetric_difference    = Group(symdiff_group_ids, symdiff_group_keys);
}

QDataStream &operator << (QDataStream &out, const VersionGraph graph)
{
    out << graph.getCurrentVersion() << graph.getCurrentVersionDb() << graph.getCurrentChildrenDb();
    return out;
}

QDataStream &operator >> (QDataStream &in, VersionGraph &graph)
{
    QByteArray _hash_key;
    QHash<QByteArray, VersionNode> _vdb;
    QHash<QByteArray, QVector<QByteArray> > _cdb;
    in >> _hash_key;
    in >> _vdb;
    in >> _cdb;
    graph = VersionGraph(_hash_key, _vdb, _cdb);
    return in;
}

}
}
