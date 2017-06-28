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

  static Direction getOppositeDirection(const Direction& direction) {
      switch (direction) {
          case Direction::LEFT:
          default:
              return Direction::RIGHT;
          break;
          case Direction::RIGHT:
              return Direction::LEFT;
          break;
          case Direction::UP:
              return Direction::DOWN;
          break;
          case Direction::DOWN:
              return Direction::UP;
          break;
      }
  }

  // Finds a random direction that is obstacle free.
  Direction randomObstacleFreeDirection(const Perception& p);

  // Returns true if it's required to return home, provides necessary direction
  // to go back to the charging station.
  bool returnHomeRequired(const Perception& p, Direction* direction);

  // Whether the agent has still some battery left.
  bool IsAlive() const;

 private:
  // Not owned by the agent.
  const Battery* battery_;
  // Not owned by the agent.
  const int* time_;

  // Variables needed by returnHomeRequired()
  Direction last_direction_;
  float last_charging_signal_;
};

#endif  // AGENT_H_
