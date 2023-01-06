
from vector2 import Vector2
from camera import Camera
from pygame.draw import circle

class Doodad():
    DOODAD_COLOR = (120,120,120)
    DOODAD_SIZE = 4

    def __init__(self, position: Vector2 = Vector2()):
        self.position = position

    def draw(self, screen, camera: Camera):
        draw_position = camera.transform(self.position)
        circle(screen, self.DOODAD_COLOR, draw_position.astuple(), self.DOODAD_SIZE, 0)
