#include "TestNode.hpp"

using namespace Dissent::Utils;

namespace Dissent {
namespace Tests {
  int TestNode::calledback;
  int TestNode::success;
  int TestNode::failure;

  void ConstructOverlay(int count, QVector<TestNode *> &nodes,
      Group *&group, bool make_keys)
  {
    QVector<Id> ids;
    QVector<AsymmetricKey *> signing_keys;

    for(int idx = 0; idx < count; idx++) {
      nodes.append(new TestNode(idx+1, make_keys));
      ids.append(nodes[idx]->cm.GetId());
      if(make_keys) {
        signing_keys.append(nodes[idx]->key->GetPublicKey());
      }
    }

    group = new Group(ids, signing_keys);

    for(int idx = 0; idx < count; idx++) {
      for(int jdx = 0; jdx < count; jdx++) {
        if(idx == jdx) {
          continue;
        }
        nodes[idx]->cm.ConnectTo(BufferAddress(jdx+1));
      }
    }

    qint64 next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }
  }

  void CreateSession(const QVector<TestNode *> &nodes,
      const Group &group, const Id &leader_id, const Id &session_id,
      CreateSessionCallback callback)
  {
    for(int idx = 0; idx < nodes.count(); idx++) {
      Session *session = callback(nodes[idx], group, leader_id, session_id);
      nodes[idx]->session = session;
      session->SetSink(&(nodes[idx]->sink));
      nodes[idx]->sm.AddSession(session);
      QObject::connect(session, SIGNAL(RoundFinished(Session *, Round *)),
          nodes[idx], SLOT(HandleRoundFinished(Session *, Round *)));
    }
  }

  void CleanUp(const QVector<TestNode *> &nodes)
  {
    for(int idx = 0; idx < nodes.count(); idx++) {
      if(nodes[idx]->session) {
        nodes[idx]->session->Stop();
      }
      nodes[idx]->cm.Disconnect();
    }

    qint64 next = Timer::GetInstance().VirtualRun();
    while(next != -1) {
      Time::GetInstance().IncrementVirtualClock(next);
      next = Timer::GetInstance().VirtualRun();
    }

    for(int idx = 0; idx < nodes.count(); idx++) {
      delete nodes[idx];
    }
  }
}
}
