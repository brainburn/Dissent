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

    for(int idx = 0; idx < 10; idx++) {
      AsymmetricKey *key0 = new CppPrivateKey();
      group_vector.append(id[idx]);
      key_vector.append(key0->GetPublicKey());
    }

    Group       group(group_vector,key_vector);
    QByteArray  qb;
    QDataStream strm(&qb, QIODevice::ReadWrite);

    strm << group;

    VersionNode vn(qb);

    VersionGraph vg2 = VersionGraph(vn);

    vg2.save("GraphFile");

    VersionGraph vg = VersionGraph("GraphFile");

    QByteArray w;
    QDataStream strm3(&w, QIODevice::WriteOnly);
    strm3 << vg;
    QDataStream strm4(&w, QIODevice::ReadOnly);
    strm4 >> vg;

    EnableLogging();
}

}
}
