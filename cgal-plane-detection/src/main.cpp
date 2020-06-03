#include "savemesh.h"
#include "objreader.h"
#include "regiongrow.h"
#include "planefit.h"
#include "processing.h"
#include "cgal.h"
#include "saveplanes.h"

#ifdef VTK_VIS

#include "vtk_vis.h"

#endif

namespace cliargs
{
    constexpr char * help_arg_name{ u8"help" };
    constexpr char * help_short_arg_name{ u8"h" };
    constexpr char * input_arg_name{ u8"input" };
    constexpr char * input_short_arg_name{ u8"i" };
    constexpr char * output_arg_name{ u8"output" };
    constexpr char * output_short_arg_name{ u8"o" };
    constexpr char * angle_arg_name{ u8"max-angle" };
    constexpr char * max_distance_arg_name{ u8"max-distance" };
    constexpr char * min_reg_size_arg_name{ u8"min-regsize" };
    constexpr char * min_plane_area_arg_name{ u8"min-plane-area" };
    constexpr char * clean_arg_name{ u8"clean" };
    constexpr char * clean_short_arg_name{ u8"c" };

#ifdef VTK_VIS
    constexpr char * display_arg_name{ u8"display" };
    constexpr char * display_arg_short_name{ u8"d" };
#endif
}

auto parse_cli_args(int argc, char ** argv)
{
    using namespace boost::program_options;
    using namespace cliargs;
    using std::string;

    string help_arg{ help_arg_name };
    help_arg += ',';
    help_arg += help_short_arg_name;

    string input_arg{ input_arg_name };
    input_arg += ',';
    input_arg += input_short_arg_name;

    string output_arg{ output_arg_name };
    output_arg += ',';
    output_arg += output_short_arg_name;

    string clean_arg{ clean_arg_name };
    clean_arg += ',';
    clean_arg += clean_short_arg_name;

    options_description desc{ u8"Program options. For meaning of some arguments see: https://doc.cgal.org/latest/Shape_detection/classCGAL_1_1Shape__detection_1_1Point__set_1_1Least__squares__plane__fit__region.html#acae6e40ba29b98a433e125622ba5ec80" };
    desc.add_options()
        (help_arg.c_str(), u8"-i input.obj -o output.ply")
        (input_arg.c_str(), value<string>(), u8"Path to .obj file")
        (output_arg.c_str(), value<string>(), u8"Path to .ply file")
        (angle_arg_name, value<double>()->default_value(20), u8"Max accepted angle in degree for the region growing algorithm (see reference).")
        (max_distance_arg_name, value<double>()->default_value(0.4), u8"Max distance to plane for the region growing algorithm (see reference).")
        (min_reg_size_arg_name, value<long>()->default_value(3), u8"A minimum size of region for region growing algorithm. Number of points (see reference).")
        (min_plane_area_arg_name, value<double>()->default_value(1), u8"A minimum area of the plane to detection. Dependent on measure unit.")
        (clean_arg.c_str(), bool_switch(), u8"Make some mesh preprocessing: merge duplicate points, merge duplicate polygons and try to orient.")
        ;

#ifdef VTK_VIS
    string display_arg{ display_arg_name };
    display_arg += ',';
    display_arg += display_arg_short_name;
    desc.add_options()(display_arg.c_str(), bool_switch(), u8"Visualize result in VTK.");
#endif

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    return std::make_tuple(vm, desc);
}

bool validate_args(const boost::program_options::variables_map & args, const boost::program_options::options_description & desc)
{
    using namespace boost::filesystem;
    using namespace cliargs;
    using std::string;

    if (args.count(help_arg_name))
    {
        return true;
    }

    if (!args.count(input_arg_name))
    {
        cerr << u8"Missing required argument '" << input_arg_name << '\'' << endl;
        return false;
    }

    if (!args.count(output_arg_name))
    {
        cerr << u8"Missing required argument '" << output_arg_name << '\'' << endl;
        return false;
    }

    path input_file{ args[input_arg_name].as<string>() };
    path output_file{ args[output_arg_name].as<string>() };

    if (status(input_file).type() != file_type::regular_file)
    {
        cerr << u8"The file " << input_file << u8" is not regular file or does not exist." << endl;
        return false;
    }

    if (output_file.empty())
    {
        cerr << u8"An output path is empty" << endl;
        return false;
    }

    if (!output_file.has_stem())
    {
        cerr << u8"An output path does not have filename." << endl;
        return false;
    }

    double angle{ args[angle_arg_name].as<double>() };

    if (angle < 0 || angle > 90)
    {
        cerr << u8"Invalid value for max angle: " << angle << " Accepted values are in [0; 90]" << endl;
        return false;
    }

    double distance{ args[max_distance_arg_name].as<double>() };

    if (distance < 0)
    {
        cerr << u8"Negative value for distance: " << distance << endl;
        return false;
    }

    long region_size{ args[min_reg_size_arg_name].as<long>() };

    if (region_size <= 0)
    {
        cerr << u8"A region size must be greater than 0. Value is " << region_size << endl;
        return false;
    }

    double area{ args[min_plane_area_arg_name].as<double>() };

    if (area <= 0)
    {
        cerr << u8"A minimum area of plane is negative or zero. Value is " << area << endl;
        return false;
    }

    return true;
}

int main(int argc, char * argv[])
{
    using namespace boost::program_options;
    using namespace boost::filesystem;
    using namespace cliargs;
    using namespace util;
    using std::string;
    using std::vector;

    variables_map args;

    try
    {
        auto res_tuple = parse_cli_args(argc, argv);
        args = get<0>(res_tuple);
        auto desc = get<1>(res_tuple);

        if (args.count(help_arg_name))
        {
            cout << desc << endl;
            return 0;
        }

        bool is_valid{ validate_args(args, desc) };

        if (!is_valid)
        {
            cout << u8"See --help" << endl;
            return 1;
        }
    }
    catch (error & err)
    {
        cerr << err.what() << endl;
        return 1;
    }

    path input_file{ args[input_arg_name].as<string>() };
    path output_file{ args[output_arg_name].as<string>() };

    vector<util::Point_3> points;
    vector<util::FaceIndices> faces;

    if (!util::read_obj(input_file.string(), points, faces))
    {
        cerr << u8"Error occurred when reading " << input_file << endl;
        return 1;
    }

    if (args[clean_arg_name].as<bool>())
    {
        processing::clean_mesh(points, faces);
    }

    try
    {
        util::Polygon_mesh polygon_mesh;

        CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh(points, faces, polygon_mesh);

        points.clear();
        faces.clear();

        long min_size{ args[min_reg_size_arg_name].as<long>() };
        double max_angle{ args[angle_arg_name].as<double>() }, max_distance{ args[max_distance_arg_name].as<double>() };

        auto regions = plane_detection(polygon_mesh, min_size, max_distance, max_angle);

        double min_area{ args[min_plane_area_arg_name].as<double>() };

        vector<util::Rectangle> planes;

        util::Polygon_mesh new_mesh{ util::build_planes(polygon_mesh, regions, planes, min_area) };

        bool is_color_mesh = colorize_mesh(polygon_mesh, regions);

        if (!is_color_mesh)
        {
            cerr << u8"Error while creating color info. Cannot save a segmented mesh." << endl;
        }

        path planes_mesh = output_file.stem().concat(u8"-planes.ply");

        path planes_path = output_file.stem().concat(u8"-planes.txt");

        if (is_color_mesh)
        {
            if (!save_ply(output_file.string(), polygon_mesh))
            {
                cerr << u8"Cannot save segmented polygon mesh" << endl;
                return 1;
            }
            else
            {
                cout << u8"Segmented polygon mesh is saved in "
                    << output_file << endl;
            }
        }

        if (!io::save_planes(planes_path.string(), planes))
        {
            cerr << u8"Cannot save detected planes" << endl;
        }
        else
        {
            cout << u8"Detected planes saved in " << planes_path << endl;
        }

        if (!save_ply(planes_mesh.string(), new_mesh))
        {
            cerr << u8"Cannot save detected planes as mesh" << endl;
            return 1;
        }
        else
        {
            cout << u8"Detected planes as mesh saved in " << planes_mesh << endl;

#ifdef VTK_VIS

            if (is_color_mesh && args[display_arg_name].as<bool>())
            {
                cout << u8"Press q to exit" << endl;
                viz::show_ply(planes_mesh.string(), output_file.string());
            }
#endif
        }
    }
    // CGAL can throw exception
    catch (std::exception & err)
    {
        cerr << err.what() << endl;
        return 1;
    }

    return 0;
}