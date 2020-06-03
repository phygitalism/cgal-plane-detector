#include "planefit.h"
#include "polygons_helper.h"

util::Polygon_mesh util::build_planes(const Polygon_mesh & mesh, const Regions & regions, std::vector<Rectangle> & rectangles, double min_area)
{
    using size_type = Polygon_mesh::size_type;
    using Halfedge_index = Polygon_mesh::Halfedge_index;
    using vertex_descriptor = Polygon_mesh::Vertex_index;
    using face_descriptor = Polygon_mesh::Face_index;

    Polygon_mesh new_mesh;

    rectangles.clear();

    for (const auto & region : regions)
    {
        std::vector<Point_3> points;

        for (const auto & index : region)
        {
            const Halfedge_index hf = mesh.halfedge(Face_index(index));

            for (const auto & vertex : mesh.vertices_around_face(hf))
            {
                if (vertex.is_valid())
                {
                    points.push_back(mesh.point(vertex));
                }
            }
        }

        Plane_3 plane;

        Point_3 centroid;

        double quality = CGAL::linear_least_squares_fitting_3(points.cbegin(), points.cend(), plane, centroid, CGAL::Dimension_tag<0>());

        if (CGAL::is_zero(quality))
        {
            std::cout << "Bad quality of fitting plane is " << quality << std::endl;
        }

        Kernel::Iso_cuboid_3 bbox = CGAL::bounding_box(points.cbegin(), points.cend());

        points.clear();

        constexpr double eps{ 1E-2 };

        if (bbox.is_degenerate())
        {
            auto mid_point{ CGAL::midpoint(bbox.min(), bbox.max()) };
            double x_size{ bbox.xmax() - bbox.xmin() }, y_size{ bbox.ymax() - bbox.ymin() }, z_size{ bbox.zmax() - bbox.zmin() };

            // Too small size of bbox. Intersections with point my be invalid
            if (boost::math::epsilon_difference(CGAL::to_double(bbox.xmax()), CGAL::to_double(bbox.xmin())) < eps)
            {
                x_size = 2 * eps;
            }

            if (boost::math::epsilon_difference(CGAL::to_double(bbox.ymax()), CGAL::to_double(bbox.ymin())) < eps)
            {
                y_size = 2 * eps;
            }

            if (boost::math::epsilon_difference(CGAL::to_double(bbox.zmin()), CGAL::to_double(bbox.zmax())) < eps)
            {
                z_size = 2 * eps;
            }

            const Vector_3 shift_vector{ x_size / 2, y_size / 2, z_size / 2 };

            bbox = Kernel::Iso_cuboid_3{ mid_point - shift_vector, mid_point + shift_vector };
        }

        auto interesct_points{ plane_aabb_intersection_points(plane, bbox) };

        if (interesct_points.size() >= 3)
        {
            if (interesct_points.size() != 4)
            {
                interesct_points = polygon_to_rectangle_on_plane(interesct_points, plane);
            }

            double length_dir1 = CGAL::sqrt((interesct_points.at(1) - interesct_points.at(0)).squared_length());
            double length_dir2 = CGAL::sqrt((interesct_points.at(3) - interesct_points.at(0)).squared_length());

            if (length_dir1 * length_dir2 >= min_area)
            {
                auto p_min{ interesct_points.at(0) }, p_max{ interesct_points.at(2) };

                auto rectangle_center{ CGAL::midpoint(p_min, p_max) };

                Vector_3 shifting{ centroid - rectangle_center };

                Rectangle rect;

                std::vector<vertex_descriptor> vertex_descriptors;

                for (size_t i = 0, j = 0; i < interesct_points.size(); i++, j += 3)
                {
                    interesct_points.at(i) = interesct_points.at(i) + shifting;
                    rect[j] = CGAL::to_double(interesct_points.at(i).x());
                    rect[j + 1] = CGAL::to_double(interesct_points.at(i).y());
                    rect[j + 2] = CGAL::to_double(interesct_points.at(i).z());
                    vertex_descriptors.push_back(new_mesh.add_vertex(interesct_points.at(i)));
                }

                rectangles.push_back(rect);

                face_descriptor f1 = new_mesh.add_face(vertex_descriptors);

                if (f1 == Polygon_mesh::null_face())
                {
                    std::cerr << u8"The face could not be added because of an orientation error." << std::endl;
                }
            }
        }
        else if (interesct_points.size() > 0 && interesct_points.size() < 3)
        {
            std::cerr << u8"Number of intersections between plane and bbox is incorrect. Number of intersections:" << interesct_points.size() << std::endl;
        }
    }

    return new_mesh;
}