#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>

#include "retqss_geometry.hh"
#include "retqss_parser.hh"
#include "retqss_types.hh"
#include "retqss_polyhedron.hh"
#include "retqss_utilities.hh"
#include "retqss_exceptions.hh"


retQSS::GeometryParser *retQSS::GeometryParser::build_from(
		const char *filename)
{
	std::string geom_file(filename);
	size_t dot_index = geom_file.find_last_of(".");
	size_t last_index = geom_file.find_last_of("/");
	std::string path = geom_file.substr(0, last_index);
	retQSS::GeometryParser *parser = nullptr;

	if(dot_index == std::string::npos || dot_index < last_index)
		parser = new retQSS::OFFParser(path, geom_file);
	else
	{
		std::string ext = geom_file.substr(dot_index+1, geom_file.size()-dot_index-1);
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

		if(ext == "off")
			parser = new retQSS::OFFParser(path, geom_file, true);
		else if(ext == "vtk")
			parser = new retQSS::VTKParser(path, geom_file);
		else
			throw retQSS::GeometryParserException("Unrecognized geometry file format!");
	}

	return parser;
}

retQSS::GeometryParser::GeometryParser(
		const std::string &path,
		const std::string &filename) :
				path(path),
				filename(filename)
{
}

void retQSS::GeometryParser::parse(retQSS::Geometry *geometry)
{
	std::ifstream file(this->filename);

	if(!file.good())
		throw(retQSS::GeometryParserException("Failed to read geometry!"));

	try
	{
		this->do_parse(file, geometry);
		this->classify_unmatched_faces(geometry);
		geometry->close();
	}
	catch(retQSS::GeometryParserException &exc)
	{
		file.close();
		throw(exc);
	}

	file.close();
}

void retQSS::GeometryParser::extract_unmatched_faces(
		retQSS::PolyhedralVolume *vol)
{
	for(auto face : vol->faces())
		if(!vol->is_internal_face(face))
		{
			// This tree is used for fast post-processing of the faces (see
			// below).
			this->tree.insert(face);
			this->unmatched_faces.push_back(face);
		}
}

void retQSS::GeometryParser::classify_unmatched_faces(
		retQSS::Geometry *geometry)
{
	double eps = 1e-5, d = eps*eps + 1e-8;
	std::set<retQSS::PolyhedronFace*> matched;

	this->tree.close();

	for(auto face : this->unmatched_faces)
	{
		if(matched.find(face) != matched.end())
			continue;

		auto face_vol = face->volume();
		bool is_boundary_face = true;

		Vector_3 normal = face->normal();
		Point_3 point = face->centroid() - eps*normal;
		Ray_3 ray(point, normal);

		// Second (optional) argument to crossed_face is the face we want to
		// ignore for the intersection computation. Internally, we build a
		// skip functor using the triangular subfaces that make up the face
		// and pass it to CGAL routines.
		auto face_and_point = this->tree.crossed_face(ray, face);
		auto crossed_face = std::get<0>(face_and_point);
		auto int_point = std::get<1>(face_and_point);

		if(crossed_face != nullptr && face_vol != crossed_face->volume())
		{
			double dist = CGAL::squared_distance(point, int_point);
			if(dist <= d)
			{
				if(face->is_face_inside(crossed_face))
				{
					face_vol->add_neighbor(face, crossed_face);
					matched.insert(crossed_face);
				}
				else if(crossed_face->is_face_inside(face))
					face_vol->add_neighbor(face, crossed_face);

				is_boundary_face = false;
			}
		}

		if(is_boundary_face)
			geometry->add_boundary_face(face);
	}
}

void retQSS::OFFParser::do_parse(
		std::ifstream &file,
		retQSS::Geometry *geometry)
{
	std::string line;
	retQSS::PolyhedralVolume *vol;

	if(this->single_file)
	{
		vol = this->new_volume(this->filename, geometry);
		this->extract_unmatched_faces(vol);
		return;
	}

	// Read polyhedrons.
	while(std::getline(file, line))
		this->new_volume(this->path + "/" + line, geometry);

	// Set neighbors.
	for(int i = 1; i < geometry->num_volumes(); i++)
		for(int j = i+1; j < geometry->num_volumes(); j++)
		{
			auto vol1 = geometry->get_polyhedral_volume(i);
			auto vol2 = geometry->get_polyhedral_volume(j);
			vol1->check_neighbor(vol2);
		}

	for(int i = 1; i < geometry->num_volumes(); i++)
		this->extract_unmatched_faces(geometry->get_polyhedral_volume(i));
}

retQSS::PolyhedralVolume *retQSS::OFFParser::new_volume(
		const std::string &filename,
		retQSS::Geometry *geometry)
{
	std::ifstream off_file(filename);
	auto poly = new retQSS::Polyhedron();
	off_file >> *poly;

	size_t first_index = 1 + filename.find_last_of("/");
	size_t last_index = filename.find_last_of(".");
	std::string name = filename.substr(first_index, last_index-first_index);

	return geometry->add_volume(poly, name);
}

retQSS::VTKParser::VTKParser(
		const std::string &path,
		const std::string &filename) :
	retQSS::GeometryParser(path, filename)
{
	this->supported_cells.insert(retQSS::VTK_TETRA);
	this->supported_cells.insert(retQSS::VTK_VOXEL);
	this->supported_cells.insert(retQSS::VTK_HEXAHEDRON);
}

void retQSS::VTKParser::do_parse(
		std::ifstream &file,
		retQSS::Geometry *geometry)
{
	std::string line, error, kwd1, kwd2;
	size_t n_points, n_cells, size, n_types, n_data, n_field;
	std::stringstream err;
	std::vector<Point_3> points;
	std::vector<std::vector<size_t>> cells;
	cell_data_t cell_data;

	std::getline(file, line);
	if(	line != "# vtk DataFile Version 2.0" &&
		line != "# vtk DataFile Version 3.0")
	{
		error = "Wrong VTK header";
		goto raise_exception;
	}

	// Name ignored
	std::getline(file, line);

	std::getline(file, line);
	if(	line != "ASCII")
	{
		error = "Non-ASCII input not supported";
		goto raise_exception;
	}

	std::getline(file, line);
	if(	line != "DATASET UNSTRUCTURED_GRID")
	{
		error = "Only unstructured grids are supported";
		goto raise_exception;
	}

	if(!(file >> kwd1 >> n_points >> kwd2) ||
		kwd1 != "POINTS")
	{
		error = "Line 5: invalid point data";
		goto raise_exception;
	}

	for(size_t i = 0; i < n_points; i++)
	{
		double x, y, z;

		if(!(file >> x >> y >> z))
		{
			err << "Point #" << i << ": invalid coordinates";
			error = err.str();
			goto raise_exception;
		}

		points.push_back(Point_3(x, y, z));
	}

	if(!(file >> kwd1 >> n_cells >> size) ||
		kwd1 != "CELLS")
	{
		error = "Invalid cell data";
		goto raise_exception;
	}

	for(size_t i = 0; i < n_cells; i++)
	{
		std::vector<size_t> cell;
		size_t n_indices, index;

		if(!(file >> n_indices))
		{
			err << "Cell #" << i << ": invalid index count";
			error = err.str();
			goto raise_exception;
		}

		for(size_t j = 0; j < n_indices; j++)
		{
			if(!(file >> index) || index >= n_points)
			{
				err << "Cell #" << i << ": index #" << j << " is invalid";
				error = err.str();
				goto raise_exception;
			}

			cell.push_back(index);
		}

		cells.push_back(cell);
	}

	if(!(file >> kwd1 >> n_types) ||
		kwd1 != "CELL_TYPES" || n_types != n_cells)
	{
		error = "Invalid cell type data";
		goto raise_exception;
	}

	for(size_t i = 0; i < n_types; i++)
	{
		size_t type;

		if(!(file >> type))
		{
			err << "Cell type #" << i << ": invalid format";
			error = err.str();
			goto raise_exception;
		}

		auto vtk_type = static_cast<retQSS::VTKCellType>(type);
		auto type_it = this->supported_cells.find(vtk_type);
		if(type_it == this->supported_cells.end())
		{
			err << "Cell type #" << i << ": type "
				<< type << " currently not supported";
			error = err.str();
			goto raise_exception;
		}

		auto cells_it = cells[i].begin();
		cells[i].insert(cells_it, type);
	}

	if(!(file >> kwd1))
		goto parse_ok;

	if(!(file >> n_data) ||
		kwd1 != "CELL_DATA" || n_data != n_cells)
	{
		error = "Invalid cell data header";
		goto raise_exception;
	}

	if(!(file >> kwd1 >> kwd2 >> n_field) ||
		kwd1 != "FIELD")
	{
		error = "Invalid field header";
		goto raise_exception;
	}

	for(size_t i = 0; i < n_field; i++)
	{
		std::string array_name, data_type;
		size_t n_comp, n_tup;
		double value;

		file >> array_name >> n_comp >> n_tup >> data_type;

		if(n_tup != n_cells)
		{
			err << "Field data array " << array_name << " should have " << n_cells << " tuples";
			error = err.str();
			goto raise_exception;
		}

		if(data_type != "double" && data_type != "int" && data_type != "float")
		{
			err << "Field data array " << array_name << " has invalid data type";
			error = err.str();
			goto raise_exception;
		}

		std::vector<std::vector<double>> data_array;

		while(n_tup--)
		{
			std::vector<double> values;
			for(size_t _ = 0; _ < n_comp; _++)
			{
				if(!(file >> value))
				{
					err << "Field data array " << array_name << " has an invalid number of entries";
					error = err.str();
					goto raise_exception;
				}
				values.push_back(value);
			}
			data_array.push_back(values);
		}

		cell_data[array_name] = data_array;
	}

	goto parse_ok;

raise_exception:
	error = "VTK parser exception: " + error;
	throw retQSS::GeometryParserException(error.c_str());

parse_ok:
	this->build_geometry(points, cells, geometry);
	this->assign_data(cell_data, geometry);
}

void retQSS::VTKParser::build_geometry(
		const std::vector<Point_3> &points,
		const std::vector<std::vector<size_t>> &cells,
		retQSS::Geometry *geometry)
{
	std::unordered_map<std::string, retQSS::Volume*> neighbors;

	for(auto cell : cells)
	{
		switch(cell[0])
		{
		case retQSS::VTK_TETRA:
			this->build_tetrahedron(points, cell, neighbors, geometry);
			break;

		case retQSS::VTK_VOXEL:
			this->build_voxel(points, cell, neighbors, geometry);
			break;

		case retQSS::VTK_HEXAHEDRON:
			this->build_hexahedron(points, cell, neighbors, geometry);
			break;
		}
	}

	for(int i = 1; i < geometry->num_volumes(); i++)
		this->extract_unmatched_faces(geometry->get_polyhedral_volume(i));
}

void retQSS::VTKParser::assign_data(
		const cell_data_t &cell_data,
		retQSS::Geometry *geometry)
{
	for(auto data : cell_data)
	{
		std::string name = data.first;
		auto &v = data.second;

		for(size_t i = 0; i < v.size(); i++)
		{
			auto vol = geometry->get_polyhedral_volume(i+1);
			vol->set_property(name, v[i]);
		}
	}
}

retQSS::PolyhedralVolume *retQSS::VTKParser::build_tetrahedron(
		const std::vector<Point_3> &points,
		std::vector<size_t> &cell,
		std::unordered_map<std::string, retQSS::Volume*> &neighbors,
		retQSS::Geometry *geometry)
{
	retQSS::Polyhedron *poly = new retQSS::Polyhedron();
	FaceIdx result;
	std::stringstream name;

	name << "Tetra";

	for(size_t i = 1; i < cell.size(); i++)
	{
		auto index = cell[i];
		poly->add_vertex(points[index]);
		name << "-" << index;
	}

	// Assume tetrahedral cell. Add four faces as indicated by VTK format.
	// WARNING: CGAL requires faces to be added in a very particular vertex
	// order. If e.g. we swap vertex indices below, one of the additions
	// might fail.
	std::vector<std::vector<size_t>> indices =
		{{0,1,2}, {0,3,1}, {0,2,3}, {1,3,2}};

	for(auto v : indices)
	{
		result = poly->add_face(
				VertexIndex(v[0]),
				VertexIndex(v[1]),
				VertexIndex(v[2]));
		if(result == retQSS::Polyhedron::null_face())
			throw retQSS::GeometryParserException("Cannot add polyhedron face!");
	}

	auto vol = geometry->add_volume(poly, name.str());

	// Compute neighbors.
	cell.erase(cell.begin());

	for(auto v : indices)
	{
		std::vector<size_t> face {cell[v[0]], cell[v[1]], cell[v[2]]};
		this->add_neighbor(vol, neighbors, face);
	}

	return vol;
}

retQSS::PolyhedralVolume *retQSS::VTKParser::build_voxel(
		const std::vector<Point_3> &points,
		std::vector<size_t> &cell,
		std::unordered_map<std::string, retQSS::Volume*> &neighbors,
		retQSS::Geometry *geometry)
{
	retQSS::Polyhedron *poly = new retQSS::Polyhedron();
	FaceIdx result;
	std::stringstream name;

	name << "Voxel";

	for(size_t i = 1; i < cell.size(); i++)
	{
		auto index = cell[i];
		poly->add_vertex(points[index]);
		name << "-" << index;
	}

	std::vector<std::vector<size_t>> indices =
		{{0,1,3,2}, {5,7,3,1}, {6,7,5,4}, {4,0,2,6}, {7,6,2,3}, {1,0,4,5}};

	for(auto v : indices)
	{
		result = poly->add_face(
					VertexIndex(v[0]),
					VertexIndex(v[1]),
					VertexIndex(v[2]),
					VertexIndex(v[3]));
		if(result == retQSS::Polyhedron::null_face())
			throw retQSS::GeometryParserException("Cannot add polyhedron face!");
	}

	auto vol = geometry->add_volume(poly, name.str());

	// Compute neighbors.
	cell.erase(cell.begin());

	for(auto v : indices)
	{
		std::vector<size_t> face {cell[v[0]], cell[v[1]], cell[v[2]], cell[v[3]]};
		this->add_neighbor(vol, neighbors, face);
	}

	return vol;
}

retQSS::PolyhedralVolume *retQSS::VTKParser::build_hexahedron(
		const std::vector<Point_3> &points,
		std::vector<size_t> &cell,
		std::unordered_map<std::string, retQSS::Volume*> &neighbors,
		retQSS::Geometry *geometry)
{
	retQSS::Polyhedron *poly = new retQSS::Polyhedron();
	FaceIdx result;
	std::stringstream name;

	name << "Hexa";

	for(size_t i = 1; i < cell.size(); i++)
	{
		auto index = cell[i];
		poly->add_vertex(points[index]);
		name << "-" << index;
	}

	std::vector<std::vector<size_t>> indices =
		{
			{0,1,2,3},
			{5,6,2,1},
			{7,6,5,4},
			{4,0,3,7},
			{6,7,3,2},
			{1,0,4,5}
		};

	for(auto v : indices)
	{
		result = poly->add_face(
					VertexIndex(v[0]),
					VertexIndex(v[1]),
					VertexIndex(v[2]),
					VertexIndex(v[3]));
		if(result == retQSS::Polyhedron::null_face())
			throw retQSS::GeometryParserException("Cannot add polyhedron face!");
	}

	auto vol = geometry->add_volume(poly, name.str());

	// Compute neighbors.
	cell.erase(cell.begin());

	for(auto v : indices)
	{
		std::vector<size_t> face {cell[v[0]], cell[v[1]], cell[v[2]], cell[v[3]]};
		this->add_neighbor(vol, neighbors, face);
	}

	return vol;
}

void retQSS::VTKParser::add_neighbor(
		retQSS::Volume *vol,
		std::unordered_map<std::string, retQSS::Volume*> &neighbors,
		std::vector<size_t> &face) const
{
		std::sort(face.begin(), face.end());
		std::stringstream face_name;
		face_name << face[0];
		for(size_t i = 1; i < face.size(); i++)
			face_name << "-" << face[i];

		if(neighbors.find(face_name.str()) != neighbors.end())
		{
			retQSS::Volume *neighbor = neighbors[face_name.str()];
			vol->check_neighbor(neighbor);
		}

		neighbors[face_name.str()] = vol;
}
