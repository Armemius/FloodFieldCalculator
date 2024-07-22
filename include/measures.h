#pragma once

#include <ostream>

namespace ffc {
    namespace measures {
        struct coords {
            double x;
            double y;
            double z;
        };

        double distance(const coords &from, const coords &to);

        std::ostream &operator<<(std::ostream &os, const coords &coords);

        template<typename T>
        struct dimensions {
            T width;
            T height;
        };

        template<typename T>
        std::ostream &operator<<(std::ostream &os,
                                 const dimensions<T> &dimensions);
    }
} // namespace ffc::measures
