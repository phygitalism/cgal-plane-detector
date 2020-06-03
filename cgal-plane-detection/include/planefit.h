#pragma once

#include "cgal.h"
#include "regiongrow.h"
#include "intersections.h"

namespace util
{
    Polygon_mesh build_planes(const Polygon_mesh &, const Regions &, std::vector<Rectangle> & rectangles, double min_area = 1);
}