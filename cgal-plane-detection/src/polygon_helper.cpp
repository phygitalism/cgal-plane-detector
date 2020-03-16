#include "polygons_helper.h"

std::vector<util::Point_3> util::polygon_to_rectangle_on_plane(const std::vector<util::Point_3>& points, const util::Kernel::Plane_3& plane)
{
    using Point_2 = Kernel::Point_2;

    assert(points.size() >= 3);

    // Choosing new basis on the linear subspace. The linear subspace is plane.
    Point_3 new_origin{ points.at(0) };

    Vector_3 new_basis_1;

    for (size_t i{ 1 }; i < points.size(); i++)
    {
        new_basis_1 = { points.at(i) - new_origin };

        if (!CGAL::is_zero(new_basis_1.squared_length()))
        {
            break;
        }
    }

    Vector_3 new_basis_3{ plane.orthogonal_vector() };
    Vector_3 new_basis_2{ CGAL::cross_product(new_basis_3, new_basis_1) };
    Vector_3 origin_shifting{ new_origin - CGAL::ORIGIN };

    // Matrix of a transformation with respect to a basis. From new coordinate system to origin.
    Kernel::Aff_transformation_3 transformation{
        new_basis_1.x(), new_basis_2.x(), new_basis_3.x(), origin_shifting.x(),
        new_basis_1.y(), new_basis_2.y(), new_basis_3.y(), origin_shifting.y(),
        new_basis_1.z(), new_basis_2.z(), new_basis_3.z(), origin_shifting.z()
    };

    Kernel::Aff_transformation_3 inveres_transformation{ transformation.inverse() };

    // Projections on plane. All points on plane have third coordinate is equal to zero.
    std::vector<Point_2> new_projection_points(points.size(), CGAL::ORIGIN);

    std::transform(points.cbegin(), points.cend(), new_projection_points.begin(), [&inveres_transformation](const Point_3& point) {
        auto projection{ point.transform(inveres_transformation) };
        return Point_2{ projection.x(), projection.y() };
        });

    const int num_rectangle_vertices{ 4 };

    std::vector<Point_2> rectangle_projection{ num_rectangle_vertices, CGAL::ORIGIN };

    CGAL::min_rectangle_2(new_projection_points.cbegin(), new_projection_points.cend(), rectangle_projection.begin());

    std::vector<Point_3> rectangle{ num_rectangle_vertices, CGAL::ORIGIN };

    std::transform(rectangle_projection.cbegin(), rectangle_projection.cend(), rectangle.begin(), [&transformation](const Point_2& point) {
        return Point_3{ point.x(), point.y(), 0 }.transform(transformation);
        });

    return rectangle;
}