from vector2 import Vector2
import copy


class PatrolDrone:
    position = Vector2()
    speed = 100
    color = (0, 255, 0)

    # targets is a list of Vec2 objects

    def __init__(self, targets):
        self.target_number = 0

        self.target_list = copy.deepcopy(targets)

        self.target_position.x = self.target_list[0].x
        self.target_position.y = self.target_list[0].y

    def cycleTargetPosition(self):
        if self.target_number == len(self.target_list) - 1:
            self.target_number = 0
        else:
            self.target_number += 1

        self.target_position = self.target_list[self.target_number]

    def update(self, dt: float):
        # identical to player movement atm
        # only change is that target position is updated in here
        delta = self.target_position.copy().sub(self.position)
        if delta.length() <= self.speed * dt:
            # when the drone reaches the destination,
            # move to the next position in the sequence
            self.position = self.target_position.copy()
            self.cycleTargetPosition()
        else:
            self.position.add(delta.norm().mult(self.speed * dt))


class RandomDrone:
    position = Vector2(size[0]/4, size[1]/4)
    speed = 100
    color = (0, 255, 255)

    def __init__(self):
        self.target_position = Vector2(
            max(ROAD_TOP+DRONE_RADIUS, min(random.random()
                * size[0], ROAD_BOTTOM-DRONE_RADIUS)),
            max(ROAD_TOP+DRONE_RADIUS, min(random.random()
                * size[1], ROAD_BOTTOM-DRONE_RADIUS)),
        )

    def update(self, dt: float):
        delta = self.target_position.copy().sub(self.position)
        if delta.length() <= self.speed * dt:
            self.position = self.target_position.copy()

            self.target_position = Vector2(
                max(ROAD_TOP+DRONE_RADIUS, min(random.random()
                    * size[0], ROAD_BOTTOM-DRONE_RADIUS)),
                max(ROAD_TOP+DRONE_RADIUS, min(random.random()
                    * size[1], ROAD_BOTTOM-DRONE_RADIUS)),
            )
        else:
            self.position.add(delta.norm().mult(self.speed * dt))


class BounceDrone:
    position = Vector2(3 * (size[0]/4), 3 * (size[1]/4))
    speed = 25
    color = (255, 0, 255)

    def __init__(self):

        init_x = random.uniform(0, self.speed/10)
        self.heading = Vector2(init_x, sqrt(self.speed - (init_x**2)))

    def update(self, dt: float):
        self.position.add(self.heading)

        # checking wall collision in here, so i know if i need to change heading
        if self.position.x < DRONE_RADIUS or self.position.x > size.x - DRONE_RADIUS:
            self.heading.x *= -1
        if self.position.y < ROAD_TOP + DRONE_RADIUS or self.position.y > ROAD_BOTTOM - DRONE_RADIUS:
            self.heading.y *= -1
