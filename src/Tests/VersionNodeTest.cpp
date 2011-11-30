#include "DissentTest.hpp"

using namespace Dissent::Connections;
using namespace Dissent::Anonymity;

namespace Dissent {
namespace Tests {

TEST(VersionNode, Serialization)
{
  DisableLogging();
     Id                         id[10];
     QByteArray                 binary_group_data;
     QDataStream                strm(&binary_group_data, QIODevice::ReadWrite);
     QVector<Id>                group_vector;
     QVector<AsymmetricKey *>   key_vector;

     for(int idx = 0; idx < 10; idx++) {
       AsymmetricKey *key0 = new CppPrivateKey();
       group_vector.append(id[idx]);
       key_vector.append(key0->GetPublicKey());
       delete key0;
     }

     Group temp_group(group_vector,key_vector);

     strm << temp_group;

     VersionNode vn(binary_group_data);
     binary_group_data.clear();

     QByteArray     z;
     QDataStream    strm1(&z, QIODevice::WriteOnly);
     QDataStream    strm2(&z, QIODevice::ReadOnly);

     strm1 << vn;
     strm2 >> vn;

     Group group = Group(QVector<Id>(0));

     vn.getGroup(group);

     // Do the tests for Group.Basic. If it survives,
     // serialization/deserializaion works
     for(int idx = 0; idx < 10; idx++){
         EXPECT_TRUE(group.GetKey(idx)->IsValid());
     }

     EXPECT_EQ(group.GetSize(), 10);
     for(int idx = 0; idx < 10; idx++) {
       EXPECT_EQ(id[idx], group.GetId(idx));
       EXPECT_EQ(idx, group.GetIndex(id[idx]));
       EXPECT_TRUE(group.Contains(id[idx]));
       if(idx == 9) {
         EXPECT_EQ(group.Next(id[idx]), Id::Zero);
         EXPECT_EQ(group.Previous(id[idx]), group.GetId(idx - 1));
       } else if(idx == 0) {
         EXPECT_EQ(group.Next(id[idx]), group.GetId(idx + 1));
         EXPECT_EQ(group.Previous(id[idx]), Id::Zero);
       } else {
         EXPECT_EQ(group.Next(id[idx]), group.GetId(idx + 1));
         EXPECT_EQ(group.Previous(id[idx]), group.GetId(idx - 1));
       }
     }

     Id id0;
     EXPECT_FALSE(group.Contains(id0));

     QVector<Id> group_vector0;
     for(int idx = 9; idx >= 0; idx--) {
       group_vector0.append(id[idx]);
     }
     Group group0(group_vector0);
     for(int idx = 0; idx < 10; idx++) {
       EXPECT_NE(group.GetId(idx), group0.GetId(idx));
       EXPECT_EQ(group.GetId(idx), group.GetId(idx));
       EXPECT_EQ(group0.GetId(idx), group0.GetId(idx));
     }
  EnableLogging();
}

TEST(VersionNode, Methods)
{
    DisableLogging();
    VersionNode             vn;
    QVector<VersionNode *>  node_parents;
    QVector<QByteArray> pv;
    QVector<QByteArray> cv;
    for(uint idx = 0; idx < 10; idx++){
        VersionNode *temp_node;

        QByteArray qb;

        qb += (idx);
        pv.append(qb);
        temp_node = new VersionNode();
        temp_node->setHash(idx+48);
        temp_node->addParents(pv);

        node_parents.append(temp_node);
    }

    vn.addParents(pv);

    VersionNode vn2(QByteArray(0), pv, pv);
    VersionNode vn3(QByteArray(0), pv, cv);

    EXPECT_EQ(vn.getParents(), vn2.getParents());
    EXPECT_EQ(vn2.getParents(), vn3.getParents());
    EXPECT_EQ(vn.getParents().count(), 10);
    vn.addParents(node_parents);
    EXPECT_EQ(vn.getParents().count(), 20);


    EXPECT_EQ(vn2.getHash(), vn2.getHash());
    EXPECT_NE(vn2.getHash(), vn.getHash());
    EXPECT_NE(vn2.getHash(), vn3.getHash());
    EXPECT_NE(vn.getHash(), vn3.getHash());

    for(int idx = 0; idx < 5; idx++){
        delete node_parents[idx];
    }
    EnableLogging();
}

}
}

