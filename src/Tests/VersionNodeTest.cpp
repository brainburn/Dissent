#include "DissentTest.hpp"

using namespace Dissent::Connections;
using namespace Dissent::Anonymity;

namespace Dissent {
namespace Tests {
TEST(VersionNode, Basic)
{
  DisableLogging();
     Id id[10];
     QByteArray qb;
     QDataStream strm(&qb, QIODevice::ReadWrite);

     QVector<Id> group_vector;
     QVector<AsymmetricKey *> key_vector;
     for(int idx = 0; idx < 10; idx++) {
       AsymmetricKey *key0 = new CppPrivateKey();
       group_vector.append(id[idx]);
       key_vector.append(key0->GetPublicKey());
     }

     Group group2(group_vector,key_vector);
     strm << group2;

     VersionNode vn(qb);
     qb.clear();

     QByteArray z;
     QDataStream strm1(&z, QIODevice::WriteOnly);
     strm1 << vn;
     QDataStream strm2(&z, QIODevice::ReadOnly);
     //int q = -7;
     strm2 >> vn;

     Group group = Group(QVector<Id>(0));
     vn.getGroup(group);

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
}
}
