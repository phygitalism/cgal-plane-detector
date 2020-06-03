#pragma once

#include "stdafx.h"
#include "cgal.h"

namespace io
{
    bool save_planes(std::string path_to_file, const std::vector<util::Rectangle> & planes);
}
