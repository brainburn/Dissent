#include "VersionGraph.hpp"

namespace Dissent {
namespace Anonymity {
    VersionNode VersionGraph::Zero = VersionNode();

VersionGraph::VersionGraph()
{
    _data = new VersionGraphData(QByteArray(0),
                                 QHash<NODE_ID, VersionNode>(),
                                 QHash<NODE_ID, QVector<NODE_ID> >(),
                                 QHash<QPair<FROM_NODE, TO_NODE>, QList <QByteArray > >());
}


VersionGraph::VersionGraph(const VersionNode & _version)
{
    QHash<NODE_ID, VersionNode>     qh;
    QHash<NODE_ID, QVector<NODE_ID> > ch;

    ch[_version.getHash()] = QVector<QByteArray>();
    qh[_version.getHash()] = _version;

    _data = new VersionGraphData(_version.getHash(),
                                 qh,
                                 ch,
                                 QHash<QPair<FROM_NODE, TO_NODE>, QList <NODE_ID > >());
}

VersionGraph::VersionGraph(const QString &filename)
{
    QByteArray                      data;
    QDataStream                     stream (&data, QIODevice::ReadOnly);
    QHash<NODE_ID, VersionNode>     qh;
    QHash<NODE_ID, QVector<NODE_ID> > ch;
    QHash<QPair<FROM_NODE, TO_NODE>, QList <QByteArray > > conh;
    QByteArray current_version;

    if(!initFromFile(filename, data))
    {
        qWarning() << "Failed to restore graph from file " << filename;
    }


    stream >> current_version;
    stream >> qh;
    stream >> ch;
    stream >> conh;

    _data = new VersionGraphData(current_version,
                                 qh,
                                 ch,
                                 conh);
}


VersionGraph::VersionGraph(const NODE_ID                           &_current_version,
                           const QHash<NODE_ID, VersionNode>       &_version_db,
                           const QHash<NODE_ID, QVector<NODE_ID> > &_children_db,
                           const QHash<QPair<FROM_NODE, TO_NODE>, QList <QByteArray > > &_confirm_db)
{
    _data = new VersionGraphData(_current_version,
                                 _version_db,
                                 _children_db,
                                 _confirm_db);
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
    QByteArray  data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    QFile       file(filename);

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

const QHash<NODE_ID, VersionNode> &VersionGraph::getCurrentVersionDb() const
{
    return _data->VersionDB;
}

const  QHash<NODE_ID, QVector<NODE_ID> > &VersionGraph::getCurrentChildrenDb() const
{
    return _data->ChildrenDB;
}

const QHash<QPair<FROM_NODE, TO_NODE>, QList <QByteArray > > &VersionGraph::getConfirmDb() const
{
    return _data->ConfirmDB;
}

int VersionGraph::getConfirmCount(const QPair<FROM_NODE, TO_NODE> edge) const
{
    if(_data->ConfirmDB.contains(edge)){
        return _data->ConfirmDB[edge].count();
    }
    else{
        return 0;
    }
}

int VersionGraph::confirm(const QPair<FROM_NODE, TO_NODE> &edge, const QByteArray puk)
{
    QVector<NODE_ID>            from_keys;                  // The keys of the members of the parent group
    QVector<AsymmetricKey *>    from_keys_pts;              // Pointers from which we will get the keys
    Group                       from_group(QVector<Id>(0)); // The parent group

    getVersion(edge.first).getGroup(from_group);
    from_keys_pts = from_group.GetKeys();

    // Get the binary representation of the keys
    for(int idx = 0; idx < from_group.GetSize(); idx++){
        from_keys.append(from_keys_pts[idx]->GetByteArray());
    }

    // If the public key belongs to a member of the parent group and
    // this member has not yet confirmed the edge, then add his puk to the list
    if(from_keys.contains(puk) && !_data->ConfirmDB[edge].contains(puk)){
        _data->ConfirmDB[edge].append(puk);
    }

    if(_data->ConfirmDB.contains(edge)){
        return _data->ConfirmDB[edge].count();
    }
    else{
        return 0;
    }
}

const QByteArray &VersionGraph::getCurrentVersion() const
{
    return _data->CurrentVersion;
}

const VersionNode &VersionGraph::getVersion(NODE_ID hash_key)
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

const QVector<QByteArray> VersionGraph::getChildren(const NODE_ID version_hash) const
{
    return _data->ChildrenDB[version_hash];
}

void VersionGraph::addChildren(const NODE_ID version_hash,
                               const QVector<VersionNode *> &children)
{
    for(int idx = 0; idx < children.size(); idx++){
        _data->ChildrenDB[version_hash].append(children[idx]->getHash());
    }
}

void VersionGraph::addChildren(const NODE_ID version_hash,QVector<QByteArray> const &children)
{
    _data->ChildrenDB[version_hash] += children;
}

void VersionGraph::addNew(VersionNode &vn){
    QVector<VersionNode *>  new_child;
    QVector<NODE_ID>     parents = vn.getParents();

    if(!_data->VersionDB.contains(vn.getHash())){
        new_child += &vn;

        _data->VersionDB.insert(vn.getHash(), vn);

        for(int idx = 0; idx < parents.size(); idx++){
            VersionGraph::addChildren(parents[idx], new_child);
        }
    }
}


void VersionGraph::getHeads(QVector<NODE_ID> &heads, NODE_ID v_hash){
    QList<NODE_ID>       children_queue; // The queue to be explored
    QVector <NODE_ID>    children_vector;// The children of the node being explored
    QHash<NODE_ID, bool> visited;
    NODE_ID current_hash;

    children_queue.push_back(v_hash);

    while(!children_queue.isEmpty()){

        current_hash = children_queue[0];
        children_vector = VersionGraph::getChildren(current_hash);

        // Found a head
        if(children_vector.count() == 0){
            heads.append(current_hash);
        }

        // Append children to the queue, save for the ones that are already there
        else{
            for(int idx = 0; idx < children_vector.count(); idx++){
                if(!children_queue.contains(children_vector[idx])){
                    children_queue.push_back(children_vector[idx]);
                }
            }
        }

        // Loop detection
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

    QVector<NODE_ID>     heads;
    QHash<Id, int>          counter_list;   // Count in how many groups a user id appears
    QHash<Id, QByteArray>   id_bkey_map;    // Map from user Id to their kep

    QVector<Id>                 inter_group_ids(0);     // Ids for intersection group
    QVector<Id>                 symdiff_group_ids(0);   // Ids for symm. difference group

    QVector<AsymmetricKey *>    inter_group_keys(0);    // Keys for intersection group
    QVector<AsymmetricKey *>    symdiff_group_keys(0);  // Keys for symm. difference group

    VersionGraph::getHeads(heads, v_hash);

    // Count in how many groups a user id appears
    for(int idx = 0; idx < heads.count(); idx++){
        VersionNode t_vn    = VersionGraph::getVersion(heads[idx]);
        Group       t_group(QVector<Id>(0));

        t_vn.getGroup(t_group);

        QVector<Id> t_ids  = t_group.GetIds();

        for(int idx2 = 0; idx2 < t_ids.count(); idx2++){
            id_bkey_map[t_ids[idx2]] = t_group.GetKey(t_ids[idx2])->GetByteArray();
            if(counter_list.contains(t_ids[idx2])){
                counter_list[t_ids[idx2]] += 1;
            }
            else{
                counter_list[t_ids[idx2]] = 1;
            }
        }


    }

    QList<Id> hkeys = counter_list.uniqueKeys();

    // Match each Id with its public key.
    for(int idx = 0; idx < hkeys.count(); idx++){
        Crypto::AsymmetricKey *key0 = new Crypto::CppPublicKey(id_bkey_map[hkeys[idx]]);
        if(counter_list[hkeys[idx]] == heads.count()){
            inter_group_ids.append(hkeys[idx]);
            inter_group_keys.append(key0);

        }
        else{
            symdiff_group_ids.append(hkeys[idx]);
            symdiff_group_keys.append(key0);
        }
    }

    // Create groups
    intersection            = Group(inter_group_ids, inter_group_keys);
    symmetric_difference    = Group(symdiff_group_ids, symdiff_group_keys);
}

QDataStream &operator << (QDataStream &out, const VersionGraph graph)
{
    out << graph.getCurrentVersion()
        << graph.getCurrentVersionDb()
        << graph.getCurrentChildrenDb()
        << graph.getConfirmDb();

    return out;
}

QDataStream &operator >> (QDataStream &in, VersionGraph &graph)
{
    NODE_ID _hash_key;
    QHash<NODE_ID, VersionNode> _vdb;
    QHash<NODE_ID, QVector<NODE_ID> > _cdb;
    QHash<QPair<FROM_NODE, TO_NODE>, QList <NODE_ID > > _condb;

    in >> _hash_key;
    in >> _vdb;
    in >> _cdb;
    in >> _condb;

    graph = VersionGraph(_hash_key, _vdb, _cdb, _condb);
    return in;
}

}
}
