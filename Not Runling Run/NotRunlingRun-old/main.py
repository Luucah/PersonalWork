
import pygame
from math import sqrt
import random

from vector2 import Vector2
from doodad import Doodad
from camera import Camera

random.seed()


def distance(vector):
    assert (len(vector) == 2)
    return sqrt(vector[0] * vector[0] + vector[1] * vector[1])
    # total = 0
    # for v in vector:
    #     total += v * v
    # return sqrt(total)


pygame.init()

size = Vector2(800, 600)
FPS = 60
screen = pygame.display.set_mode(size.astuple())
pygame.display.set_caption("Shit Quest")
clock = pygame.time.Clock()

# font = pygame.font.Font(pygame.font.get_default_font(), 20)
font = pygame.font.Font(".\RobotoMono.ttf", 20)

ROAD_TOP = 100
ROAD_BOTTOM = size[1] - ROAD_TOP
PLAYER_RADIUS = 10
DRONE_RADIUS = 15
ACCELERATION = 0.1


class Player:
    position = Vector2(size[0]/2, size[1]/2)
    max_speed = 200
    max_energy = 200

    time_between_frames = .200
    frames = 4

    still = True
    facingLeft = False
    using_ability = False

    def __init__(self):
        self.target_position = self.position.copy()
        self.living = True
        self.current_speed = self.max_speed/10

        self.current_energy = 50
        self.energy_tick = 0

        self.spritesheet = pygame.image.load("assets/beauty.png")
        self.left_spritesheet = pygame.transform.flip(
            self.spritesheet, True, False)
        self.timeleft = self.time_between_frames
        self.current_frame = 0
        self.setImage()

    def setImage(self):
        if self.facingLeft:
            self.left_spritesheet.set_clip(pygame.Rect(
                32*(self.frames-1-self.current_frame), 0, 32, 32))
            self.image = self.left_spritesheet.get_clip()
        else:
            self.spritesheet.set_clip(pygame.Rect(
                32*self.current_frame, 0, 32, 32))
            self.image = self.spritesheet.get_clip()

    def setTargetPosition(self, target: Vector2):
        self.target_position = Vector2(
            target[0],
            max(ROAD_TOP, min(target[1], ROAD_BOTTOM))
        )
        self.still = False
        facingLeft = self.target_position.x < self.position.x
        if facingLeft != self.facingLeft:
            self.facingLeft = facingLeft
            self.setImage()

    def update(self, dt: float):
        # Update the animation timer
        if not self.still:
            self.timeleft -= dt
            if self.timeleft < 0:
                self.timeleft += self.time_between_frames
                self.current_frame = (self.current_frame + 1) % self.frames
                self.setImage()
        else:
            if self.current_frame != 0:
                self.current_frame = 0
                self.setImage()

        # energy regen/drain
        if self.current_energy < self.max_energy and not self.using_ability:
            if self.energy_tick % 3 == 0:
                self.current_energy += 1
        if self.using_ability:
            self.speedDrain()
            if self.current_energy == 0:
                self.max_speed = 200
                self.using_ability = False

        self.energy_tick += 1

        # Linearly interpolate between target and current positions
        # for now.
        delta = self.target_position.copy().sub(self.position)
        delta_distance = delta.length()
        if delta_distance <= self.max_speed * dt:
            self.position = self.target_position.copy()
            self.still = True
            self.current_speed = self.max_speed/10
        else:
            self.position.add(delta.norm().mult(self.current_speed * dt))

            self.current_speed = min(
                self.current_speed + ACCELERATION*self.max_speed, self.max_speed)

    def draw(self, screen, camera):
        drawPos = self.position.copy().sub(Vector2(16, 16))
        drawPos = camera.transform(drawPos).astuple()
        if self.facingLeft:
            screen.blit(self.left_spritesheet, drawPos, self.image)
        else:
            screen.blit(self.spritesheet, drawPos, self.image)

    def blink(self):
        if self.current_energy < 50:
            return

        blink_length = self.max_speed/3
        delta = self.target_position.copy().sub(self.position)
        delta_distance = delta.length()
        if delta_distance <= blink_length:
            self.position = self.target_position.copy()
            self.still = True
            self.current_speed = self.max_speed/10
        else:
            self.position.add(delta.norm().mult(blink_length))
            self.current_speed = self.max_speed/10
        self.current_energy -= 50

    def speedDrain(self):
        if self.energy_tick % 15 == 0:
            self.current_energy -= 2

    def speed(self):
        if self.current_energy < 25:
            return
        self.using_ability = not self.using_ability
        if self.using_ability:
            self.max_speed *= 2
        else:
            self.max_speed /= 2


class PatrolDrone:
    position = Vector2(size[0]/4, size[1]/4)
    speed = 100
    color = (0, 255, 0)

    def __init__(self):
        self.target_number = 0

        self.target_list = [self.position]
        self.target_list.append(Vector2(3*size[0]/4, self.position.y))
        self.target_list.append(Vector2(size[0]/2, 3*size[1]/4))

        self.target_position = self.target_list[1]

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


camera = Camera(size)
player = Player()
# drone = PatrolDrone()
active_drones = []

# Make some doodads to fill out the space
doodads = []
for i in range(20):
    x = random.random() * size.x
    y = random.random() * (ROAD_BOTTOM - ROAD_TOP) + ROAD_TOP
    d = Doodad(Vector2(x, y))
    doodads.append(d)

# active_drones.append(BounceDrone())
# active_drones.append(RandomDrone())
active_drones.append(PatrolDrone())
doLoop = True

while doLoop and player.living:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            doLoop = False
            player.living = False
        if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
            doLoop = False
            player.living = False

        # ability use
        if event.type == pygame.KEYDOWN and event.key == pygame.K_b:
            player.blink()
        if event.type == pygame.KEYDOWN and event.key == pygame.K_s:
            player.speed()

        # If the player has clicked, we should set that as the player's
        # new destination
        if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
            # Set the player's position to the mouse position
            player.setTargetPosition(
                camera.reverseTransform(Vector2(event.pos)))

    # Update the world
    player.update(1. / FPS)
    for enemy in active_drones:
        enemy.update(1. / FPS)

    # Do drawing stuff
    screen.fill((255, 255, 255))
    GREY = 177

    road_corner = camera.transform(Vector2(0, ROAD_TOP - PLAYER_RADIUS - 2))
    road_rect = (road_corner[0], road_corner[1], size[0],
                 ROAD_BOTTOM - ROAD_TOP + 2 * PLAYER_RADIUS + 4)
    pygame.draw.rect(screen, (GREY, GREY, GREY), road_rect)

    # Center the camera on the player
    camera.centerOn(player.position)

    # Draw the doodads
    for d in doodads:
        d.draw(screen, camera)

    # Draw where the player would like to be
    pygame.draw.circle(screen, (0, 0, 255), camera.transform(
        player.target_position).astuple(), 5, 0)
    # Draw a red circle centered at the player with radius 10 and line
    # width 0 (meaning filled circle)
    # pygame.draw.circle(screen, (255,0,0), player.position.astuple(), PLAYER_RADIUS, 0)
    player.draw(screen, camera)

    # draw patrol drone
    for enemy in active_drones:
        pygame.draw.circle(screen, enemy.color, camera.transform(
            enemy.position).astuple(), DRONE_RADIUS, 0)

    # check collision
    for enemy in active_drones:
        delta = player.position.copy().sub(Vector2(enemy.position))
        if delta.length() < PLAYER_RADIUS + DRONE_RADIUS:
            player.living = False
            break

    # draw energy meter
    energy_display = font.render(
        f'ENERGY: {player.current_energy:>3}', True, (255, 0, 255))
    screen.blit(energy_display, (size[0]-135, 0))

    # draw controls
    control_text = font.render(
        "Move: LMB   Blink: B   Speed(toggle): S", True, (0, 0, 0))
    screen.blit(control_text, (0, size[1]-25))
    # Draw some debug information
    # delta = (
    #     player.target_position[0] - player.position[0],
    #     player.target_position[1] - player.position[1]
    # )
    # delta_distance = distance(delta)
    # text = font.render(f'{delta_distance:.2f} {player.position[0]:.2f} {player.position[1]:.2f} {player.speed * (1. / FPS)}',
    #     True, (0,0,0))
    # screen.blit(text, (0,0))

    pygame.display.flip()

    clock.tick(FPS)

while doLoop and not player.living:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            doLoop = False
        if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
            doLoop = False

    # Do drawing stuff
    screen.fill((255, 255, 255))
    game_over = font.render("get fucked idiot", True, (0, 0, 0))
    screen.blit(game_over, (size[0]/3, size[1]/2))

    pygame.display.flip()

    clock.tick(FPS)

print("closing")
