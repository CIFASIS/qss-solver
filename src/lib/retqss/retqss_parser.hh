/**
 * \file retqss_parser.hh
 */

#ifndef _RETQSS_PARSER_H_
#define _RETQSS_PARSER_H_

#include <fstream>
#include <vector>
#include <list>
#include <set>

#include "retqss_polyhedral_volume.hh"
#include "retqss_polyhedron_face.hh"
#include "retqss_volume.hh"
#include "retqss_aabb_tree.hh"
#include "retqss_types.hh"


namespace retQSS
{

class Geometry;

/**
 * \brief Parser of geometry files
 *
 * The geometry parser is responsible for reading and interpreting files
 * describing tessellated tridimensional domains conforming a 3D mesh of convex
 * polyhedrons. This geometrical information is then materialized in an
 * instance of retQSS::Geometry. In fact, for each input polyhedron, the parser
 * creates a low-level CGAL object (retQSS::Polyhedron) and feeds it into the
 * geometry, which in turn instanciates a \em volume (retQSS::Volume). A volume
 * hides away the low-level 3D aspects of the underlying polyhedron and adds
 * complementary behavior about adjacent volumes, among other things.
 *
 * \paragraph formats Supported file formats
 *
 * Currently, retQSS supports VTK (in particular, unstructured grids composed
 * of tetrahedrons, voxels and/or hexahedrons) and OFF (a single text file
 * pointing to the source OFF file of each polyhedron).
 */
class GeometryParser
{
protected:
	std::string path, filename;

	/**
	 * Unmatched faces extracted from the geometry (i.e., those faces not
	 * shared by two polyhedrons).
	 */
	std::list<retQSS::PolyhedronFace*> unmatched_faces;

	/**
	 * AABB tree containing the unmatched faces that is used to classify them
	 * as internal or boundary (see
	 * retQSS::GeometryParser::classify_unmatched_faces()).
	 */
	retQSS::AABBTreeForMultiplePolyhedrons tree;

	/**
	 * \brief Extract unmatched faces from volume
	 *
	 * After parsing the geometry, there will be some faces for which the
	 * parser could not find a corresponding face in some other volume. This
	 * method scans a given volume for these unmatched faces, which will be
	 * post-processed in order to determine whether they are boundary faces
	 * or if there are "smaller" faces in neighboring volumes that match it
	 * completely.
	 */
	void extract_unmatched_faces(retQSS::PolyhedralVolume *volume);

	/**
	 * \brief Classify unmatched faces as boundary or internal
	 *
	 * Unmatched faces extracted from the volumes that compose the geometry
	 * can be internal faces or boundary faces. The algorithm to decide the
	 * type of each face follows these steps:
	 *  - For each unmatched face \f$f\f$, compute its outward normal vector
	 *  \f$\vec{n}\f$ and its centroid \f$C\f$, and a point
	 *  \f$P = C - e\vec{n}\f$ inside the corresponding volume, where
	 *  \f$e\f$ is "small".
	 *  - Use the AABB tree to compute the first face \f$f'\f$ (other than
	 *  \f$f\f$) crossed by the ray \f$r\f$ with origin \f$P\f$ and direction
	 *  \f$\vec{n}\f$.
	 *  - If there is no such face, then \f$f\f$ is a boundary face and we
	 *  stop. We also stop if the crossed face \f$f'\f$ belongs to the same
	 *  volume of \f$f\f$.
	 *  - Let \f$P'\f$ be the intersection point of \f$r\f$ and the crossed
	 *  face \f$f'\f$. If the distance between \f$P\f$ and \f$P'\f$ is "large",
	 *  then \f$f\f$ is a boundary face (\f$r\f$ intersects the geometry far
	 *  away from \f$f\f$).
	 *  - Otherwise, \f$f\f$ and \f$f'\f$ are both internal and neighboring
	 *  faces. If \f$f'\f$ lies completely inside \f$f\f$, then \f$f'\f$ has no
	 *  further neighbors and so we can skip it in future iterations.
	 */
	void classify_unmatched_faces(retQSS::Geometry *geometry);

	/**
	 * \brief Read geometry from input stream
	 *
	 * Read polyhedrons from the given input stream and add them to the
	 * geometry.
	 */
	virtual void do_parse(
			std::ifstream &input,
			retQSS::Geometry *geometry) = 0;

	/**
	 * \brief Parser constructor
	 *
	 * Instanciates a parser with the path to the geometry file/s and the main
	 * filename that will be read.
	 */
	GeometryParser(
			const std::string &path,
			const std::string &filename);

public:
	virtual ~GeometryParser() {};

	/**
	 * \brief Parse input file and populate geometry
	 *
	 * Parse geometry file provided when instanciating the parser and populate
	 * geometry object.
	 */
	void parse(retQSS::Geometry *geometry);

	/**
	 * \brief Create geometry parser for file
	 *
	 * Factory method to create a proper parser for the given file. It uses the
	 * filename extension to decide which subclass will be instanciated.
	 */
	static retQSS::GeometryParser *build_from(const char *filename);
};

class OFFParser : public retQSS::GeometryParser
{
private:
	bool single_file; //!< Whether a single OFF file should be read.

	/**
	 * \brief Add new volume to the geometry
	 *
	 * Read a polyhedron from the given file and add a new volume to the
	 * geometry supported by this polyhedron.
	 */
	retQSS::PolyhedralVolume *new_volume(
			const std::string &filename,
			retQSS::Geometry *geometry);

protected:
	/**
	 * \brief Read geometry from input stream
	 *
	 * Read polyhedrons from the given input stream and add them to the
	 * geometry.
	 */
	virtual void do_parse(
			std::ifstream &input,
			retQSS::Geometry *geometry);

public:
	/**
	 * \brief Parser constructor
	 *
	 * Instanciates a parser with the path to the geometry file/s and the main
	 * filename that will be read.
	 */
	OFFParser(
			const std::string &path,
			const std::string &filename,
			bool single_file =false) :
				retQSS::GeometryParser(path, filename),
				single_file(single_file) {};

};

class VTKParser : public retQSS::GeometryParser
{
private:
	typedef std::unordered_map<
				std::string,
				std::vector<
					std::vector<double>
				>
			> cell_data_t;

	std::set<retQSS::VTKCellType> supported_cells; //!< VTK cells currently
	                                               //!< supported.

	/**
	 * \brief Build geometry from parsed data
	 *
	 * Build geometry using points and cells parsed from the input file. This
	 * is typically the last action of retQSS::VTKParser::do_parse() (if no
	 * initial volume properties are defined in the file).
	 * For each cell, an appropriate polyhedron is created and a new volume is
	 * then added to the geometry.
	 */
	void build_geometry(
			const std::vector<Point_3> &points,
			const std::vector<std::vector<size_t>> &cells,
			retQSS::Geometry *geometry);

	/**
	 * \brief Set static volume properties provided in the input file
	 *
	 * If cell data is included in the input file, assign it as properties in
	 * the corresponding volumes.
	 */
	void assign_data(
			const cell_data_t &properties,
			retQSS::Geometry *geometry);

	/**
	 * \brief Create new tetrahedron
	 *
	 * Create new tetrahedron represented by the given VTK cell. Indices in
	 * this cell are mapped to vertex coordinates using the point vector, and
	 * then the polygonal faces are added to the polyhedron. Finally, add a new
	 * volume to the geometry and check whether the faces correspond to
	 * previously parsed volumes, which will become adjacent neighbors of this
	 * new volume.
	 */
	retQSS::PolyhedralVolume *build_tetrahedron(
			const std::vector<Point_3> &points,
			std::vector<size_t> &cell,
			std::unordered_map<std::string, retQSS::Volume*> &neighbors,
			retQSS::Geometry *geometry);

	/**
	 * \brief Create new voxel
	 *
	 * \sa retQSS::VTKParser::build_tetrahedron
	 */
	retQSS::PolyhedralVolume *build_voxel(
			const std::vector<Point_3>&,
			std::vector<size_t>&,
			std::unordered_map<std::string, retQSS::Volume*>&,
			retQSS::Geometry*);

	/**
	 * \brief Create new hexahedron
	 *
	 * \sa retQSS::VTKParser::build_tetrahedron
	 */
	retQSS::PolyhedralVolume *build_hexahedron(
			const std::vector<Point_3>&,
			std::vector<size_t>&,
			std::unordered_map<std::string, retQSS::Volume*>&,
			retQSS::Geometry*);

	/**
	 * \brief Add volume as adjacent neighbor of previously parsed volumes
	 *
	 * Add current volume as an adjacent neighbor of previously parsed volumes,
	 * using for this purpose the given face (represented by the VTK indices
	 * of its vertices). The <tt>neighbors</tt> map is used to retrieve the
	 * volume in which this face was already seen.
	 */
	void add_neighbor(
			retQSS::Volume *volume,
			std::unordered_map<std::string, retQSS::Volume*> &neighbors,
			std::vector<size_t> &face) const;
protected:
	/**
	 * \brief Read geometry from input stream
	 *
	 * Read polyhedrons from the given input stream and add them to the
	 * geometry.
	 */
	virtual void do_parse(
			std::ifstream &input,
			retQSS::Geometry *geometry);

public:
	/**
	 * \brief Parser constructor
	 *
	 * Instanciates a parser with the path to the geometry file/s (ignored by
	 * the VTK parser) and the main filename that will be read.
	 */
	VTKParser(
			const std::string &path,
			const std::string &filename);

};

}

#endif
