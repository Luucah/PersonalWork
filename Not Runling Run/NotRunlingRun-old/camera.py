
from vector2 import Vector2

class Camera:
    def __init__(self, windowSize: Vector2):
        self.centerOffset = windowSize.copy().div(2)
        self.position = Vector2()

    def centerOn(self, center: Vector2):
        self.position = center.copy().sub(self.centerOffset)

    # Linearly move the camera
    def translate(self, offset: Vector2):
        self.position.add(offset)
    
    # Given a vector, transform that vector from world space to screen space
    def transform(self, toTranslate: Vector2) -> Vector2:
        return toTranslate.copy().sub(self.position)
    
    # Given a vector, transform that vector from screen space to world space
    def reverseTransform(self, toTranslate: Vector2) -> Vector2:
        return toTranslate.copy().add(self.position)
