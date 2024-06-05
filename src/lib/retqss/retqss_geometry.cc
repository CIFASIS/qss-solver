#include <CGAL/bounding_box.h>
#include <CGAL/centroid.h>

#include "retqss_geometry.hh"
#include "retqss_logical_face.hh"


retQSS::Geometry::Geometry() :
	current_volume_id(1),
	current_vertex_id(0),
	current_face_id(0),
	closed(false)
{
	this->_world = new retQSS::OpenVolume(0, RETQSS_WORLD_NAME);
	this->volume_map[this->_world->get_ID()] = this->_world;
}

retQSS::Geometry::~Geometry()
{
	for(auto pair : this->volume_map)
		delete pair.second;

	for(auto vertex : this->_vertices)
		delete vertex;

	for(auto face : this->_faces)
		delete face;
}

retQSS::PolyhedralVolume *retQSS::Geometry::add_volume(
		retQSS::Polyhedron *poly,
		const std::string &name)
{
	if(this->closed)
		throw retQSS::Exception(
				"Attempting to add new volume into a closed geometry.");

	retQSS::VertexArray vol_vertices;

	for(auto point : poly->get_vertices())
	{
		auto vertex = this->add_vertex(point);
		vol_vertices.push_back(vertex);
	}

	auto vol = new retQSS::PolyhedralVolume(
			this->current_volume_id++,
			name,
			poly,
			vol_vertices);

	for(auto vertex : vol->vertices())
	{
		auto it = this->vertex_to_volumes.find(vertex);
		if(it != this->vertex_to_volumes.end())
			it->second.push_back(vol);
		else
		{
			retQSS::VolumeArray vertex_vols {vol};
			this->vertex_to_volumes[vertex] = vertex_vols;
		}
	}

	this->_volumes.push_back(vol);
	this->volume_map[vol->get_ID()] = vol;

	return vol;
}

retQSS::Vertex *retQSS::Geometry::add_vertex(const Point_3 &point)
{
	if(this->closed)
		throw retQSS::Exception(
				"Attempting to add new vertex into a closed geometry.");

	retQSS::Vertex *vertex;
	auto it = this->vertex_map.find(point);

	if(it == this->vertex_map.end())
	{
		vertex = new retQSS::Vertex(this->current_vertex_id++, point);
		this->vertex_map[point] = vertex;
		this->_vertices.push_back(vertex);
	}
	else
		vertex = it->second;

	return vertex;
}

void retQSS::Geometry::add_boundary_face(retQSS::PolyhedronFace *face)
{
	if(this->closed)
		throw retQSS::Exception(
				"Attempting to add boundary face into a closed geometry.");

	auto face_vol = face->volume();

	this->_world->add_face(face);

	for(auto vertex: face->get_vertices())
		this->_world->add_vertex(vertex);

	face_vol->add_neighbor(face, face, this->_world);
	face_vol->on_boundary = true;
}

void retQSS::Geometry::close()
{
	if(!this->closed)
	{
		this->add_logical_faces();
		this->_world->close();
		this->closed = true;
	}
}

void retQSS::Geometry::add_logical_faces()
{
	retQSS::LogicalFace *logical_face;
	retQSS::PolyhedronFace *primary_face;

	for(auto vol : this->_volumes)
	{
		for(auto vol_face : vol->faces())
		{
			auto &adjacent_faces = vol->face_to_face[vol_face];
			auto n_faces = adjacent_faces.size();
			assert(n_faces > 0);

			if(vol_face->logical_face() == nullptr)
			{
				primary_face = n_faces > 1 ? vol_face : adjacent_faces[0];

				logical_face = new retQSS::LogicalFace(
									this->current_face_id++,
									primary_face);

				this->_faces.push_back(logical_face);

				auto primary_vol = primary_face->volume();
				auto &adjacent_faces_of_primary =
						primary_vol->face_to_face[primary_face];
				for(auto adjacent_face : adjacent_faces_of_primary)
					logical_face->add_physical_face(adjacent_face);
			}
		}
	}
}

retQSS::FaceAndPoint retQSS::Geometry::opposite_intersection(
		retQSS::PolyhedronFace *face,
		const Point_3 &point) const
{
	assert(face->is_on_boundary());

	// 1. Compute the ray to the opposite side of the geometry.
	auto normal = -1 * face->normal();
	Ray_3 ray(point, normal);

	// 2. Compute the intersection with the opposite face f' on the geometry
	// surface, skipping face f.
	auto face_and_point = this->_world->crossed_face(ray, face);

	assert(std::get<0>(face_and_point) != nullptr);

	return face_and_point;
}

retQSS::FaceAndPoint retQSS::Geometry::opposite_intersection(
		retQSS::PolyhedronFace *face) const
{
	auto centroid = face->centroid();
	return this->opposite_intersection(face, centroid);
}

retQSS::Volume *retQSS::Geometry::opposite_volume(
		retQSS::PolyhedronFace *face) const
{
	auto opposite_face = this->opposite_face(face);
	return opposite_face->volume();
}

retQSS::PolyhedronFace *retQSS::Geometry::opposite_face(
		retQSS::PolyhedronFace *face) const
{
	auto face_and_point = this->opposite_intersection(face);
	return std::get<0>(face_and_point);
}

retQSS::Volume *retQSS::Geometry::adjacent_volume(
		retQSS::Volume *volume,
		retQSS::PolyhedronFace *face,
		const Point_3 &point,
		retQSS::PolyhedronFace **adjacent_face_ptr) const
{
	retQSS::PolyhedronFace *adjacent_face;

	retQSS::Volume *adjacent_vol;

	adjacent_face =
			const_cast<retQSS::PolyhedronFace*>(
					volume->get_neighbor_face(face,point));

	// 1. There is no adjacent face. This can happen if currently there is no
	// boundary crossing scheduled for this particle.
	if(adjacent_face == nullptr)
		adjacent_vol = volume;

	// 2. The exit face and and the entry face are the same object (which
	// means that this is a boundary face) and the volume we are inspecting is
	// an internal volume. Thus, the adjacent volume is the exterior of the
	// geometry.
	else if(face == adjacent_face && volume != this->_world)
		adjacent_vol = this->_world;

	// 3. This covers two different scenarios:
	//   i. Exit face and entry face are different objects, and so they
	//   belong to different volumes. Adjacent volume is, thus, the owner of
	//   the entry face.
	//  ii. Same as case 2. but now the inspected volume is the outside world.
	//  Thus, the adjacent volume is the one that owns the entry face.
	else
		adjacent_vol = adjacent_face->volume();

	if(adjacent_face_ptr != nullptr)
		*adjacent_face_ptr = adjacent_face;

	return adjacent_vol;
}

retQSS::Volume *
retQSS::Geometry::locate(
		const Point_3 &point,
		const Vector_3 &direction) const
{
	Ray_3 ray(point, direction);

	retQSS::Volume *current_vol = this->_world;

	for(auto vol : this->volumes())
	{
		retQSS::PointLocation location = vol->locate_point(point);

		switch(location)
		{
		case retQSS::PointLocation::POINT_INSIDE:
			current_vol = vol;
			break;

		case retQSS::PointLocation::POINT_ON_BOUNDARY:
			// If particle has no velocity, use this volume (it can actually
			// lie in the neighborging volume, but we will not care since
			// most likely this is a phantom particle that might get
			// initialized later during the simulation).
			if(ray.is_degenerate())
			{
				current_vol = vol;
				break;
			}

			// Check if the ray induced by the direction of the particle
			// crosses a different face.
			for(auto face : vol->faces())
			{
				if(	!face->is_point_inside(point) &&
					face->intersected_by(ray))
				{
					current_vol = vol;
					break;
				}
			}
			break;

		default:
			break;
		}

		if(current_vol != this->_world)
			break;
	}

	return current_vol;
}

bool retQSS::Geometry::dump_volume(
		VolumeID id,
		const std::string &filename) const
{
	int idx = 0, n_cell_points = 0, n_cells = 0;
	std::stringstream cell_stream, cell_type_stream;
	std::unordered_map<retQSS::Vertex*, int> vertex_idx;

	auto volume = this->get_volume(id);
	int n_vertices = volume->num_vertices();

	std::ofstream out(filename);
	if(!out.good())
		throw retQSS::FileAccessException(filename);

	out.precision(10);

	out
		<< "# vtk DataFile Version 3.0\n"
		<< "retQSS volume "
		<< volume->get_name() << " (ID "
		<< id << ")\n"
		<< "ASCII\n"
		<< "DATASET UNSTRUCTURED_GRID\n\n";

	out << "POINTS " << n_vertices << " double\n";

	for(auto vertex: volume->vertices())
	{
		auto point = vertex->point();

		out
			<< point.x() << " "
			<< point.y() << " "
			<< point.z() << "\n";

		vertex_idx[vertex] = idx++;
	}

	for(auto face: volume->faces())
	{
		auto face_type = face->vtk_cell_type();
		int face_points = face->num_vertices();
		n_cells++;
		n_cell_points += face_points;

		cell_stream << face_points << " ";
		for(int i = 0; i < face_points; i++)
			cell_stream << vertex_idx[face->vertex(i)] << " ";
		cell_stream << "\n";

		cell_type_stream << face_type << "\n";
	}

	out << "\nCELLS " << n_cells << " " << (n_cells+n_cell_points) << "\n";
	out << cell_stream.str();

	out << "\nCELL_TYPES " << n_cells << "\n";
	out << cell_type_stream.str();

	out.close();

	return true;
}

bool retQSS::Geometry::dump_stats(const std::string &filename) const
{
	double
		total_capacity = 0,
		min_capacity = 1e20,
		max_capacity = 0;

	Iso_cuboid_3 min_bbox, max_bbox;

	std::ofstream out(filename);
	if(!out.good())
		throw retQSS::FileAccessException(filename);

	out.precision(10);

	out
		<< "Volumes: "
		<< this->_volumes.size()
		<< "\n";

	out
		<< "Vertices: "
		<< this->_vertices.size()
		<< " ("
		<< this->_world->num_vertices()
		<< " on surface)\n\n";

	std::vector<Point_3> surface_points;
	for(auto vertex : this->_world->vertices())
		surface_points.push_back(vertex->point());

	auto centroid = CGAL::centroid(
						surface_points.begin(),
						surface_points.end());

	auto bbox = CGAL::bounding_box(
						surface_points.begin(),
						surface_points.end());

	out
		<< "Surface vertices centroid: ("
		<< centroid.x() << ", "
		<< centroid.y() << ", "
		<< centroid.z() << ")\n";

	out
		<< "Surface bounding box: "
		<< "(" << bbox.min().x()
		<< ", " << bbox.min().y()
		<< ", " << bbox.min().z()
		<< ") - "
		<< "(" << bbox.max().x()
		<< ", " << bbox.max().y()
		<< ", " << bbox.max().z()
		<< ")\n";

	out
		<< "Surface bounding box dimensions: "
		<< (bbox.max().x() - bbox.min().x())
		<< " x "
		<< (bbox.max().y() - bbox.min().y())
		<< " x "
		<< (bbox.max().z() - bbox.min().z())
		<< "\n\n";

	for(auto vol : this->_volumes)
	{
		double vol_capacity = vol->capacity();

		total_capacity += vol_capacity;

		if(vol_capacity < min_capacity)
		{
			min_bbox = vol->bounding_box();
			min_capacity = vol_capacity;
		}

		if(vol_capacity > max_capacity)
		{
			max_bbox = vol->bounding_box();
			max_capacity = vol_capacity;
		}
	}

	out
		<< "Total volume capacity: "
		<< total_capacity
		<< "\n";

	out
		<< "Average volume capacity: "
		<< (total_capacity/this->_volumes.size())
		<< "\n";

	out
		<< "Min volume capacity: "
		<< min_capacity
		<< " (bounded by box of "
		<< (min_bbox.max().x() - min_bbox.min().x())
		<< " x "
		<< (min_bbox.max().y() - min_bbox.min().y())
		<< " x "
		<< (min_bbox.max().z() - min_bbox.min().z())
		<< ")\n";

	out
		<< "Max volume capacity: "
		<< max_capacity
		<< " (bounded by box of "
		<< (max_bbox.max().x() - max_bbox.min().x())
		<< " x "
		<< (max_bbox.max().y() - max_bbox.min().y())
		<< " x "
		<< (max_bbox.max().z() - max_bbox.min().z())
		<< ")\n";

	out.close();

	return true;
}
