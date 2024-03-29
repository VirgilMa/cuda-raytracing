#include "camera.hpp"
#include "float.h"
#include "hitable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include <fstream>
#include <iostream>
#include <random>

vec3 color(const ray &r, hitable *world, int depth) {
  hit_record rec;
  if (world->hit(r, 0.001, MAXFLOAT, rec)) {
    ray scattered;
    vec3 attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
      return attenuation * color(scattered, world, depth + 1);
    else
      return vec3(0, 0, 0);
  }
  vec3 unit_direction = unit_vector(r.direction());
  float t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

hitable *random_scene() {
  int n = 500;
  hitable **list = new hitable *[n + 1];
  list[0] =
      new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = drand48();
      vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
      if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
        material *mat_ptr;
        if (choose_mat < 0.8) {
          mat_ptr =
              new lambertian(vec3(drand48() * drand48(), drand48() * drand48(),
                                  drand48() * drand48()));
        } else if (choose_mat < 0.95) {
          mat_ptr = new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()),
                                   0.5 * (1 + drand48())),
                              0.5 * drand48());
        } else {
          mat_ptr = new dielectric(1.5);
        }
        list[i++] = new sphere(center, 0.2, mat_ptr);
      }
    }
  }

  list[i++] = new sphere(vec3(0, 1, -4), 1.0, new dielectric(1.5));
  list[i++] =
      new sphere(vec3(-4, 1, -4), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
  list[i++] =
      new sphere(vec3(4, 1, -4), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0));

  return new hitable_list(list, i);
}

void render(char *filename) {

  std::ofstream file;
  file.open(filename);

  int nx = 4096;
  int ny = 2160;
  int ns = 50;
  file << "P3\n" << nx << " " << ny << "\n255\n";

  // hitable *list[5];
  // list[0] =
  //     new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
  // list[1] =
  //     new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8,
  //     0.0)));
  // list[2] =
  //     new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
  // list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
  // list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
  // hitable *world = new hitable_list(list, 5);
  hitable *world = random_scene();

  vec3 lookfrom(2, 2, 1);
  vec3 lookat(0, 0, -1);
  float dist_to_focus = (lookfrom - lookat).length();
  float aperture = 1.0;
  camera cam(lookfrom, lookat, vec3(0, 1, 0), 90, float(nx) / float(ny),
             aperture, dist_to_focus);

  for (int j = ny - 1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      vec3 col(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        ray r = cam.get_ray(u, v);
        col += color(r, world, 0);
      }
      col /= float(ns);
      col = sqrt(col);
      vec3 icol = 255 * col;
      file << int(icol.r()) << " " << int(icol.g()) << " " << int(icol.b())
           << "\n";
    }
  }

  file.close();
}

int main(int argc, char *argv[]) {
  char *filename = (char *)"./out/p.ppm";
  if (argc == 2) {
    filename = argv[1];
  }

  time_t start, stop;
  start = time(NULL);

  render(filename);

  stop = time(NULL);
  printf("Use Time: %lds\n", (stop - start));

  return 0;
}
