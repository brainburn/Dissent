#include "DissentTest.hpp"

using namespace Dissent::Connections;
using namespace Dissent::Anonymity;

namespace Dissent {
namespace Tests {
TEST(VersionGraph, Basic)
{
    DisableLogging();

    Id                          id[10];
    QVector<Id>                 group_vector;
    QVector<AsymmetricKey *>    key_vector;

    QVector<Id>                 group_vector2;
    QVector<AsymmetricKey *>    key_vector2;

    QMap<QString, QVariant>     gpolicy;

    gpolicy.insert("Quorum", 10);

    for(int idx = 0; idx < 10; idx++) {
      AsymmetricKey *key0 = new CppPrivateKey();
      group_vector.append(id[idx]);
      key_vector.append(key0->GetPublicKey());

      if(idx > 3){
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
    //vg2.addNew(vn7);


    vg2.save("GraphFile");

    VersionGraph vg = VersionGraph("GraphFile");

    QVector<QByteArray> heads;

    vg.getHeads(heads, vn.getHash());


    Group       intersection(QVector<Id>(0),QVector<AsymmetricKey *>(0));
    Group       symmetric_diff(QVector<Id>(0),QVector<AsymmetricKey *>(0));

    vg.getHeadsIaSD(intersection, symmetric_diff, vn.getHash());

    std::cout << "Headcount : " << heads.count() << std::endl;
    std::cout << "Intersecion Size : " << intersection.GetSize() << std::endl;
    std::cout << "Symmetric Difference Size : " << symmetric_diff.GetSize() << std::endl;

    QByteArray w;
    QDataStream strm3(&w, QIODevice::WriteOnly);
    strm3 << vg;
    QDataStream strm4(&w, QIODevice::ReadOnly);
    strm4 >> vg;

    EnableLogging();
}

}
}
