// A list of all objects in the scene. When a ray is fired, it checks every object
// and returns the closest thing the ray hit.
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "common.h"
#include <vector>

class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects; // every object in the scene lives here

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() {
        objects.clear();
    }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;

        for (const auto& object : objects) {
            // After each hit, update the max distance to only accept something closer.
            // Why: this way we always end up with the nearest object the ray hit, not just any object.
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }


};

#endif