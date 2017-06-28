#ifndef AGENT_H_
#define AGENT_H_

#include "battery.h"
#include "geometry.h"
#include "map.h"
#include "perception.h"

// Models a vacuum cleaner.
class Agent {
 public:
  // Constructs an agent with a |battery| and a |time| that are read
  // only. Both arguments should outlive the agent.
  Agent(const Battery* battery, const int* time);

  // Get the next direction of the agent based on the current perception |p|.
  Direction NextDirection(const Perception& p);
  
  Direction randomStrategy(const Perception& p);
  
  // Returns true if it's required to return home, provides necessary direction
  // to direction.
  bool returnHomeRequired(const Perception& p, Direction* direction);

  // Whether the agent has still some battery left.
  bool IsAlive() const;

  Direction followWallsStrategy(const Perception& p);

 private:
  // Not owned by the agent.
  const Battery* battery_;
  // Not owned by the agent.
  const int* time_;
};

#endif  // AGENT_H_
