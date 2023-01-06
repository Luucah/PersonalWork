#ifndef __Vec2_
#define __Vec2_

#include "Vec2.h"

#include <math.h>

#include <cassert>
#include <stdexcept>

Vec2::Vec2(const std::vector<float> src) {  // effectively fromlist()
  assert(src.size() >= 2 && "Source list size must be greater than 2!\n");
  x = src[0];
  y = src[1];
}

Vec2::Vec2(const Vec2* src) {
  x = src->x;
  y = src->y;
}

float& Vec2::operator[](unsigned int n) {
  if (n == 0) {
    return x;
  } else if (n == 1) {
    return y;
  } else {
    throw std::out_of_range("Vec2: Index must be less than 2!");
  }
}

Vec2& Vec2::operator=(const Vec2& rhs) {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

Vec2* Vec2::div(const Vec2* other) {
  if (other->x == 0 || other->y == 0) {
    throw std::runtime_error("Vec2: Divide by zero");
  }
  x /= other->x;
  y /= other->y;
  return this;
}
Vec2* Vec2::div(float num) {
  if (num == 0) {
    throw std::runtime_error("Vec2: Divide by zero");
  }
  x /= num;
  y /= num;
  return this;
}

Vec2* Vec2::mult(const Vec2* other) {
  x *= other->x;
  y *= other->y;
  return this;
}
Vec2* Vec2::mult(float num) {
  x *= num;
  y *= num;
  return this;
}

Vec2* Vec2::sub(const Vec2* other) {
  x -= other->x;
  y -= other->y;
  return this;
}
Vec2* Vec2::sub(float num) {
  x -= num;
  y -= num;
  return this;
}

Vec2* Vec2::add(const Vec2* other) {
  x += other->x;
  y += other->y;
  return this;
}
Vec2* Vec2::add(float num) {
  x += num;
  y += num;
  return this;
}

Vec2* Vec2::norm(bool inplace = false) {
  float sz = this->length();
  if (inplace) {
    x /= sz;
    y /= sz;
    return this;
  } else {  // this might be wrong, my bad if so
    Vec2* ret_val = new Vec2(x / sz, y / sz);
    return ret_val;
  }
}

float Vec2::length() const { return sqrt(x * x + y * y); }

Vec2* Vec2::fromvec(const std::vector<float> src) {
  x = src[0];
  y = src[1];
  return this;
}

// wooooOOOOOOOOOOOOOOOOOO tests :||||||
int testVec() {
  // put a whole buncha tests here :)

  Vec2* vec1 = new Vec2(10, 12);
  assert(vec1->x == 10);
  assert(vec1->y == 12);
  Vec2* vec2 = new Vec2(5, 6);

  // Add test
  vec2->add(vec1);
  assert(vec2->x == 15);
  assert(vec2->y == 18);
  // Old is unchanged
  assert(vec1->x == 10);
  assert(vec1->y == 12);

  delete vec2;
  // Subtract test
  vec2 = new Vec2(5, 6);
  vec2->sub(vec1);
  assert(vec2->x == -5);
  assert(vec2->y == -6);
  // Old is unchanged
  assert(vec1->x == 10);
  assert(vec1->y == 12);

  delete vec2;
  // multiply test
  vec2 = new Vec2(5, 6);
  vec2->mult(vec1);
  assert(vec2->x == 50);
  assert(vec2->y == 72);
  // Old is unchanged
  assert(vec1->x == 10);
  assert(vec1->y == 12);

  delete vec2;
  // divide test
  vec2 = new Vec2(5, 6);
  vec2->div(vec1);  // vec2 / vec1
  assert(fabs(vec2->x - 1. / 2.) < 0.00001);
  assert(vec2->y - 1. / 2. < 0.00001);
  // Old is unchanged
  assert(vec1->x == 10);
  assert(vec1->y == 12);

  // Normalize Test
  delete vec2;
  vec2 = vec1->norm(false);
  assert(fabs(vec2->x - 0.64018439966447986837128874883271) < 0.00001);
  assert(fabs(vec2->y - 0.76822127959737584204554649859926) < 0.00001);
  // Old is unchanged
  assert(vec1->x == 10);
  assert(vec1->y == 12);
  delete vec2;
  vec2 = vec1->norm(true);
  assert(fabs(vec1->x - 0.64018439966447986837128874883271) < 0.000001);
  assert(fabs(vec1->y - 0.76822127959737584204554649859926) < 0.000001);
  assert((*vec1) == (*vec2));

  // From array test
  std::vector<float> tmp_vec = {47, 42};
  Vec2* vec3 = new Vec2(tmp_vec);
  assert(vec3->x == 47);
  assert(vec3->y == 42);
  tmp_vec[0] = 10;
  tmp_vec[1] = 12;
  vec3->fromvec(tmp_vec);
  assert(vec3->x == 10);
  assert(vec3->y == 12);

  // Indexing test
  assert((*vec3)[0] == 10);
  assert((*vec3)[1] == 12);

  printf("Vec2 passed all tests\n");
  return 0;
}

int main() { return testVec(); }
#endif