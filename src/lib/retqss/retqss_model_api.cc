#include "retqss_model_api.h"
#include "retqss_interface.hh"
#include "retqss_utilities.hh"
#include "retqss_exceptions.hh"


template<class ReturnType, class... Args>
ReturnType retQSS_execute(
		std::function<ReturnType(Args...)> retQSS_query,
		Args... args)
{
	ReturnType value;

	try
	{
		value = retQSS_query(args...);
	}
	catch(retQSS::Exception &exc)
	{
		std::cout
			<< "retQSS exception: "
			<< exc.what()
			<< std::endl;

		RETQSS()->abort();
	}

	return value;
}

#define retQSS_EXECUTE(query) \
{ \
	try\
	{\
		return query();\
	}\
	catch(retQSS::Exception &exc)\
	{\
		std::cout << "retQSS exception: " << exc.what() << std::endl;\
		RETQSS()->abort();\
	}\
}

#define retQSS_EXECUTE_VEC(query, rv, x, y, z) \
{ \
	try\
	{\
		query(rv,x,y,z);\
	}\
	catch(retQSS::Exception &exc)\
	{\
		std::cout << "retQSS exception: " << exc.what() << std::endl;\
		RETQSS()->abort();\
	}\
}

#define retQSS_EXECUTE_1(query, arg) \
{ \
	try\
	{\
		return query(arg);\
	}\
	catch(retQSS::Exception &exc)\
	{\
		std::cout << "retQSS exception: " << exc.what() << std::endl;\
		RETQSS()->abort();\
	}\
}

#define retQSS_EXECUTE_1_VEC(query, arg, rv, x, y, z) \
{ \
	try\
	{\
		query(arg,rv,x,y,z);\
	}\
	catch(retQSS::Exception &exc)\
	{\
		std::cout << "retQSS exception: " << exc.what() << std::endl;\
		RETQSS()->abort();\
	}\
}

#define retQSS_EXECUTE_2(query,arg1,arg2) \
{ \
	try\
	{\
		return query(arg1,arg2);\
	}\
	catch(retQSS::Exception &exc)\
	{\
		std::cout << "retQSS exception: " << exc.what() << std::endl;\
		RETQSS()->abort();\
	}\
}

#define retQSS_EXECUTE_2_VEC(query,arg1,arg2,rv,x,y,z) \
{ \
	try\
	{\
		query(arg1,arg2,rv,x,y,z);\
	}\
	catch(retQSS::Exception &exc)\
	{\
		std::cout << "retQSS exception: " << exc.what() << std::endl;\
		RETQSS()->abort();\
	}\
}

#define retQSS_EXECUTE_3(query,arg1,arg2,arg3) \
{ \
	try\
	{\
		return query(arg1,arg2,arg3);\
	}\
	catch(retQSS::Exception &exc)\
	{\
		std::cout << "retQSS exception: " << exc.what() << std::endl;\
		RETQSS()->abort();\
	}\
}

#define retQSS_EXECUTE_3_VEC(query,arg1,arg2,arg3,rv,x,y,z) \
{ \
	try\
	{\
		query(arg1,arg2,arg3,rv,x,y,z);\
	}\
	catch(retQSS::Exception &exc)\
	{\
		std::cout << "retQSS exception: " << exc.what() << std::endl;\
		RETQSS()->abort();\
	}\
}

#define retQSS_EXECUTE_4_VEC(query,arg1,arg2,arg3,arg4,rv,x,y,z) \
{ \
	try\
	{\
		query(arg1,arg2,arg3,arg4,rv,x,y,z);\
	}\
	catch(retQSS::Exception &exc)\
	{\
		std::cout << "retQSS exception: " << exc.what() << std::endl;\
		RETQSS()->abort();\
	}\
}

extern "C"
{

/*
 * Bootstrapping & error handling
 */

Bool retQSS_geometry_setUp(const char *geom_filename)
{
	std::function<bool(const char*)> func = RETQSS_BIND_1(geometry_set_up);

	return retQSS_execute(func, geom_filename);
}

Bool retQSS_geometry_gridSetUp(int xCount, int yCount, int zCount, double cellEdgeLength)
{

	std::function<bool(int,int,int,double)> func = RETQSS_BIND_4(geometry_grid_set_up);

	return retQSS_execute(func, xCount, yCount, zCount, cellEdgeLength);
}

Bool retQSS_particle_setUp(
		int n_particles,
		const char *model_name)
{
	std::function<bool(int,const char*,const char*)> func =
			RETQSS_BIND_3(particle_set_up);

	return retQSS_execute(func, n_particles, "", model_name);
}

Bool retQSS_particle_setUpFromFile(
		int n_particles,
		const char *ic_filename,
		const char *model_name)
{
	std::function<bool(int,const char*,const char*)> func =
			RETQSS_BIND_3(particle_set_up);

	return retQSS_execute(func, n_particles, ic_filename, model_name);
}

Bool retQSS_fullSetUp(
		int n_particles,
		const char *geom_filename,
		const char *model_name)
{
	std::function<bool(int,const char*,const char*,const char*)>
		func = RETQSS_BIND_4(full_setup);

	return retQSS_execute(func, n_particles, geom_filename, "", model_name);
}

Bool retQSS_fullSetUpWithInitialConditions(
		int n_particles,
		const char *geom_filename,
		const char *ic_filename,
		const char *model_name)
{
	std::function<bool(int,const char*,const char*,const char*)>
		func = RETQSS_BIND_4(full_setup);

	return retQSS_execute(
			func,
			n_particles,
			geom_filename,
			ic_filename,
			model_name);
}

Bool retQSS_abort()
{
	RETQSS()->abort();
	return true;
}


/*
 * General geometry queries
 */

int retQSS_geometry_countVolumes()
{
	std::function<int()>
		func = RETQSS_BIND(geometry_count_volumes);

	return retQSS_execute(func);
}

int retQSS_geometry_countVertices()
{
	std::function<int()>
		func = RETQSS_BIND(geometry_count_vertices);

	return retQSS_execute(func);
}

int retQSS_geometry_countFaces()
{
	std::function<int()>
		func = RETQSS_BIND(geometry_count_faces);

	return retQSS_execute(func);
}

VolumeID retQSS_geometry_randomVolumeID()
{
	std::function<VolumeID()>
		func = RETQSS_BIND(geometry_random_volume_ID);

	return retQSS_execute(func);
}

Bool retQSS_geometry_dumpSurface(const char *filename)
{
	std::function<bool(const char*)>
		func = RETQSS_BIND_1(geometry_dump_surface);

	return retQSS_execute(func, filename);
}

Bool retQSS_geometry_dumpVolume(VolumeID v, const char *filename)
{
	std::function<bool(VolumeID,const char*)>
		func = RETQSS_BIND_2(geometry_dump_volume);

	return retQSS_execute(func, v, filename);
}

Bool retQSS_geometry_dumpStats(const char *filename)
{
	std::function<bool(const char*)>
		func = RETQSS_BIND_1(geometry_dump_stats);

	return retQSS_execute(func, filename);
}


/*
 * Particle queries
 */

double retQSS_particle_nextCrossingTime(ParticleID p)
{
	retQSS_EXECUTE_1(
			RETQSS()->particle_next_crossing_time,
			RETQSS_TO_INTERNAL(p));
}

VolumeID retQSS_particle_currentVolumeID(ParticleID p)
{
	retQSS_EXECUTE_1(
			RETQSS()->particle_current_volume_ID,
			RETQSS_TO_INTERNAL(p));
}

VolumeID retQSS_particle_previousVolumeID(ParticleID p)
{
	std::function<VolumeID(retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_1(particle_previous_volume_ID);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(p));
}

VolumeID retQSS_particle_nextVolumeID(ParticleID p)
{
	std::function<VolumeID(retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_1(particle_next_volume_ID);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(p));
}

LogicalFaceID retQSS_particle_crossedFaceID(ParticleID p)
{
	std::function<retQSS::Internal_LogicalFaceID(retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_1(particle_crossed_face_ID);

	auto f = retQSS_execute(func, RETQSS_TO_INTERNAL(p));
	return RETQSS_TO_PUBLIC(f);
}

void retQSS_particle_crossedFaceCentroid(
		ParticleID p,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_1_VEC(
			RETQSS()->particle_crossed_face_centroid,
			RETQSS_TO_INTERNAL(p),
			&rv, x, y, z);
}

void retQSS_particle_crossedFaceNormal(
		ParticleID p,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_1_VEC(
			RETQSS()->particle_crossed_face_normal,
			RETQSS_TO_INTERNAL(p),
			&rv, x, y, z);
}

Bool retQSS_particle_hasProperty(ParticleID p, PropertyTag tag)
{
	std::function<bool(retQSS::Internal_ParticleID,PropertyTag)>
		func = RETQSS_BIND_2(particle_has_property);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(p), tag);
}

PropertyValue retQSS_particle_getProperty(ParticleID p, PropertyTag tag)
{
	std::function<PropertyValue(retQSS::Internal_ParticleID,PropertyTag)>
		func = RETQSS_BIND_2(particle_get_property);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(p), tag);
}

void retQSS_particle_getPropertyVector(
		ParticleID p,
		PropertyTag tag,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_2_VEC(
			RETQSS()->particle_get_property_vector,
			RETQSS_TO_INTERNAL(p),
			tag,
			&rv, x, y, z);
}

PropertyValue retQSS_particle_setProperty(
		ParticleID p,
		PropertyTag tag,
		PropertyValue value)
{
	std::function<PropertyValue(
				retQSS::Internal_ParticleID,
				PropertyTag,
				PropertyValue)>
		func = RETQSS_BIND_3(particle_set_property);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(p), tag, value);
}

Bool retQSS_particle_setPropertyVector(
		ParticleID p,
		PropertyTag tag,
		PropertyValue value1,
		PropertyValue value2,
		PropertyValue value3)
{
	std::function<bool(retQSS::Internal_ParticleID,PropertyTag,Vector_3)>
		func = RETQSS_BIND_3(particle_set_property_vector);

	Vector_3 values(value1,value2,value3);
	return retQSS_execute(func, RETQSS_TO_INTERNAL(p), tag, values);
}

double retQSS_particle_squaredDistanceBetween(ParticleID p1, ParticleID p2)
{
	std::function<double(
			retQSS::Internal_ParticleID,
			retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_2(particle_squared_distance_between);

	return retQSS_execute(
			func,
			RETQSS_TO_INTERNAL(p1),
			RETQSS_TO_INTERNAL(p2));
}

double retQSS_particle_angleBetween(ParticleID p1, ParticleID p2)
{
	std::function<double(
			retQSS::Internal_ParticleID,
			retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_2(particle_angle_between);

	return retQSS_execute(
			func,
			RETQSS_TO_INTERNAL(p1),
			RETQSS_TO_INTERNAL(p2));
}

void retQSS_particle_reflectiveBounce(
		ParticleID p,
		double *vx, double *vy, double *vz)
{
	int rv;
	retQSS_EXECUTE_2_VEC(
			RETQSS()->particle_reflective_bounce,
			RETQSS_TO_INTERNAL(p),
			&rv, vx, vy, vz,
			0.);
}

void retQSS_particle_reflectiveBounceWithSpeed(
		ParticleID p,
		double speed,
		double *vx, double *vy, double *vz)
{
	int rv;
	retQSS_EXECUTE_2_VEC(
			RETQSS()->particle_reflective_bounce,
			RETQSS_TO_INTERNAL(p),
			&rv, vx, vy, vz,
			speed);
}

void retQSS_particle_oppositePosition(
		ParticleID p,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_1_VEC(
			RETQSS()->particle_opposite_position,
			RETQSS_TO_INTERNAL(p),
			&rv, x, y, z);
}

VolumeID retQSS_particle_relocate(
		ParticleID p,
		double x, double y, double z,
		double vx, double vy, double vz)
{
	retQSS_EXECUTE_3(
			RETQSS()->particle_relocate,
			RETQSS_TO_INTERNAL(p),
			Point_3(x, y, z),
			Vector_3(vx, vy, vz));
}

Bool retQSS_particle_kill(ParticleID p)
{
	std::function<bool(retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_1(particle_kill);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(p));
}

Bool retQSS_particle_isAlive(ParticleID p)
{
	std::function<bool(retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_1(particle_is_alive);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(p));
}

void retQSS_particle_initialPosition(
		ParticleID p,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_1_VEC(
			RETQSS()->particle_initial_position,
			RETQSS_TO_INTERNAL(p),
			&rv, x, y, z);
}

void retQSS_particle_initialVelocity(
		ParticleID p,
		double *vx, double *vy, double *vz)
{
	int rv;
	retQSS_EXECUTE_1_VEC(
			RETQSS()->particle_initial_velocity,
			RETQSS_TO_INTERNAL(p),
			&rv, vx, vy, vz);
}

void retQSS_particle_currentPosition(
		ParticleID p,
		double *x, double *y, double *z)
{
	std::function<Point_3(retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_1(particle_current_position);

	auto pos = retQSS_execute(func, RETQSS_TO_INTERNAL(p));

	*x = pos.x();
	*y = pos.y();
	*z = pos.z();
}

void retQSS_particle_currentVelocity(
		ParticleID p,
		double *vx, double *vy, double *vz)
{
	std::function<Vector_3(retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_1(particle_current_velocity);

	auto vel = retQSS_execute(func, RETQSS_TO_INTERNAL(p));

	*vx = vel.x();
	*vy = vel.y();
	*vz = vel.z();
}

Bool retQSS_particle_dumpInitialConditions(ParticleID p, const char *filename)
{
	std::function<bool(retQSS::Internal_ParticleID,const char*)>
		func = RETQSS_BIND_2(particle_dump_initial_conditions);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(p), filename);
}

Bool retQSS_particle_dumpAllInitialConditions(const char *filename)
{
	std::function<bool(const char*)>
		func = RETQSS_BIND_1(particle_dump_all_initial_conditions);

	return retQSS_execute(func, filename);
}

Bool retQSS_particle_dumpAllCurrentValues(const char *filename, int index)
{
	retQSS_EXECUTE_2(
			RETQSS()->particle_dump_all_current_values,
			filename,
			index);
}

Bool retQSS_particle_checkPosition(ParticleID p)
{
	std::function<bool(retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_1(particle_check_position);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(p));
}

} // Functions with C++ linkage follow

Vector_3 retQSS_particle_vectorTo(ParticleID p, ParticleID q)
{
	std::function<Vector_3(
			retQSS::Internal_ParticleID,
			retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_2(particle_vector_to);

	return retQSS_execute(
			func,
			RETQSS_TO_INTERNAL(p),
			RETQSS_TO_INTERNAL(q));
}

extern "C"
{

/*
 * Face queries
 */

VolumeID retQSS_face_primaryVolumeID(LogicalFaceID f)
{
	std::function<VolumeID(retQSS::Internal_LogicalFaceID)>
		func = RETQSS_BIND_1(face_primary_volume_ID);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(f));
}

Bool retQSS_face_isOnBoundary(LogicalFaceID f)
{
	std::function<bool(retQSS::Internal_LogicalFaceID)>
		func = RETQSS_BIND_1(face_is_on_boundary);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(f));
}

void retQSS_face_normal(
		LogicalFaceID f,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_1_VEC(
			RETQSS()->face_normal,
			RETQSS_TO_INTERNAL(f),
			&rv, x, y, z);
}

void retQSS_face_centroid(
		LogicalFaceID f,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_1_VEC(
			RETQSS()->face_centroid,
			RETQSS_TO_INTERNAL(f),
			&rv, x, y, z);
}

Bool retQSS_face_isNormalTo(
		LogicalFaceID f,
		double nx, double ny, double nz)
{
	std::function<bool(retQSS::Internal_LogicalFaceID,Vector_3)>
		func = RETQSS_BIND_2(face_is_normal_to);

	return retQSS_execute(
			func,
			RETQSS_TO_INTERNAL(f),
			Vector_3(nx,ny,nz));
}

LogicalFaceID retQSS_face_oppositeFaceID(LogicalFaceID f)
{
	std::function<retQSS::Internal_LogicalFaceID(
			retQSS::Internal_LogicalFaceID)>
		func = RETQSS_BIND_1(face_opposite_face_ID);

	auto f1 = retQSS_execute(func, RETQSS_TO_INTERNAL(f));
	return RETQSS_TO_PUBLIC(f1);
}

VolumeID retQSS_face_oppositeVolumeID(LogicalFaceID f)
{
	std::function<VolumeID(retQSS::Internal_LogicalFaceID)>
		func = RETQSS_BIND_1(face_opposite_volume_ID);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(f));
}

Bool retQSS_face_hasProperty(LogicalFaceID f, PropertyTag tag)
{
	std::function<bool(retQSS::Internal_LogicalFaceID,PropertyTag)>
		func = RETQSS_BIND_2(face_has_property);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(f), tag);
}

PropertyValue retQSS_face_getProperty(LogicalFaceID f, PropertyTag tag)
{
	std::function<bool(retQSS::Internal_LogicalFaceID,PropertyTag)>
		func = RETQSS_BIND_2(face_get_property);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(f), tag);
}

void retQSS_face_getPropertyVector(
		LogicalFaceID f,
		PropertyTag tag,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_2_VEC(
			RETQSS()->face_get_property_vector,
			RETQSS_TO_INTERNAL(f),
			tag,
			&rv, x, y, z);
}

PropertyValue retQSS_face_setProperty(
		LogicalFaceID f,
		PropertyTag tag,
		PropertyValue value)
{
	std::function<PropertyValue(
			retQSS::Internal_LogicalFaceID,
			PropertyTag,
			PropertyValue)>
		func = RETQSS_BIND_3(face_set_property);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(f), tag, value);
}

Bool retQSS_face_setPropertyVector(
		LogicalFaceID f,
		PropertyTag tag,
		PropertyValue value1,
		PropertyValue value2,
		PropertyValue value3)
{
	std::function<bool(
			retQSS::Internal_LogicalFaceID,
			PropertyTag,
			Vector_3)>
		func = RETQSS_BIND_3(face_set_property_vector);

	Vector_3 values(value1,value2,value3);
	return retQSS_execute(func, RETQSS_TO_INTERNAL(f), tag, values);
}

/*
 * Volume queries
 */

int retQSS_volume_countParticlesInside(VolumeID v)
{
	std::function<int(VolumeID)>
		func = RETQSS_BIND_1(volume_count_particles_inside);

	return retQSS_execute(func, v);
}

ParticleID retQSS_volume_IDOfParticleInside(VolumeID v, Index i)
{
	std::function<retQSS::Internal_ParticleID(VolumeID,retQSS::Internal_Index)>
		func = RETQSS_BIND_2(volume_ID_of_particle_inside);

	auto p = retQSS_execute(func, v, RETQSS_TO_INTERNAL(i));
	return RETQSS_TO_PUBLIC(p);
}

void retQSS_volume_centroid(
		VolumeID v,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_1_VEC(
			RETQSS()->volume_centroid,
			v,
			&rv, x, y, z);
}

void retQSS_volume_randomPoint(
		VolumeID v,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_1_VEC(
			RETQSS()->volume_random_point,
			v,
			&rv, x, y, z);
}

VertexID retQSS_volume_vertexID(VolumeID v, Index i)
{
	std::function<retQSS::Internal_VertexID(VolumeID,retQSS::Internal_Index)>
		func = RETQSS_BIND_2(volume_vertex_ID);

	auto u = retQSS_execute(func, v, RETQSS_TO_INTERNAL(i));
	return RETQSS_TO_PUBLIC(u);
}

LogicalFaceID retQSS_volume_faceID(VolumeID v, Index i)
{
	std::function<retQSS::Internal_LogicalFaceID(
			VolumeID,
			retQSS::Internal_Index)>
		func = RETQSS_BIND_2(volume_face_ID);

	auto f = retQSS_execute(func, v, RETQSS_TO_INTERNAL(i));
	return RETQSS_TO_PUBLIC(f);
}

PropertyValue retQSS_volume_getVertexProperty(
		VolumeID v,
		Index i,
		PropertyTag tag)
{
	std::function<PropertyValue(VolumeID,retQSS::Internal_Index,PropertyTag)>
		func = RETQSS_BIND_3(volume_get_vertex_property);

	return retQSS_execute(func, v, RETQSS_TO_INTERNAL(i), tag);
}

PropertyValue retQSS_volume_setVertexProperty(
		VolumeID v,
		Index i,
		PropertyTag tag,
		PropertyValue value)
{
	std::function<PropertyValue(
			VolumeID,
			retQSS::Internal_Index,
			PropertyTag,
			PropertyValue)>
		func = RETQSS_BIND_4(volume_set_vertex_property);

	return retQSS_execute(func, v, RETQSS_TO_INTERNAL(i), tag, value);
}

void retQSS_volume_vertexCoordinates(
		VolumeID v,
		Index i,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_2_VEC(
			RETQSS()->volume_vertex_coordinates,
			v,
			RETQSS_TO_INTERNAL(i),
			&rv, x, y, z);
}

int retQSS_volume_countVertices(VolumeID v)
{
	std::function<int(VolumeID)>
		func = RETQSS_BIND_1(volume_count_vertices);

	return retQSS_execute(func, v);
}

int retQSS_volume_countFaces(VolumeID v)
{
	std::function<int(VolumeID)>
		func = RETQSS_BIND_1(volume_count_faces);

	return retQSS_execute(func, v);
}

double retQSS_volume_capacity(VolumeID v)
{
	std::function<double(VolumeID)>
		func = RETQSS_BIND_1(volume_capacity);

	return retQSS_execute(func, v);
}

Bool retQSS_volume_isOnBoundary(VolumeID v)
{
	std::function<bool(VolumeID)>
		func = RETQSS_BIND_1(volume_is_on_boundary);

	return retQSS_execute(func, v);
}

Bool retQSS_volume_hasProperty(VolumeID v, PropertyTag tag)
{
	std::function<bool(VolumeID,PropertyTag)>
		func = RETQSS_BIND_2(volume_has_property);

	return retQSS_execute(func, v, tag);
}

PropertyValue retQSS_volume_getProperty(VolumeID v, PropertyTag tag)
{
	std::function<PropertyValue(VolumeID,PropertyTag)>
		func = RETQSS_BIND_2(volume_get_property);

	return retQSS_execute(func, v, tag);
}

void retQSS_volume_getPropertyVector(
		VolumeID v,
		PropertyTag tag,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_2_VEC(
			RETQSS()->volume_get_property_vector,
			v,
			tag,
			&rv, x, y, z);
}

PropertyValue retQSS_volume_setProperty(
		VolumeID v,
		PropertyTag tag,
		PropertyValue value)
{
	std::function<PropertyValue(VolumeID,PropertyTag,PropertyValue)>
		func = RETQSS_BIND_3(volume_set_property);

	return retQSS_execute(func, v, tag, value);
}

Bool retQSS_volume_setPropertyVector(
		VolumeID v,
		PropertyTag tag,
		PropertyValue value1,
		PropertyValue value2,
		PropertyValue value3)
{
	std::function<bool(VolumeID,PropertyTag,Vector_3)>
		func = RETQSS_BIND_3(volume_set_property_vector);

	Vector_3 values(value1,value2,value3);
	return retQSS_execute(func, v, tag, values);
}

double retQSS_volume_distanceToPoint(VolumeID v, double x, double y, double z)
{
	std::function<double(VolumeID,Point_3)>
		func = RETQSS_BIND_2(volume_distance_to_point);

	Point_3 point(x,y,z);
	return retQSS_execute(func, v, point);
}

void retQSS_volume_rayIntersection(
		VolumeID v,
		double x, double y, double z,
		double dx, double dy, double dz,
		double *ix, double *iy, double *iz)
{
	Point_3 point(x,y,z);
	Vector_3 dir(dx,dy,dz);

	int rv;
	retQSS_EXECUTE_2_VEC(
			RETQSS()->volume_ray_intersection,
			v,
			Ray_3(point,dir),
			&rv, ix, iy, iz);
}

VolumeID retQSS_volume_adjacentVolumeID(
		VolumeID v,
		double x, double y, double z,
		double dx, double dy, double dz)
{
	std::function<VolumeID(VolumeID,Ray_3)>
		func = RETQSS_BIND_2(volume_adjacent_volume_ID);

	Point_3 point(x,y,z);
	Vector_3 dir(dx,dy,dz);

	return retQSS_execute(func, v, Ray_3(point,dir));
}


/*
 * Vertex queries
 */

Bool retQSS_vertex_hasProperty(VertexID u, PropertyTag tag)
{
	std::function<bool(retQSS::Internal_VertexID,PropertyTag)>
		func = RETQSS_BIND_2(vertex_has_property);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(u), tag);
}

PropertyValue retQSS_vertex_getProperty(VertexID u, PropertyTag tag)
{
	std::function<PropertyValue(retQSS::Internal_VertexID,PropertyTag)>
		func = RETQSS_BIND_2(vertex_get_property);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(u), tag);
}

void retQSS_vertex_getPropertyVector(
		VertexID u,
		PropertyTag tag,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_2_VEC(
			RETQSS()->vertex_get_property_vector,
			RETQSS_TO_INTERNAL(u),
			tag,
			&rv, x, y, z);
}

PropertyValue retQSS_vertex_setProperty(
		VertexID u,
		PropertyTag tag,
		PropertyValue value)
{
	std::function<PropertyValue(
				retQSS::Internal_VertexID,
				PropertyTag,
				PropertyValue)>
		func = RETQSS_BIND_3(vertex_set_property);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(u), tag, value);
}

Bool retQSS_vertex_setPropertyVector(
		VertexID u,
		PropertyTag tag,
		PropertyValue value1,
		PropertyValue value2,
		PropertyValue value3)
{
	std::function<bool(retQSS::Internal_VertexID,PropertyTag,Vector_3)>
		func = RETQSS_BIND_3(vertex_set_property_vector);

	Vector_3 values(value1,value2,value3);
	return retQSS_execute(func, RETQSS_TO_INTERNAL(u), tag, values);
}

void retQSS_vertex_coordinates(
		VertexID u,
		double *x, double *y, double *z)
{
	int rv;
	retQSS_EXECUTE_1_VEC(
			RETQSS()->vertex_coordinates,
			RETQSS_TO_INTERNAL(u),
			&rv, x, y, z);
}


/*
 * Neighborhood queries
 */

Bool retQSS_volumeNeighborhood_toDefault()
{
	std::function<bool()>
		func = RETQSS_BIND(volume_neighborhood_to_default);

	return retQSS_execute(func);
}

Bool retQSS_volumeNeighborhood_toFile(const char *filename)
{
	std::function<bool(const char*)>
		func = RETQSS_BIND_1(volume_neighborhood_to_file);

	return retQSS_execute(func, filename);
}

Bool retQSS_volumeNeighborhood_toRadial(double radius)
{
	std::function<bool(double)>
		func = RETQSS_BIND_1(volume_neighborhood_to_radial);

	return retQSS_execute(func, radius);
}

Bool retQSS_volumeNeighborhood_toPeriodicRadial(double radius)
{
	std::function<bool(double)>
		func = RETQSS_BIND_1(volume_neighborhood_to_periodic_radial);

	return retQSS_execute(func, radius);
}

Bool retQSS_volumeNeighborhood_toVertexSharing()
{
	std::function<bool()>
		func = RETQSS_BIND(volume_neighborhood_to_vertex_sharing);

	return retQSS_execute(func);
}

Bool retQSS_volumeNeighborhood_precomputeAll()
{
	std::function<bool()>
		func = RETQSS_BIND(volume_neighborhood_precompute_all);

	return retQSS_execute(func);
}

VolumeID retQSS_volumeNeighborhood_neighborID(VolumeID v, Index i)
{
	std::function<VolumeID(VolumeID,retQSS::Internal_Index)>
		func = RETQSS_BIND_2(volume_neighborhood_neighbor_ID);

	return retQSS_execute(func, v, RETQSS_TO_INTERNAL(i));
}

int retQSS_volumeNeighborhood_countVolumes(VolumeID v)
{
	std::function<int(VolumeID)>
		func = RETQSS_BIND_1(volume_neighborhood_count_volumes);

	return retQSS_execute(func, v);
}

PropertyValue retQSS_volumeNeighborhood_getNeighborProperty(
		VolumeID v,
		Index i,
		PropertyTag tag)
{
	std::function<PropertyValue(VolumeID,retQSS::Internal_Index,PropertyTag)>
		func = RETQSS_BIND_3(volume_neighborhood_get_neighbor_property);

	return retQSS_execute(func, v, RETQSS_TO_INTERNAL(i), tag);
}

PropertyValue retQSS_volumeNeighborhood_setProperty(
		VolumeID v,
		PropertyTag tag,
		PropertyValue value)
{
	std::function<PropertyValue(VolumeID,PropertyTag,PropertyValue)>
		func = RETQSS_BIND_3(volume_neighborhood_set_property);

	return retQSS_execute(func, v, tag, value);
}

int retQSS_particleNeighborhood_countParticles(ParticleID p)
{
	std::function<int(retQSS::Internal_ParticleID)>
		func = RETQSS_BIND_1(particle_neighborhood_count_particles);

	return retQSS_execute(func, RETQSS_TO_INTERNAL(p));
}

void retQSS_particleNeighborhood_averagePosition(
		ParticleID p,
		double *count, double *x, double *y, double *z)
{
	retQSS_EXECUTE_1_VEC(
			RETQSS()->particle_neighborhood_average_position,
			RETQSS_TO_INTERNAL(p),
			count, x, y, z);
}

void retQSS_particleNeighborhood_averageVelocity(
		ParticleID p,
		double *count, double *x, double *y, double *z)
{
	retQSS_EXECUTE_1_VEC(
			RETQSS()->particle_neighborhood_average_velocity,
			RETQSS_TO_INTERNAL(p),
			count, x, y, z);
}

void retQSS_particleNeighborhood_repulsiveDirection(
		ParticleID p,
		double *count, double *x, double *y, double *z)
{
	retQSS_EXECUTE_1_VEC(
			RETQSS()->particle_neighborhood_repulsive_direction,
			RETQSS_TO_INTERNAL(p),
			count, x, y, z);
}

void retQSS_particleNeighborhood_forEachParticle(
		ParticleID p,
		const char *function,
		double *count, double *x, double *y, double *z)
{
	std::vector<double> args;

	retQSS_EXECUTE_3_VEC(
			RETQSS()->particle_neighborhood_for_each_particle,
			RETQSS_TO_INTERNAL(p),
			function,
			args,
			count, x, y, z);
}

void retQSS_particleNeighborhood_forEachParticle_1(
		ParticleID p,
		const char *function,
		double arg1,
		double *count, double *x, double *y, double *z)
{
	std::vector<double> args {arg1};

	retQSS_EXECUTE_3_VEC(
			RETQSS()->particle_neighborhood_for_each_particle,
			RETQSS_TO_INTERNAL(p),
			function,
			args,
			count, x, y, z);
}

void retQSS_particleNeighborhood_forEachParticle_2(
		ParticleID p,
		const char *function,
		double arg1,
		double arg2,
		double *count, double *x, double *y, double *z)
{
	std::vector<double> args {arg1, arg2};

	retQSS_EXECUTE_3_VEC(
			RETQSS()->particle_neighborhood_for_each_particle,
			RETQSS_TO_INTERNAL(p),
			function,
			args,
			count, x, y, z);
}

void retQSS_particleNeighborhood_evaluate_1(
		ParticleID p,
		const char *function,
		double arg1,
		double *x, double *y, double *z)
{
	double count;

	retQSS_EXECUTE_3_VEC(
			RETQSS()->particle_neighborhood_evaluate_1,
			RETQSS_TO_INTERNAL(p),
			function,
			arg1,
			&count, x, y, z);
}

/*
 * Point & vector queries
 */

void retQSS_vectorWithNorm(
		double vx, double vy, double vz,
		double norm,
		double *ux, double *uy, double *uz)
{
	int rv;
	retQSS_EXECUTE_4_VEC(
			RETQSS()->vector_with_norm,
			vx, vy, vz,
			norm,
			&rv, ux, uy, uz);
}

void retQSS_randomVector(double *vx, double *vy, double *vz)
{
	int rv;
	retQSS_EXECUTE_VEC(
			RETQSS()->random_vector,
			&rv, vx, vy, vz);
}

void retQSS_randomVectorWithNorm(
		double norm,
		double *vx, double *vy, double *vz)
{
	int rv;
	retQSS_EXECUTE_1_VEC(
			RETQSS()->random_vector_with_norm,
			norm,
			&rv, vx, vy, vz);
}


/*
 * Miscellaneous queries
 */

Bool retQSS_random_reseed(int seed)
{
	return retQSS::random_reseed(seed);
}

double retQSS_random(double from, double to)
{
	return retQSS::random_double(from, to);
}

double retQSS_random_lognormal(double m, double s)
{
	return retQSS::random_lognormal(m, s);
}

double retQSS_random_normal(double m, double s)
{
	return retQSS::random_normal(m, s);
}

int retQSS_modulus(int a, int b)
{
	return a%b;
}

int retQSS_div(int a, int b)
{
	return a/b;
}

double retQSS_max(double a, double b)
{
	return std::max(a,b);
}

double retQSS_min(double a, double b)
{
	return std::min(a,b);
}


/*
 * Engine interface
 */

void retQSS_clear()
{
	retQSS::Interface::clear_instance();
}

}
