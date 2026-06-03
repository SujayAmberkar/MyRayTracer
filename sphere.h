// Sphere primitive: a concrete hittable that solves the ray-sphere intersection analytically.
#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "hittable.h"

class sphere : public hittable {
public:
    // fmax clamps negative radii to 0, preventing an inside-out sphere from silently corrupting normals.
    sphere(const point3& center, const double radius) : center(center), radius(std::fmax(0, radius)) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin();

        // Coefficients of the quadratic (ray-sphere intersection equation).
        // Using half-b (h) halves all terms, eliminating the factor-of-2 that appears
        // in the standard form and keeping the discriminant formula h*h - a*c.
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = h * h - a * c;

        if (discriminant < 0) // no real roots → ray misses the sphere
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Try the nearer root first; fall back to the far root if the near one is outside [tmin, tmax].
        // Why: we always want the closest valid hit, and a single sphere can produce two intersections
        // (entry and exit), so we must check both before giving up.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - center) / radius; // dividing by radius normalizes the outward normal
        rec.set_face_normal(r, rec.normal);

        return true;

    }

private:
    point3 center;
    double radius;
};

#endif