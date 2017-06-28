// Copyright 2017 Stefano Pellegrini

#include <limits>

#include "agent.h"

#include "utils.h"

Agent::Agent(const Battery* battery, const int* time) :
  battery_(battery), time_(time), current_location_({0, 0}) {
  options_.push_back({Direction::NONE, {0,0}});
  options_.push_back({Direction::LEFT, {0,-1}});
  options_.push_back({Direction::RIGHT, {0,1}});
  options_.push_back({Direction::UP, {-1,0}});
  options_.push_back({Direction::DOWN, {1,0}});
}

Direction Agent::NextDirection(const Perception& p) {
  constexpr int strategy = 0;
  Direction direction;
  if (strategy == 0) {
    direction = randomStrategy(p);
  } else if (strategy == 1) {
    
  }
  
  Direction return_direction;
  if (returnHomeRequired(p, &return_direction)) {
    direction = return_direction;
  }
  
  for (const Option& option : options_) {
    if (option.name == direction) {
      current_location_ = current_location_ + option.value;
    }
  }
  
  return direction;
}

Direction Agent::randomStrategy(const Perception& p) {
  const float random = Random();
  if (random < 0.2) {
    return Direction::NONE;
  }
  if (random < 0.4) {
    return Direction::LEFT;
  }
  if (random < 0.6) {
    return Direction::UP;
  }
  if (random < 0.8) {
    return Direction::DOWN;
  }
  return Direction::RIGHT;
}

bool Agent::returnHomeRequired(const Perception& p, Direction* direction) {
  return false;
}

Direction Agent::frontierBasedExploration(const Perception& p) {
  // Process perception.
  processPerception(p);
  // If we have a target assigned, go to it.
  if (!distance_from_goal_.empty()) {
    if (distance_from_goal_[current_location_] == 0) {
      distance_from_goal_.clear();
    } else {
      int lowest_distance = std::numeric_limits<int>::max();
      Direction best = Direction::NONE;
      for (const Option& option : options_) {
        const int option_dist = distance_from_goal_[current_location_ + option.value];
        if (option_dist < lowest_distance) {
          lowest_distance = option_dist;
          best = option.name;
        }
      }
      return best;
    }
  }
}

void Agent::processPerception(const Perception& p) {

}

bool Agent::IsAlive() const {
  return battery_->Level() > 0;
}

Agent::Coordinate Agent::Coordinate::operator +(const Coordinate& other) const {
  return {x + other.x, y + other.y};
}

bool Agent::Coordinate::operator <(const Coordinate& other) const {
  if (x == other.x) {
    return y < other.y;
  }
  return x < other.x;
}
