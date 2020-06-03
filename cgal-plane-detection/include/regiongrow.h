#pragma once

#include "cgal.h"

namespace util
{
    using Region_type = CGAL::Shape_detection::Polygon_mesh::Least_squares_plane_fit_region<Kernel, Polygon_mesh>;
    using Face_range = typename Polygon_mesh::Face_range;
    using Neighbor_query = CGAL::Shape_detection::Polygon_mesh::One_ring_neighbor_query<Polygon_mesh>;
    using Vertex_to_point_map = typename Region_type::Vertex_to_point_map;

    using Sorting = CGAL::Shape_detection::Polygon_mesh::Least_squares_plane_fit_sorting<Kernel, Polygon_mesh, Neighbor_query>;
    using Region = std::vector<std::size_t>;
    using Regions = std::vector<Region>;
    using Region_growing = CGAL::Shape_detection::Region_growing<Face_range, Neighbor_query, Region_type, typename Sorting::Seed_map>;

    Regions plane_detection(Polygon_mesh & polygon_mesh, size_t min_region_size = 5, double max_distance_to_plane = 5, double max_accepted_angle = 30);

    bool colorize_mesh(Polygon_mesh & polygon_mesh, const Regions & regions);
}