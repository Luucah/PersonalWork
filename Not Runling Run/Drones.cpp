#ifndef __Drones_
#define __Drones_

#include "Drones.h"

PatrolDrone::PatrolDrone() {
  position = Vec2(0., 0.);
  target_position = Vec2(1., 1.);
  speed = 100;
  color = (0, 255, 0);
}

PatrolDrone::PatrolDrone(const PatrolDrone* src) {}
PatrolDrone::PatrolDrone(const Vec2* init_pos,
                         std::vector<Vec2> init_target_list) {}
void PatrolDrone::addTarget(const Vec2* targ_pos) {}
void PatrolDrone::cycleTargetPosition() {}
void PatrolDrone::update(float dt) {}

#endif