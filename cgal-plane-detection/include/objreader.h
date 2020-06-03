#pragma once

#include "cgal.h"

namespace util
{
    bool read_obj(const std::string & filename, std::vector<Point_3> & points, std::vector < FaceIndices> & faces_ref);
}