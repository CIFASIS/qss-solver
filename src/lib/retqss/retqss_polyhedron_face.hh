/**
 * \file retqss_polyhedron_face.hh
 * \brief Representation of 2D facets of polyhedrons
 */

#ifndef _RETQSS_POLYHEDRON_FACE_H_
#define _RETQSS_POLYHEDRON_FACE_H_

#include <iostream>
#include <iterator>
#include <queue>
#include <set>
#include <utility>
#include <mutex>

#include "retqss_cgal_main_types.hh"
#include "retqss_polyhedron.hh"
#include "retqss_types.hh"
#include "retqss_vertex.hh"

/**
 * Default error tolerance for mathematical tests.
 */
#define _INSIDE_TOLERANCE 1e-09


/**
 * \namespace retQSS Main retQSS namespace
 */
namespace retQSS
{

class LogicalFace;
class Volume;
class PolyhedralVolume;
class OpenVolume;
class PolyhedronFace;
class FaceNeighborhood;
class FaceNeighborhoodIterator;
class Order2IntersectionLocator;
class Order3IntersectionLocator;


/**
 * \brief Representation of a 2D facet of a polyhedron
 *
 * Abstract class that represents a 2D polygonal face of a 3D polyhedron.
 */
class PolyhedronFace
{
	friend class retQSS::LogicalFace;
	friend class retQSS::Volume;
	friend class retQSS::PolyhedralVolume;
	friend class retQSS::Polyhedron;
	friend class retQSS::OpenVolume;
	friend class retQSS::FaceNeighborhoodIterator;
	friend class retQSS::Order2IntersectionLocator;
	friend class retQSS::Order3IntersectionLocator;

protected:
	/**
	 * Polyhedron that owns this face.
	 */
	retQSS::Polyhedron *poly;

	retQSS::VertexArray vertices;

	retQSS::LogicalFace *_logical_face; //!< retQSS object linked to this face.

	/**
	 * Face neighbors belonging to the same polyhedron (see
	 * retQSS::FaceNeighborhood).
	 */
	std::vector<retQSS::PolyhedronFace*> poly_neighbors;
	/**
	 * Face neighbors belonging to the geometry surface, possibly from
	 * different polyhedrons (see retQSS::FaceNeighborhood).
	 */
	std::vector<retQSS::PolyhedronFace*> boundary_neighbors;

	/**
	 * Array of low-level CGAL triangular face indices. Since CGAL requires
	 * triangulated surface meshes for fast intersection and distance query
	 * support, retQSS polyhedrons are transparently triangulated when the
	 * geometry is initialized. This array contains the indices of these
	 * triangular subfaces into which an arbitrary polygonal face is
	 * partitioned.
	 */
	std::vector<FaceIdx> _subfaces;

	double _ox; /// x coordinate of origin (first vertex).
	double _oy; /// y coordinate of origin (first vertex).
	double _oz; /// z coordinate of origin (first vertex).

	double _nx; /// x coordinate of the outward normal vector of this face.
	double _ny; /// y coordinate of the outward normal vector of this face.
	double _nz; /// z coordinate of the outward normal vector of this face.

	/**
	 * Precomputed squared length of the normal vector (it is used for several
	 * algebraic computations --see e.g.
	 * retQSS::PolyhedronFace::is_point_inside()).
	 */
	double _sum_n_sq;
	/**
	 * Precomputed negated dot product of normal vector and origin (first
	 * vertex). This is used mainly for testing if a point is inside the
	 * plane spanned by this face, which in turn is required by the
	 * intersection locator to find interesections between trajectories and
	 * faces (see retQSS::IntersectionLocator).
	 */
	double _sub_n_o;

	/**
	 * Precomputed face centroid.
	 */
	Point_3 _centroid;

	/**
	 * Mutex to protect internal geometry computations in case parallel
	 * simulations are enabled (CGAL is not fully thread-safe).
	 */
	std::mutex cgal_mutex;

	/**
	 * Virtual method to precompute data that is systematically used for
	 * different mathematical tests.
	 * \sa retQSS::PolyhedronFace::_sum_n_sq,
	 * retQSS::PolyhedronFace::_sub_n_o
	 */
	virtual void precompute_data() = 0;
	/**
	 * Precompute common data for every type of face.
	 * \sa retQSS::PolyhedronFace::precompute_data
	 */
	void precompute_common_data(const Vector_3&, const Vector_3&);
	void set_polyhedron_neighbors(const std::vector<retQSS::PolyhedronFace*>&);
	void add_subface(FaceIdx);

	/**
	 * Factory method that instanciates a proper subclass depending on the
	 * number of vertices and their arrangement:
	 *  - Three vertices correspond to a retQSS::TriangularFace.
	 *  - In case of four vertices,
	 *    -# When the angles formed by the edges are right, a
	 *    retQSS::RectangularFace is instanciated.
	 *    -# Otherwise, fall back to a retQSS::QuadrilateralFace.
	 *  - In any other situation, an error message is displayed since currently
	 *  polygonal faces with more than 4 vertices are not supported.
	 */
	static retQSS::PolyhedronFace*
	from_vertices(retQSS::Polyhedron*, retQSS::VertexArray&);

	/**
	 * Constructor is not public: faces are very closely bound to polyhedrons,
	 * and thus we do not want to allow construction of faces as independent
	 * objects.
	 */
	PolyhedronFace(
			retQSS::Polyhedron*,
			retQSS::VertexArray&);

public:
	virtual ~PolyhedronFace() {};

	/**
	 * Test if point \f$(x,y,z)\f$ is inside this face.
	 */
	virtual bool is_point_inside(double x,double y, double z) const = 0;

	/**
	 * Test if point \f$(x,y,z)\f$ is on one of the edges of the face.
	 */
	bool is_point_on_boundary(double x, double y, double z) const;

	/**
	 * \brief Check whether this face is on the geometry surface
	 */
	bool is_on_boundary() const;

	/**
	 * Test if a ray intersects the face.
	 */
	virtual bool intersected_by(const Ray_3 &ray) = 0;

	/**
	 * \brief Type of VTK cell that describes this face
	 * Used e.g. when exporting a volume to a file.
	 */
	virtual retQSS::VTKCellType vtk_cell_type() const = 0;

	bool operator==(const retQSS::PolyhedronFace&) const;

	inline retQSS::LogicalFace *logical_face() const
	{
		return this->_logical_face;
	}

	inline retQSS::PolyhedralVolume *volume() const
	{
		return this->poly->volume();
	}

	inline retQSS::Polyhedron *polyhedron() const
	{
		return this->poly;
	}

	inline retQSS::Vertex *vertex(int i) const
	{
		return this->vertices[i];
	};

	inline int num_vertices() const
	{
		return this->vertices.size();
	}

	inline const retQSS::VertexArray &get_vertices() const
	{
		return this->vertices;
	}

	/**
	 * Face neighbors belonging to the same polyhedron (see
	 * retQSS::FaceNeighborhood).
	 */
	inline const std::vector<retQSS::PolyhedronFace*> &get_polyhedron_neighbors() const
	{
		return this->poly_neighbors;
	}

	/**
	 * Face neighbors belonging to the geometry surface, possibly from
	 * different polyhedrons (see retQSS::FaceNeighborhood).
	 */
	inline const std::vector<retQSS::PolyhedronFace*> &get_boundary_neighbors() const
	{
		return this->boundary_neighbors;
	}

	/**
	 * Add new face that shares one edge with this face in the geometry
	 * surface.
	 *
	 * This method is used when initalizing the geometry and constructing the
	 * geometry surface (see retQSS::OpenVolume::add_face()).
	 *
	 * \sa retQSS::PolyhedronFace::boundary_neighbors
	 */
	inline void add_boundary_neighbor(retQSS::PolyhedronFace *neighbor)
	{
		this->boundary_neighbors.push_back(neighbor);
	}

	/**
	 * Get array of CGAL triangular subfaces of this face.
	 *
	 * \sa retQSS::PolyhedronFace::_subfaces
	 */
	inline const std::vector<FaceIdx> &subfaces() const
	{
		return this->_subfaces;
	}

	/**
	 * Get the outward normal vector to this face (i.e., pointing to the
	 * outside of the polyhedron).
	 */
	inline Vector_3 normal() const
	{
		return Vector_3(this->_nx, this->_ny, this->_nz);
	}

	/**
	 * \brief Get the centroid of this face
	 */
	inline const Point_3 &centroid() const
	{
		return this->_centroid;
	}

	/**
	 * Test if point is inside this face.
	 */
	inline bool is_point_inside(const Point_3 &point) const
	{
		return this->is_point_inside(point.x(), point.y(), point.z());
	}

	/**
	 * Test if point is on one of the edges of this face.
	 */
	inline bool is_point_on_boundary(const Point_3 &point) const
	{
		return this->is_point_on_boundary(point.x(), point.y(), point.z());
	}

	/**
	 * Test if face is contained inside this face.
	 */
	inline bool is_face_inside(const retQSS::PolyhedronFace *face) const
	{
		for(auto vertex : face->vertices)
			if(!this->is_point_inside(vertex->point()))
				return false;
		return true;
	}

	friend std::ostream &operator<<(
			std::ostream& os,
			const retQSS::PolyhedronFace& face)
	{
		std::string sep = "";

		os << "{";
		for(auto vertex: face.vertices)
		{
			os << sep << vertex->point();
			sep = ";";
		}
		os << "}";

		return os;
	};
};


/**
 * \brief Representation of 2D triangular faces
 *
 * This class specializes retQSS::PolyhedronFace with custom behavior for
 * triangular faces (in particular, tests for deciding whether points are
 * inside the face and determining ray intersections).
 */
class TriangularFace : public retQSS::PolyhedronFace
{
	friend class retQSS::Volume;
	friend class retQSS::PolyhedralVolume;
	friend class retQSS::PolyhedronFace;
	friend class retQSS::FaceNeighborhoodIterator;

protected:
	double _v01x, _v01y, _v01z;
	double _v02x, _v02y, _v02z;
	double _dot0101, _dot0102, _dot0202, _inv_denom;

	virtual void precompute_data();

	TriangularFace(retQSS::Polyhedron*, retQSS::VertexArray&);

public:
	virtual retQSS::VTKCellType vtk_cell_type() const;

	/**
	 * \brief Test if point \f$(x,y,z)\f$ is inside this face
	 *
	 * The algorithm to test point \f$P\f$ membership follows these steps:
	 *   - First, check whether \f$P\f$ is contained in the plane spanned by
	 *   this face, \f$f\f$. For this to happen, the dot product between the
	 *   face normal \f$\vec{n}\f$ and the vector \f$\vec{v} = P - o\f$ should
	 *   be zero (\f$o\f$ is a point in the plane taken as origin: the first
	 *   vertex of the face).
	 *   - Then, let \f$\vec{v_1}\f$ and \f$\vec{v_2}\f$ be vectors from the
	 *   origin to the two other vertices of \f$f\f$, and let \f$d_i\f$ be
	 *   the dot product between \f$v\f$ and \f$v_i\f$.
	 *     -# Compute
	 *     \f[a = \frac{\VecNorm{v_1}^2 \, d_2 - \left(\vec{v_1}
	 *     				\cdot \vec{v_2}\right) \, d_1}
	 *            {\VecNorm{v_1}^2 \VecNorm{v_2}^2 - \left(\vec{v_1}
	 *            		\cdot \vec{v_2}\right)^2 }\f]
	 *     -# Check that \f$a \geq 0\f$ (in case it is not, \f$P\f$ is outside
	 *     \f$f\f$).
	 *     -# Compute
	 *     \f[b = \frac{\VecNorm{v_2}^2 \, d_1 - \left(\vec{v_1}
	 *     				\cdot \vec{v_2}\right) \, d_2}
	 *            {\VecNorm{v_1}^2 \VecNorm{v_2}^2 - \left(\vec{v_1}
	 *            		\cdot \vec{v_2}\right)^2 }\f]
	 *     -# Check that \f$b \geq 0\f$ and that \f$a+b < 1\f$.
	 *
	 */
	virtual bool is_point_inside(double x, double y, double z) const;

	/**
	 * \brief Test if ray intersects this face
	 *
	 * Ray intersection is decided by means of the CGAL primitive
	 * CGAL::do_intersect(), which takes as arguments a triangle in the 3D
	 * space and a ray.
	 */
	virtual bool intersected_by(const Ray_3&);
};


/**
 * \brief Representation of 2D quadrilateral faces
 *
 * This class specializes retQSS::PolyhedronFace with custom behavior for
 * arbitrary quadrilateral, regular polygons.
 *   - Point membership is decided by splitting the quadrilateral into two
 *   triangles and testing membership on each one of them using the triangle
 *   membership algorithm as implemented in retQSS::TriangularFace.
 *   - Likewise, ray intersections are solved by testing intersections with the
 *   same two triangles.
 *
 * IMPORTANT: it is assumed that vertices are sorted clockwise or
 * anti-clockwise.
 */
class QuadrilateralFace : public retQSS::PolyhedronFace
{
	friend class retQSS::Volume;
	friend class retQSS::PolyhedralVolume;
	friend class retQSS::PolyhedronFace;
	friend class retQSS::FaceNeighborhoodIterator;

protected:
	double _v01x, _v01y, _v01z;
	double _v02x, _v02y, _v02z;
	double _v03x, _v03y, _v03z;
	double _dot0101, _dot0102, _dot0202, _dot0203, _dot0303;
	double _inv_denom1, _inv_denom2;

	virtual void precompute_data();

	QuadrilateralFace(retQSS::Polyhedron*, retQSS::VertexArray&);

public:
	virtual retQSS::VTKCellType vtk_cell_type() const;

	/**
	 * \brief Test if point \f$(x,y,z)\f$ is inside this face
	 *
	 * This makes two tests splitting the face into two triangles. See
	 * retQSS::TriangularFace::is_point_inside() for further details.
	 */
	virtual bool is_point_inside(double x,double y, double z) const;

	/**
	 * \brief Test if ray intersects this face
	 *
	 * This makes two tests splitting the face into two triangles. See
	 * retQSS::TriangularFace::intersected_by() for further details.
	 */
	virtual bool intersected_by(const Ray_3 &ray);
};

/**
 * \brief Representation of 2D rectangular faces
 *
 * This class represents special cases of quadrilateral faces where the angles
 * between the edges are right. This makes the point membership algorithm more
 * straightforward.
 */
class RectangularFace : public retQSS::QuadrilateralFace
{
	friend class retQSS::Volume;
	friend class retQSS::PolyhedralVolume;
	friend class retQSS::PolyhedronFace;
	friend class retQSS::FaceNeighborhoodIterator;

protected:
	double _v01x, _v01y, _v01z;
	double _v03x, _v03y, _v03z;
	double _dot0101, _dot0303;

	virtual void precompute_data();

	RectangularFace(retQSS::Polyhedron*, retQSS::VertexArray&);

public:
	virtual retQSS::VTKCellType vtk_cell_type() const;

	/**
	 * \brief Test if point \f$(x,y,z)\f$ is inside this face
	 *
	 * The algorithm to test point \f$P\f$ membership follows these steps:
	 *   - First, check whether \f$P\f$ is contained in the plane spanned by
	 *   this face, \f$f\f$. For this to happen, the dot product between the
	 *   face normal \f$\vec{n}\f$ and the vector \f$\vec{v} = P - o\f$ should
	 *   be zero (\f$o\f$ is a point in the plane taken as origin: the first
	 *   vertex of the face).
	 *   - Then, let \f$\vec{v_1}\f$ and \f$\vec{v_2}\f$ be vectors from the
	 *   origin to the next vertex and to the last one, respectively.
	 *     -# Compute the dot product \f$d = \vec{v_1} \cdot \vec{v}\f$ and
	 *     check that \f$0 \leq d \leq \VecNorm{v_1}^2\f$.
	 *   - Repeat last step for \f$\vec{v_2}\f$.
	 */
	virtual bool is_point_inside(double x, double y, double z) const;
};

}

#endif
