#include "regiongrow.h"

util::Regions util::plane_detection(Polygon_mesh& polygon_mesh, size_t min_region_size, double max_distance_to_plane_param, double max_accepted_angle_param)
{
    using std::size_t;
    using std::cin;
    using std::cout;
    using std::endl;
    using namespace util;

    cout << "Faces: " << polygon_mesh.number_of_faces() << ' ' << "Vertices: " << polygon_mesh.number_of_vertices() << endl;

    const Face_range face_range = faces(polygon_mesh);

    cout << "* polygon mesh with " << face_range.size() << " faces is loaded" << endl;

    // Default parameter values for the data file polygon_mesh.off.
    const FT          max_distance_to_plane = FT{ max_distance_to_plane_param };
    const FT          max_accepted_angle = FT{ max_accepted_angle_param };

    // Create instances of the classes Neighbor_query and Region_type.
    Neighbor_query neighbor_query(polygon_mesh);
    const Vertex_to_point_map vertex_to_point_map(
        get(CGAL::vertex_point, polygon_mesh));

    Region_type region_type(
        polygon_mesh,
        max_distance_to_plane, max_accepted_angle, min_region_size,
        vertex_to_point_map);

    // Sort face indices.
    Sorting sorting(
        polygon_mesh, neighbor_query,
        vertex_to_point_map);

    sorting.sort();
    // Create an instance of the region growing class.
    Region_growing region_growing(
        face_range, neighbor_query, region_type,
        sorting.seed_map());

    // Run the algorithm.
    Regions regions;

    region_growing.detect(std::back_inserter(regions));

    // Print the number of found regions.
    cout << "* " << regions.size() <<
        " regions have been found"
        << endl;

    return regions;
}

bool util::colorize_mesh(Polygon_mesh& polygon_mesh, const Regions& regions)
{
    std::default_random_engine engine;
    std::uniform_int_distribution<> color_dist(0, 255);

    // Save the result in a file only if it is stored in CGAL::Surface_mesh.
    using Face_index = typename Polygon_mesh::Face_index;

    bool created;

    typename Polygon_mesh::template Property_map<Face_index, Color> face_color;
    boost::tie(face_color, created) =
        polygon_mesh.template add_property_map<Face_index, Color>(
            "f:color", Color(0, 0, 0));

    if (!created)
    {
        return created;
    }

    // Iterate through all region items.
    using size_type = typename Polygon_mesh::size_type;

    // Iterate through all regions.
    for (const auto& region : regions) {
        // Generate a random color.
        const Color color(
            static_cast<unsigned char>(color_dist(engine)),
            static_cast<unsigned char>(color_dist(engine)),
            static_cast<unsigned char>(color_dist(engine)));

        for (const auto index : region)
        {
            face_color[Face_index(static_cast<size_type>(index))] = color;
        }
    }

    return true;
}