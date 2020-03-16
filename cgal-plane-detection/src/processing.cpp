#include "processing.h"

void processing::clean_mesh(std::vector<util::Point_3>& points, std::vector<util::FaceIndices>& faces)
{
    using namespace CGAL::Polygon_mesh_processing;

    size_t numRemoved = merge_duplicate_points_in_polygon_soup(points, faces);

    std::cout << "Duplicate points: " << numRemoved << std::endl;

    numRemoved = merge_duplicate_polygons_in_polygon_soup(points, faces);

    std::cout << "Duplicate faces: " << numRemoved << std::endl;

    if (!orient_polygon_soup(points, faces))
    {
        std::cout << "Cannot orient polygon soup." << std::endl;
    }
}