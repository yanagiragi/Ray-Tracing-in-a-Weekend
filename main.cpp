#include <iostream>

#include "sphere.h"
#include "hitable_list.h"
#include "material.h"
#include "camera.h"

#ifndef MAXFLOAT
    #define MAXFLOAT 1e9
#endif

vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    const float eps = 0.001;
    if (world->hit(r, eps , MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return vec3(0.0, 0.0, 0.0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }    
}

int main(int argc, char**argv)
{
    int nx = 200;
    int ny = 100;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *list[4];
    list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)) );
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0))) ;
    list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8)));

    hitable* world = new hitable_list(list, 4);

    camera cam;

    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {

            vec3 col(0, 0, 0);

            for(int s = 0; s < ns; ++s) {

                // drand48() range: [0.0, 1.0)
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);

                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            
            // divide by sample counts
            col /= float(ns);
            
            // perform gamma correction, gamma = 2
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}