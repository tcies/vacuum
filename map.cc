#include "map.h"

#include <cassert>
#include <iostream>
#include <unistd.h>

#include "utils.h"

using std::cout;
using std::endl;

Map::Map(int x_size, int y_size) :
  x_size_(x_size),
  y_size_(y_size),
  agent_position_(Point{0, 0}),
  dirt_map_(Matrix(x_size, y_size)),
  obstacles_map_(Matrix(x_size, y_size)),
  initial_dirt_(0) {
  bool agent_set = false;
  for (int x = 0; x < x_size_; ++x) {
    for (int y = 0; y < y_size_; ++y) {
      if (x * y == 0 || x == x_size_ - 1  || y == y_size_ - 1) {
        // Add the borders of the map.
        obstacles_map_.Set(x, y, 1);
        continue;
      }
      if (!agent_set) {
        agent_position_ = Point{x, y};
        charging_station_ = Point{x, y};
        agent_set = true;
      }
      obstacles_map_.Set(x, y, 0);
    }
  }
}

void Map::SetObstacle(int x, int y) {
  obstacles_map_.Set(x, y, 1);
}

void Map::SetDirt(int x, int y) {
  dirt_map_.Set(x, y, 1);
}

void Map::SetDirtCount() {
  initial_dirt_ = dirt_map_.CountNonZero();
}
bool Map::Update(const Direction agent_action) {
  dirt_map_.Set(agent_position_.x, agent_position_.y, 0);

  Point move{0, 0};
  switch (agent_action) {
  case Direction::UP:
    move.x = -1;
    break;
  case Direction::DOWN:
    move.x = 1;
    break;
  case Direction::LEFT:
    move.y = -1;
    break;
  case Direction::RIGHT:
    move.y = 1;
    break;
  case Direction::NONE:
    break;
  }
  const Point new_agent_position = agent_position_ + move;
  if (obstacles_map_.Get(new_agent_position.x,
                         new_agent_position.y) == 0) {
    agent_position_ = new_agent_position;
    return true;
  }
  return false;
}

Perception Map::GetAgentPerception() const {
  Perception p;
  p.SetChargingSignal(charging_station_.Distance(agent_position_) +
                      Random() - 0.5);
  for (int x_offset = - 1; x_offset <= 1; ++x_offset) {
    for (int y_offset = - 1; y_offset <= 1; ++y_offset) {
      if (obstacles_map_.Get(agent_position_.x + x_offset,
                             agent_position_.y + y_offset)) {
        p.SetObstacle(OffsetToDirection(Point{x_offset + 1, y_offset + 1}));
      }
    }
  }
  return p;
}

bool Map::AgentAtHome() const {
  return agent_position_ == charging_station_;
}

float Map::CleanedRatio() const {
  if (initial_dirt_ == 0) {
    return 0.0f;
  }

  const int current_dirt = dirt_map_.CountNonZero();
  assert(current_dirt <= initial_dirt_);
  return 1 - (static_cast<float>(current_dirt) /
              static_cast<float>(initial_dirt_));
}


/* static */
Map Map::TwoRoomsMap(const int x_size, const int y_size) {
  assert(x_size > 1);
  assert(y_size > 1);
  Map map = Map::RandomMap(x_size, y_size);
  bool opening = false;
  bool need_to_reposition_agent = false;
  int y = y_size / 2;
  for (int x = 1; x < x_size - 1; ++x) {
    if (opening || Random() > static_cast<float>(x) / (x_size - 1)) {
      map.obstacles_map_.Set(x, y, 1);
       map.dirt_map_.Set(x, y, 0);
      if (map.agent_position_ == Point{x, y}) {
        need_to_reposition_agent = true;
      }
    } else {
      // Leave the door open.
      map.obstacles_map_.Set(x, y, 0);
      opening = true;
    }
  }

  if (need_to_reposition_agent) {
    assert(map.InitAgentAndChargingStation());
  }

  map.SetDirtCount();
  return map;
}

/* static */
Map Map::RandomMap(const int x_size, const int y_size) {
  Map map(x_size, y_size);
  // The borders already have obstacles.
  for (int x = 1; x < x_size - 1; ++x) {
    for (int y = 1; y < y_size - 1; ++y) {
      // Add some random obstacles in the map.
      if (Random() < 0.05) {
        map.obstacles_map_.Set(x, y, 1);
      }

      // Add some dirt.
      if (Random() > 0.85) {
        // Dirt things tend to stick to each other.
        for (int i = -1; i < 2; ++i) {
          for (int j = -1; j < 2; ++j) {
            if (x + i <= 0 ||
                y + j <= 0 ||
                x + i >= x_size - 1 ||
                y + j >= y_size - 1) {
              // We're at the boundary, nothing to do.
              continue;
            }
            if (Random() > 0.5) {
              map.dirt_map_.Set(x + i, y + i, 1);
            }
          }
        }
      }
    }
  }

  map.SetDirtCount();
  assert(map.InitAgentAndChargingStation());
  return map;
}

bool Map::InitAgentAndChargingStation() {
  for (int x = 0; x < x_size_; ++x) {
    for (int y = 0; y < y_size_; ++y) {
      if (obstacles_map_.Get(x, y) == 0) {
        agent_position_ = Point{x, y};
        charging_station_ = Point{x, y};
        return true;
      }
    }
  }

  // This will happen when the method is called on a map full of obstacles.
  return false;
}

void Map::Print() const {
  // Sleep a bit so we see what happens.
  usleep(100000);
  
  for (int x = 0; x < x_size_; ++x) {
    for (int y = 0; y < y_size_; ++y) {
      if (obstacles_map_.Get(x, y) > 0) {
        cout << "#";
      } else if (agent_position_.x == x && agent_position_.y == y) {
        cout << "A";
      } else if (dirt_map_.Get(x, y) > 0) {
        cout << "'";
      } else if (charging_station_.x == x && charging_station_.y == y) {
        cout << "S";
      } else {
        cout << " ";
      }
    }
    cout << endl;
  }
}

const Point& Map::AgentPosition() const {
  return agent_position_;
}
