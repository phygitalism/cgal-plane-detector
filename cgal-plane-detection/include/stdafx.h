#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <array>
#include <tuple>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/math/special_functions/relative_difference.hpp>

#include <CGAL/memory.h>
#include <CGAL/IO/Color.h>
#include <CGAL/Iterator_range.h>
#include <CGAL/min_quadrilateral_2.h>
#include <CGAL/HalfedgeDS_vector.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing_on_polygon_mesh.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing.h>
#include <CGAL/IO/OBJ_reader.h>
#include <CGAL/IO/PLY_reader.h>
#include <CGAL/polygon_mesh_processing.h>
#include <CGAL/bounding_box.h>
#include <CGAL/linear_least_squares_fitting_3.h>