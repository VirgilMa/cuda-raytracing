#include "float.h"
#include "hitable_list.hpp"
#include "sphere.hpp"
#include <iostream>

vec3 color(const ray &r, hitable *world) {
  hit_record rec;
  if (world->hit(r, 0.0, MAXFLOAT, rec))
    return 0.5 * (rec.normal + vec3(1, 1, 1));

  vec3 unit_direction = unit_vector(r.direction());
  float t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
  int nx = 200;
  int ny = 100;
  std::cout << "P3\n" << nx << " " << ny << "\n255\n";

  vec3 lower_left_corner(-2.0, -1.0, -1.0);
  vec3 horizontal(4.0, 0.0, 0.0);
  vec3 vertical(0.0, 2.0, 0.0);
  vec3 origin(0.0, 0.0, 0.0);

  hitable *list[2];
  list[0] = new sphere(vec3(0, 0, -1), 0.5);
  list[1] = new sphere(vec3(0, -100.5, -1), 100);
  hitable *world = new hitable_list(list, 2);

  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      float u = float(i) / float(nx);
      float v = float(j) / float(ny);
      ray r(origin, lower_left_corner + u * horizontal + v * vertical);
      vec3 icol = 255 * color(r, world);
      std::cout << int(icol.r()) << " " << int(icol.g()) << " " << int(icol.b())
                << "\n";
    }
  }

  return 0;
}
