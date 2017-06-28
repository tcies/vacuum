#ifndef AGENT_H_
#define AGENT_H_

#include <stack>
#include <map>
#include <vector>

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
  
  Direction frontierBasedExploration(const Perception& p);
  
  void processPerception(const Perception& p);

  // Whether the agent has still some battery left.
  bool IsAlive() const;

 private:
  // Not owned by the agent.
  const Battery* battery_;
  // Not owned by the agent.
  const int* time_;
  
  struct Coordinate {
    int x;
    int y;
    Coordinate operator +(const Coordinate& other) const;
    bool operator <(const Coordinate& other) const;
  };
  Coordinate current_location_;
  std::map<Coordinate, bool> occupied_;
  // Using a stack allows very effective processing of frontiers:
  // Most recently added frontiers will be processed first.
  std::stack<Coordinate> frontiers_;
  // Used once a goal frontier is set, to navigate towards the frontier.
  std::map<Coordinate, int> distance_from_goal_;
  
  struct Option {
    Direction name;
    Coordinate value;
  };
  std::vector<Option> options_;
};

#endif  // AGENT_H_
