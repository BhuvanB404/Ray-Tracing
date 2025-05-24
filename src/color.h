#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "raytracing.h"
#include "vec3.h"

using color = vec3;

inline double create_gamma(double lcomp) {
    if (lcomp > 0)
        return std::sqrt(lcomp);
    return 0;
}

void write_color(std::ostream &out, const color& pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

        // smootheingng edges
    auto scale = 1.0 / samples_per_pixel;
    r = create_gamma(r * scale);
    g = create_gamma(g * scale);
    b = create_gamma(b * scale);

    // if more than 0.999 breaks code
    static const interval intensity(0.0, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' ' << static_cast<int>(256 * intensity.clamp(g)) << ' '<< static_cast<int>(256 * intensity.clamp(b)) << '\n';
}

#endif

