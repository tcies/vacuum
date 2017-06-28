#include "agent.h"

#include <cassert>

#include "battery.h"
#include "perception.h"
#include "tests.h"

TEST(Agent_IsAlive) {
  Battery battery(50);
  int time = 100;
  Agent agent(&battery, &time);
  assert(agent.IsAlive());

  for (int i = 0; i < 50; ++i) {
    battery.Use();
  }
  assert(!agent.IsAlive());
}

// This test check only that nothing crashes.
TEST(Agent_NextDirection) {
  Battery battery(50);
  int time = 100;
  Agent agent(&battery, &time);
  Perception p;
  agent.NextDirection(p);
}

TEST(Agent_returnHomeRequiredEnoughBattery) {
  Battery battery(51);
  int time = 100;
  Agent agent(&battery, &time);
  Perception p;
  Direction direction;
  bool go_home = agent.returnHomeRequired(p, &direction);
  assert(!go_home);
}

TEST(Agent_returnHomeRequiredNotEnoughBattery) {
  Battery battery(49);
  int time = 100;
  Agent agent(&battery, &time);
  Perception p;
  Direction direction;
  bool go_home = agent.returnHomeRequired(p, &direction);
  assert(go_home);
}



