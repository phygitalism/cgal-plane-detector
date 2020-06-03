#include "intersections.h"

// Find intersections between AABB and plane.
std::vector<util::Point_3> util::plane_aabb_intersection_points(const Kernel::Plane_3 & plane, const Kernel::Iso_cuboid_3 & bbox)
{
    std::vector<Point_3> points;

    // See https://doc.cgal.org/latest/Kernel_23/classCGAL_1_1Iso__cuboid__3.html#a48f499351f3ef6d80751115372489729
    std::array<std::tuple<int, int>, 12> end_points{
        std::make_tuple(0, 1),
        std::make_tuple(3, 2),
        std::make_tuple(5, 6),
        std::make_tuple(4, 7),
        std::make_tuple(0, 3),
        std::make_tuple(1, 2),
        std::make_tuple(5, 4),
        std::make_tuple(6, 7),
        std::make_tuple(0, 5),
        std::make_tuple(1, 6),
        std::make_tuple(2, 7),
        std::make_tuple(3, 4)
    };

    int i{};

    for (const auto & end_point : end_points)
    {
        Kernel::Segment_3 segment{ bbox.vertex(std::get<0>(end_point)), bbox.vertex(std::get<1>(end_point)) };

        auto intersection = CGAL::intersection(segment, plane);

        if (intersection)
        {
            if (const Kernel::Point_3 * p = boost::get<Kernel::Point_3>(&*intersection))
            {
                points.emplace_back(*p);
            }
            // Include ends
            else if (i < 4)
            {
                if (const Kernel::Segment_3 * seg = boost::get < Kernel::Segment_3>(&*intersection))
                {
                    points.emplace_back(seg->start());
                    points.emplace_back(seg->end());
                }
            }
        }

        i++;
    }

    if (!points.empty())
    {
        const Vector_3 plane_normal = plane.orthogonal_vector();
        const Point_3 origin = *points.cbegin();

        // Sort in counter clockwise order
        std::sort(points.begin(), points.end(), [&origin, &plane_normal](const Point_3 & lhs, const Point_3 & rhs) -> bool
            {
                auto v{ CGAL::cross_product(lhs - origin, rhs - origin) };
                return CGAL::scalar_product(v, plane_normal) < 0;
            }
        );
    }

    return points;
}