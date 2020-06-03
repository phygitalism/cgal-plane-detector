#include "objreader.h"

bool util::read_obj(const std::string & filename, std::vector<Point_3> & points_ref, std::vector<FaceIndices> & faces_ref)
{
    using namespace std;

    ifstream in(filename);

    if (!in)
    {
        in.close();
        return false;
    }

    bool isRead = CGAL::read_OBJ(in, points_ref, faces_ref);

    in.close();

    if (!isRead)
    {
        return false;
    }

    return isRead;
}