// Copyright 2017 Stefano Pellegrini

#include "agent.h"

#include "utils.h"

Agent::Agent(const Battery* battery, const int* time) :
  battery_(battery), time_(time) {}

Direction Agent::NextDirection(const Perception& p) {
  constexpr int strategy = 0;
  Direction direction;
  if (strategy == 0) {
    direction = randomStrategy(p);
  } else if (strategy == 1) {
	direction = followWallsStrategy(p);
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

bool Agent::returnHomeRequired(const Perception& p, Direction* direction) {
  return false;
}

bool Agent::IsAlive() const {
  return battery_->Level() > 0;
}

Direction Agent::followWallsStrategy(const Perception& p) {
  if (p.HasObstacle(Direction::UP)) {
	// TODO: react to obstacle
  }
  if (p.HasObstacle(Direction::LEFT)) {
	// TODO: react to obstacle
  }
  if (p.HasObstacle(Direction::DOWN)) {
	// TODO: react to obstacle
  }
  if (p.HasObstacle(Direction::RIGHT)) {
	// TODO: react to obstacle
  }
  return Direction::NONE;
}
