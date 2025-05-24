#ifndef MATERIAL_H
#define MATERIAL_H

#include "it_hit.h"
#include "ray.h"
#include "vec3.h"
#include "rtweekend.h"

using std::shared_ptr;

class material {
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
    virtual color emitted(double u, double v, const point3& p) const {
        return color(0, 0, 0);
    }
    virtual ~material() = default ;

};

class lambertian : public material {
public:
    lambertian(const color& base) : base(base) {}
    
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        vec3 scatter_direction = rec.normal + random_unit_vector();
        

        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        
        scattered = ray(rec.p, unit_vector(scatter_direction), r_in.time());
        attenuation = base;
        return true;
    }

private:
    color base;
};

class metal : public material {
public:
    metal(const color& base, double fuzz) : base(base), fuzz(fuzz < 1 ? fuzz : 1) {}
    
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(), r_in.time());
        attenuation = base;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color base;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}
    
    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        attenuation = color(1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;
        
        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        
        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;
        
        if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, ri);
        }
        
        scattered = ray(rec.p, direction, r_in.time());
        return true;
    }

private:

    double refraction_index;
    
    static double reflectance(double cosine, double refraction_index) {

        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }
};

#endif