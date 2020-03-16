#include "saveplanes.h"

bool io::save_planes(std::string path_to_file, const std::vector<util::Rectangle>& planes)
{
    using std::cerr;
    using std::endl;

    std::ofstream fstream(path_to_file);

    if (!fstream.is_open())
    {
        cerr << u8"Cannot open " << path_to_file << endl;
        return false;
    }

    fstream << u8"Total: " << planes.size() << endl;
    fstream << u8"x1 y1 z1 x2 y2 z2 x3 y3 z3 x4 y4 z4" << endl;

    for (const auto& rectangle : planes)
    {
        size_t i{};

        for (i = 0; i < rectangle.size() - 1; i++)
        {
            fstream << rectangle[i] << ' ';
        }

        fstream << rectangle[i] << endl;

        if (!fstream.good())
        {
            cerr << u8"Cannot write coordinates" << endl;
            return false;
        }
    }

    return true;
}