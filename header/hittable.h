// Defines the hittable abstract base class and hit_record, used to represent
// any object a ray can intersect and the resulting intersection data.
#ifndef HITTABLE_H
#define HITTABLE_H
#include "common.h"

class material;

// Stores intersection data: hit point, surface normal, ray parameter t, and face orientation.
class hit_record {
public:
    point3 p;       // world-space hit point
    vec3 normal;    // always points against the incident ray
    shared_ptr<material> mat;
    double t;       // ray parameter at intersection
    bool front_face; // true if ray hit the outside of the surface

    // Sets normal to always face the incoming ray, tracking which side was hit.
    // Why: shading and refraction both need to know which side of a surface was struck;
    // encoding it here once avoids each material having to recompute it.
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// Abstract base for any geometry a ray can hit; concrete shapes override hit().
class hittable {
public:
    virtual ~hittable() = default;

    // Returns true if ray hits within [ray_tmin, ray_tmax], filling rec with intersection data.
    // Why: the t-range clamp prevents self-intersections (ray_tmin > 0) and lets the caller
    // limit hits to a region of interest, e.g. shadow rays stopping at the light.
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif