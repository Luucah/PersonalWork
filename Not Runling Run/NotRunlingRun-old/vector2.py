
import math
import copy
from typing import Union, TypeVar

TVector2 = TypeVar("TVector2", bound="Vector2")


class Vector2:
    def __init__(self, x: Union[int, float, tuple, TVector2] = 0, y: float = 0) -> TVector2:
        if not (type(x) == int or type(x) == float):
            if not y == 0:
                print(
                    f"[WARN] y argument assigned during Vector2 construction of single value. x:{x}, y:{y}")
            self.x = x[0]
            self.y = x[1]
        else:
            self.x = x
            self.y = y

    def __getitem__(self, item: int) -> float:
        if item == 0:
            return self.x
        elif item == 1:
            return self.y
        else:
            raise IndexError(
                f'Key {item} invalid for object of type "Vector2"')

    def div(self, other: any) -> TVector2:
        if type(other) == Vector2:
            self.x /= other.x
            self.y /= other.y
        else:
            self.x /= other
            self.y /= other
        return self

    def mult(self, other: any) -> TVector2:
        if type(other) == Vector2:
            self.x *= other.x
            self.y *= other.y
        else:
            self.x *= other
            self.y *= other
        return self

    def sub(self, other: any) -> TVector2:
        if type(other) == Vector2:
            self.x -= other.x
            self.y -= other.y
        else:
            self.x -= other
            self.y -= other
        return self

    def add(self, other: any) -> TVector2:
        if type(other) == Vector2:
            self.x += other.x
            self.y += other.y
        else:
            self.x += other
            self.y += other
        return self

    def norm(self, inplace=False) -> TVector2:
        length = self.length()
        if inplace:
            self.x /= length
            self.y /= length
            return self
        else:
            return Vector2(self.x / length, self.y / length)

    def length(self) -> float:
        return math.sqrt(self.x * self.x + self.y * self.y)

    def astuple(self) -> tuple:
        return (self.x, self.y)

    def fromlist(self, source: Union[list, tuple]) -> TVector2:
        self.x = source[0]
        self.y = source[1]
        return self

    def copy(self) -> TVector2:
        return copy.deepcopy(self)


if __name__ == "__main__":
    # Run some tests

    vec1 = Vector2(10, 12)
    assert (vec1.x == 10)
    assert (vec1.y == 12)
    vec2 = Vector2(5, 6)

    # Add test
    vec2.add(vec1)
    assert (vec2.x == 15)
    assert (vec2.y == 18)
    # Old is unmodified
    assert (vec1.x == 10)
    assert (vec1.y == 12)

    # Subtract test
    vec2 = Vector2(5, 6)
    vec2.sub(vec1)
    assert (vec2.x == -5)
    assert (vec2.y == -6)
    # Old is unmodified
    assert (vec1.x == 10)
    assert (vec1.y == 12)

    # Multiply test
    vec2 = Vector2(5, 6)
    vec2.mult(vec1)
    assert (vec2.x == 50)
    assert (vec2.y == 72)
    # Old is unmodified
    assert (vec1.x == 10)
    assert (vec1.y == 12)

    # Divide test
    vec2 = Vector2(5, 6)
    vec2.div(vec1)
    assert (vec2.x == 1/2)
    assert (vec2.y == 1/2)
    # Old is unmodified
    assert (vec1.x == 10)
    assert (vec1.y == 12)

    # Normalize test
    vec2 = vec1.norm(inplace=False)
    # Check to ensure they are the same within some tolerance
    assert (vec2.x - 0.64018439966447986837128874883271 < 0.000001)
    assert (vec2.y - 0.76822127959737584204554649859926 < 0.000001)
    # Old is unmodified
    assert (vec1.x == 10)
    assert (vec1.y == 12)
    vec2 = vec1.norm(inplace=True)
    assert (vec1.x - 0.64018439966447986837128874883271 < 0.000001)
    assert (vec1.y - 0.76822127959737584204554649859926 < 0.000001)
    assert (vec1 == vec2)

    # From array test
    vals = (10, 12)
    vec3 = Vector2(vals)
    assert (vec3.x == 10)
    assert (vec3.y == 12)
    vec3.fromlist((47, 42))
    assert (vec3.x == 47)
    assert (vec3.y == 42)

    # Indexing test
    assert (vec3[0] == vec3.x)
    assert (vec3[1] == vec3.y)

    print("Vector2 passed all tests")
