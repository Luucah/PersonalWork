#include <tuple>
#include <vector>

#include "Vec2.h"

class Drone {
 protected:
  Vec2 position, target_position;
  int speed;
  std::tuple<int> color = (0, 0, 0);
};

class PatrolDrone : protected Drone {
 protected:
  int target_num;
  std::vector<Vec2> target_list;

 private:
  PatrolDrone();
  PatrolDrone(const PatrolDrone* src);
  PatrolDrone(const Vec2* init_pos, std::vector<Vec2> init_target_list);
  void addTarget(const Vec2* targ_pos);
  void cycleTargetPosition();
  void update(float dt);
};

class RandomDrone : protected Drone {
 private:
  RandomDrone();
  RandomDrone(const RandomDrone* src);
  RandomDrone(const Vec2* init_pos, const Vec2* init_target_pos);
  void update(float dt);
};

class BounceDrone : protected Drone {
 protected:
  Vec2 heading;

 private:
  BounceDrone();
  BounceDrone(const BounceDrone* stc);
  BounceDrone(const Vec2* init_pos, const Vec2* init_heading);
  void update(float dt);
};
