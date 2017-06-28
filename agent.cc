// Copyright 2017 Stefano Pellegrini

#include "agent.h"

#include "utils.h"

#include <iostream>

Agent::Agent(const Battery* battery, const int* time) :
  battery_(battery), time_(time) {
    last_charging_signal_ = -1;  // uninitialized
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

Direction Agent::randomObstacleFreeDirection(const Perception& p) {
    Direction direction;
    bool found_obstacle_free_direction = false;
    do {
        direction = randomStrategy(p);
        if (!p.HasObstacle(direction)) {
            found_obstacle_free_direction = true;
        }
    } while (!found_obstacle_free_direction);

    return direction;
}

bool Agent::returnHomeRequired(const Perception& p, Direction* direction) {
    constexpr int min_battery_level = 50;
    if (battery_->Level() < min_battery_level) {
      std::cout << "Heading home!" << std::endl;

      // Simple homing strategy:
      //   * If the distance to the station has decreased
      //     since the last step, go to a new obstacle free direction.
      //   * If the distance has increased, just step back.
      //
      // If this is the first step, just go to a new obstacle free direction.

      const float current_charging_signal = p.GetChargingSignal();
      if (current_charging_signal > last_charging_signal_
              && last_charging_signal_ >= 0) {
          *direction = getOppositeDirection(last_direction_);
      } else {
          Direction possible_direction = randomObstacleFreeDirection(p);
          *direction = possible_direction;
          last_direction_ = possible_direction;
      }

      last_charging_signal_ = current_charging_signal;

      return true;
  } else {
      return false;
  }
}

bool Agent::IsAlive() const {
  return battery_->Level() > 0;
}
