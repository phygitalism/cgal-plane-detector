#pragma once

#include "cgal.h"

namespace util
{
    /* Original code: https://asawicki.info/news_1428_finding_polygon_of_plane-aabb_intersection.html
    */
    std::vector<Kernel::Point_3> plane_aabb_intersection_points(const Kernel::Plane_3& plane, const Kernel::Iso_cuboid_3& bbox);
}