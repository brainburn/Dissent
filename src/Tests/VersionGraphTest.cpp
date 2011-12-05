#include "DissentTest.hpp"

using namespace Dissent::Connections;
using namespace Dissent::Anonymity;

namespace Dissent {
namespace Tests {
TEST(VersionGraph, All)
{
    DisableLogging();

    Id                          id[10];

    // Group 1
    QVector<Id>                 group_vector;
    QVector<AsymmetricKey *>    key_vector;

    // Group 2 - subset of Group 1
    QVector<Id>                 group_vector2;
    QVector<AsymmetricKey *>    key_vector2;

    // Sample policy
    QMap<QString, QVariant>     gpolicy;

    gpolicy.insert("Quorum", 10);

    for(int idx = 0; idx < 10; idx++) {
      AsymmetricKey *key0 = new CppPrivateKey();
      group_vector.append(id[idx]);
      key_vector.append(key0->GetPublicKey());

      if(idx < 3){
          group_vector2.append(id[idx]);
          key_vector2.append(key0->GetPublicKey());
      }
    }

    Group       group(group_vector,key_vector);
    Group       group2(group_vector2,key_vector2);

    QByteArray  qb;
    QDataStream strm(&qb, QIODevice::ReadWrite);
    QByteArray  qb2;
    QDataStream strm2(&qb2, QIODevice::ReadWrite);

    strm << group;
    strm2 << group2;

    VersionNode vn(qb);
    VersionGraph vg2 = VersionGraph(vn);

    QVector<QByteArray> parents_vector;

    parents_vector.append(vn.getHash());

    VersionNode vn2 = VersionNode(qb, parents_vector, gpolicy);

    parents_vector.clear();
    parents_vector.append(vn2.getHash());


    VersionNode vn3 = VersionNode(qb2, parents_vector, gpolicy);
    VersionNode vn4 = VersionNode(qb, parents_vector, gpolicy);

    parents_vector.clear();
    parents_vector.append(vn4.getHash());

    VersionNode vn5 = VersionNode(qb, parents_vector, gpolicy);
    VersionNode vn6 = VersionNode(qb2, parents_vector, gpolicy);



    parents_vector.clear();
    parents_vector.append(vn5.getHash());
    parents_vector.append(vn6.getHash());

    VersionNode vn7 = VersionNode(qb, parents_vector, gpolicy);

    vg2.addNew(vn2);
    vg2.addNew(vn3);
    vg2.addNew(vn4);
    vg2.addNew(vn5);
    vg2.addNew(vn6);

    // getHeads tests start
    QVector<QByteArray> heads;

    vg2.getHeads(heads, vn.getHash());

    EXPECT_EQ(heads.count(), 3);

    heads.clear();

    vg2.getHeads(heads, vn2.getHash());
    EXPECT_EQ(heads.count(), 3);

    heads.clear();

    vg2.getHeads(heads, vn3.getHash());
    EXPECT_EQ(heads.count(), 1);

    heads.clear();

    vg2.getHeads(heads, vn4.getHash());
    EXPECT_EQ(heads.count(), 2);

    heads.clear();

    // getHeads tests end

    vg2.confirm(QPair<QByteArray,QByteArray>(vn3.getHash(), vn5.getHash()),
                                  key_vector2[1]->GetByteArray());

    EXPECT_EQ(vg2.getConfirmCount(QPair<QByteArray,QByteArray>(vn3.getHash(), vn5.getHash())), 1);

    EXPECT_EQ(vg2.confirm(QPair<QByteArray,QByteArray>(vn3.getHash(), vn5.getHash()),
                              key_vector2[0]->GetByteArray()), 2);

    vg2.save("GraphFile");

    VersionGraph vg = VersionGraph("GraphFile");

    EXPECT_EQ(vg.getConfirmCount(QPair<QByteArray,QByteArray>(vn3.getHash(), vn5.getHash())), 2);

    EXPECT_EQ(vg.confirm(QPair<QByteArray,QByteArray>(vn3.getHash(), vn5.getHash()),
                                  key_vector2[0]->GetByteArray()), 2);

    EXPECT_EQ(vg.confirm(QPair<QByteArray,QByteArray>(vn3.getHash(), vn5.getHash()),
                                  key_vector2[2]->GetByteArray()), 3);

    EXPECT_EQ(vg2.getConfirmCount(QPair<QByteArray,QByteArray>(vn3.getHash(), vn5.getHash())), 2);


    heads.clear();

    vg.getHeads(heads, vn.getHash());

    EXPECT_EQ(heads.count(), 3);

    vg.addNew(vn7);

    heads.clear();

    vg.getHeads(heads, vn.getHash());

    EXPECT_EQ(heads.count(), 2);

    heads.clear();

    vg2.getHeads(heads, vn.getHash());

    EXPECT_EQ(heads.count(), 3);

    Group       intersection(QVector<Id>(0),QVector<AsymmetricKey *>(0));
    Group       symmetric_diff(QVector<Id>(0),QVector<AsymmetricKey *>(0));

    vg.getHeadsIaSD(intersection, symmetric_diff, vn.getHash());


    EXPECT_EQ(intersection.GetSize(), 3);

    EXPECT_EQ(symmetric_diff.GetSize(), 7);

    EnableLogging();
}

}
}
