#include "savemesh.h"

bool util::save_ply(const std::string& path, const Polygon_mesh& mesh)
{
	std::ofstream out(path);

	bool is_io_error = !out.is_open();

	if (is_io_error)
	{
		std::cout << "Cannot open " << path << std::endl;
		return is_io_error;
	}

	if (!is_io_error)
	{
		is_io_error = !CGAL::write_ply(out, mesh);

		if (is_io_error)
		{
			std::cout << "Cannot save file." << std::endl;
		}
	}

	out.close();

	return !is_io_error;
}