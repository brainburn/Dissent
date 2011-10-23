#include "DissentTest.hpp"

using namespace Dissent::Connections;
using namespace Dissent::Anonymity;

namespace Dissent {
namespace Tests {

  TEST(Group, Basic)
  {
    Id id[10];

    QByteArray qb;
    QDataStream strm(&qb, QIODevice::WriteOnly);

    QVector<Id> group_vector;
    for(int idx = 0; idx < 10; idx++) {
      group_vector.append(id[idx]);
    }

    Group group2(group_vector);
    strm << group2;

    Group group(group_vector);
    std::cout << "ID_1  " << group.GetId(1).GetInteger() << std::endl;
    QDataStream stream2(&qb, QIODevice::ReadOnly);
     stream2 >> group;
    std::cout << "ID_1  " << group.GetId(1).GetInteger() << std::endl;

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

  }
}
}
