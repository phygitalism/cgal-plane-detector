// CGALTest.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "stdafx.h"

namespace util
{
    using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
    using FT = typename Kernel::FT;
    using Point_3 = typename Kernel::Point_3;
    using Vector_3 = typename Kernel::Vector_3;
    using Color = CGAL::Color;
    using Polygon_mesh = CGAL::Surface_mesh<Point_3>;
    using Plane_3 = typename Kernel::Plane_3;
    using Face_index = typename Polygon_mesh::Face_index;
    using FaceIndices = std::vector<std::size_t>;
    using Rectangle = std::array<double, 12>;
}
