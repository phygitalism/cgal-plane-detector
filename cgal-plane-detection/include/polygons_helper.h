#pragma once

#include "cgal.h"

namespace util
{
    std::vector<Point_3> polygon_to_rectangle_on_plane(const std::vector<Point_3>& points, const Plane_3& plane);
}
