#pragma once

#include "cgal.h"

namespace processing
{
    void clean_mesh(std::vector < util::Point_3 > & points, std::vector<util::FaceIndices> & faces);
}