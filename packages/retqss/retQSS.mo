package retQSS

  // Bootstrapping & error handling

  function geometry_setUp
    input String geom_filename;
    output Real status;
    external "C" status=retQSS_geometry_setUp(geom_filename) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end geometry_setUp;

  function particle_setUp
    input Integer n_particles;
    input String model_name;
    output Real status;
    external "C" status=retQSS_particle_setUp(n_particles,model_name) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_setUp;

  function particle_setUpFromFile
    input Integer n_particles;
    input String ic_filename;
    input String model_name;
    output Real status;
    external "C" status=retQSS_particle_setUpFromFile(n_particles,ic_filename,model_name) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_setUpFromFile;

  function retQSS_setUp
    input Integer n_particles;
    input String geom_filename;
    input String model_name;
    output Real status;
    external "C" status=retQSS_fullSetUp(n_particles,geom_filename,model_name) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end retQSS_setUp;

  function retQSS_setUpWithInitialConditions
    input Integer n_particles;
    input String geom_filename;
    input String ic_filename;
    input String model_name;
    output Real status;
    external "C" status=retQSS_fullSetUpWithInitialConditions(n_particles,geom_filename,ic_filename,model_name) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end retQSS_setUpWithInitialConditions;

  function retQSS_fail
    output Integer ret;
    external "C" ret=retQSS_abort() annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end retQSS_fail;


  // General geometry queries

  function geometry_countVolumes
    output Integer n;
    external "C" n=retQSS_geometry_countVolumes() annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end geometry_countVolumes;

  function geometry_countVertices
    output Integer n;
    external "C" n=retQSS_geometry_countVertices() annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end geometry_countVertices;

  function geometry_countFaces
    output Integer n;
    external "C" n=retQSS_geometry_countFaces() annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end geometry_countFaces;

  function geometry_randomVolumeID
    output Integer v;
    external "C" v=retQSS_geometry_randomVolumeID() annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end geometry_randomVolumeID;

  function geometry_dumpSurface
    input String filename;
    output Boolean status;
    external "C" status=retQSS_geometry_dumpSurface(filename) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end geometry_dumpSurface;

  function geometry_dumpVolume
    input Integer v;
    input String filename;
    output Boolean status;
    external "C" status=retQSS_geometry_dumpVolume(v,filename) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end geometry_dumpVolume;

  function geometry_dumpStats
    input String filename;
    output Boolean status;
    external "C" status=retQSS_geometry_dumpStats(filename) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end geometry_dumpStats;


  // Particle queries

  function particle_nextCrossingTime
    input Integer p;
    input Real x;
    input Real y;
    input Real z;
    input Real vx;
    input Real vy;
    input Real vz;
    output Real t;
    external "C" t=retQSS_particle_nextCrossingTime(p) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_nextCrossingTime;

  function particle_currentVolumeID
    input Integer p;
    output Integer v;
    external "C" v=retQSS_particle_currentVolumeID(p) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_currentVolumeID;

  function particle_previousVolumeID
    input Integer p;
    output Integer v;
    external "C" v=retQSS_particle_previousVolumeID(p) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_previousVolumeID;

  function particle_nextVolumeID
    input Integer p;
    output Integer v;
    external "C" v=retQSS_particle_nextVolumeID(p) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_nextVolumeID;

  function particle_relocate
    input Integer p;
    input Real x;
    input Real y;
    input Real z;
    input Real vx;
    input Real vy;
    input Real vz;
    output Integer v;
    external "C" v=retQSS_particle_relocate(p,x,y,z,vx,vy,vz) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_relocate;

  function particle_kill
    input Integer p;
    output Boolean status;
    external "C" status=retQSS_particle_kill(p) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_kill;

  function particle_isAlive
    input Integer p;
    output Boolean status;
    external "C" status=retQSS_particle_isAlive(p) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_isAlive;

  function particle_crossedFaceID
    input Integer p;
    output Integer f;
    external "C" f=retQSS_particle_crossedFaceID(p) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_crossedFaceID;

  function particle_crossedFaceCentroid
    input Integer p;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particle_crossedFaceCentroid(p,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_crossedFaceCentroid;

  function particle_crossedFaceNormal
    input Integer p;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particle_crossedFaceNormal(p,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_crossedFaceNormal;

  function particle_initialPosition
    input Integer p;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particle_initialPosition(p,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_initialPosition;

  function particle_initialVelocity
    input Integer p;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particle_initialVelocity(p,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_initialVelocity;

  function particle_hasProperty
    input Integer p;
    input String tag;
    output Integer val;
    external "C" val=retQSS_particle_hasProperty(p, tag) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_hasProperty;

  function particle_getProperty
    input Integer p;
    input String tag;
    output Real val;
    external "C" val=retQSS_particle_getProperty(p, tag) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_getProperty;

  function particle_getPropertyVector
    input Integer p;
    input String tag;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particle_getPropertyVector(p,tag,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_getPropertyVector;

  function particle_setProperty
    input Integer p;
    input String tag;
    input Real val;
    output Real rval;
    external "C" rval=retQSS_particle_setProperty(p, tag, val) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_setProperty;

  function particle_setPropertyVector
    input Integer p;
    input String tag;
    input Real value1;
    input Real value2;
    input Real value3;
    output Boolean status;
    external "C" status=retQSS_particle_setPropertyVector(p, tag, value1, value2, value3) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_setPropertyVector;

  function particle_squaredDistanceBetween
    input Integer p1;
    input Integer p2;
    output Real dist;
    external "C" dist=retQSS_particle_squaredDistanceBetween(p1, p2) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_squaredDistanceBetween;

  function particle_angleBetween
    input Integer p1;
    input Integer p2;
    output Real angle;
    external "C" angle=retQSS_particle_angleBetween(p1, p2) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_angleBetween;

  function particle_checkPosition
    input Integer p;
    output Boolean status;
    external "C" status=retQSS_particle_checkPosition(p) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_checkPosition;

  function particle_reflectiveBounce
    input Integer p;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particle_reflectiveBounce(p,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_reflectiveBounce;

  function particle_reflectiveBounceWithSpeed
    input Integer p;
    input Real speed;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particle_reflectiveBounceWithSpeed(p,speed,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_reflectiveBounceWithSpeed;

  function particle_oppositePosition
    input Integer p;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particle_oppositePosition(p,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_oppositePosition;

  function particle_dumpInitialConditions
    input Integer p;
    input String filename;
    output Boolean status;
    external "C" status=retQSS_particle_dumpInitialConditions(p,filename) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_dumpInitialConditions;

  function particle_dumpAllInitialConditions
    input String filename;
    output Boolean status;
    external "C" status=retQSS_particle_dumpAllInitialConditions(filename) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_dumpAllInitialConditions;

  function particle_dumpAllCurrentValues
    input String filename;
    input Integer index;
    output Boolean status;
    external "C" status=retQSS_particle_dumpAllCurrentValues(filename,index) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particle_dumpAllCurrentValues;


  // Face queries

  function face_primaryVolumeID
    input Integer f;
    output Integer v;
    external "C" v=retQSS_face_primaryVolumeID(f) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_primaryVolumeID;

  function face_isOnBoundary
    input Integer f;
    output Boolean status;
    external "C" status=retQSS_face_isOnBoundary(f) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_isOnBoundary;

  function face_normal
    input Integer f;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_face_normal(f,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_normal;

  function face_centroid
    input Integer f;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_face_centroid(f,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_centroid;

  function face_isNormalTo
    input Integer f;
    input Real nx;
    input Real ny;
    input Real nz;
    output Boolean status;
    external "C" status=retQSS_face_isNormalTo(f,nx,ny,nz) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_isNormalTo;

  function face_oppositeFaceID
    input Integer f;
    output Integer f1;
    external "C" f1=retQSS_face_oppositeFaceID(f) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_oppositeFaceID;

  function face_oppositeVolumeID
    input Integer f;
    output Integer v;
    external "C" v=retQSS_face_oppositeVolumeID(f) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_oppositeVolumeID;

  function face_hasProperty
    input Integer f;
    input String tag;
    output Boolean val;
    external "C" val=retQSS_face_hasProperty(f, tag) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_hasProperty;

  function face_getProperty
    input Integer f;
    input String tag;
    output Real val;
    external "C" val=retQSS_face_getProperty(f, tag) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_getProperty;

  function face_getPropertyVector
    input Integer f;
    input String tag;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_face_getPropertyVector(f,tag,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_getPropertyVector;

  function face_setProperty
    input Integer f;
    input String tag;
    input Real val;
    output Real rval;
    external "C" rval=retQSS_face_setProperty(f, tag, val) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_setProperty;

  function face_setPropertyVector
    input Integer f;
    input String tag;
    input Real value1;
    input Real value2;
    input Real value3;
    output Boolean status;
    external "C" status=retQSS_face_setPropertyVector(f, tag, value1, value2, value3) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end face_setPropertyVector;


  // Volume queries

  function volume_countParticlesInside
    input Integer v;
    output Integer n;
    external "C" n=retQSS_volume_countParticlesInside(v) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_countParticlesInside;

  function volume_IDOfParticleInside
    input Integer v;
    input Integer i;
    output Integer p;
    external "C" p=retQSS_volume_IDOfParticleInside(v, i) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_IDOfParticleInside;

  function volume_centroid
    input Integer v;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_volume_centroid(v,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_centroid;

  function volume_capacity
    input Integer v;
    output Real cap;
    external "C" cap=retQSS_volume_capacity(v) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_capacity;

  function volume_countVertices
    input Integer v;
    output Integer n;
    external "C" n=retQSS_volume_countVertices(v) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_countVertices;

  function volume_countFaces
    input Integer v;
    output Integer n;
    external "C" n=retQSS_volume_countFaces(v) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_countFaces;

  function volume_vertexID
    input Integer v;
    input Integer i;
    output Integer u;
    external "C" u=retQSS_volume_vertexID(v, i) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_vertexID;

  function volume_faceID
    input Integer v;
    input Integer i;
    output Integer f;
    external "C" f=retQSS_volume_faceID(v, i) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_faceID;

  function volume_getVertexProperty
    input Integer v;
    input Integer i;
    input String tag;
    output Real val;
    external "C" val=retQSS_volume_getVertexProperty(v, i, tag) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_getVertexProperty;

  function volume_setVertexProperty
    input Integer v;
    input Integer i;
    input String tag;
    input Real val;
    output Real rval;
    external "C" rval=retQSS_volume_setVertexProperty(v, i, tag, val) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_setVertexProperty;

  function volume_vertexCoordinates
    input Integer v;
    input Integer i;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_volume_vertexCoordinates(v,i,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_vertexCoordinates;

  function volume_isOnBoundary
    input Integer v;
    output Boolean val;
    external "C" val=retQSS_volume_isOnBoundary(v) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_isOnBoundary;

  function volume_hasProperty
    input Integer v;
    input String tag;
    output Boolean val;
    external "C" val=retQSS_volume_hasProperty(v, tag) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_hasProperty;

  function volume_getProperty
    input Integer v;
    input String tag;
    output Real val;
    external "C" val=retQSS_volume_getProperty(v, tag) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_getProperty;

  function volume_getPropertyVector
    input Integer v;
    input String tag;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_volume_getPropertyVector(v,tag,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_getPropertyVector;

  function volume_setProperty
    input Integer v;
    input String tag;
    input Real val;
    output Real rval;
    external "C" rval=retQSS_volume_setProperty(v, tag, val) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_setProperty;

  function volume_setPropertyVector
    input Integer v;
    input String tag;
    input Real value1;
    input Real value2;
    input Real value3;
    output Boolean status;
    external "C" status=retQSS_volume_setPropertyVector(v, tag, value1, value2, value3) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_setPropertyVector;

  function volume_distanceToPoint
    input Integer v;
    input Real x;
    input Real y;
    input Real z;
    output Real dist;
    external "C" dist=retQSS_volume_distanceToPoint(v, x, y, z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_distanceToPoint;

  function volume_rayIntersection
    input Integer v;
    input Real x;
    input Real y;
    input Real z;
    input Real dx;
    input Real dy;
    input Real dz;
    output Real ix;
    output Real iy;
    output Real iz;
    external "C" retQSS_volume_rayIntersection(v, x, y, z, dx, dy, dz, ix, iy, iz) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_rayIntersection;

  function volume_adjacentVolumeID
    input Integer v;
    input Real x;
    input Real y;
    input Real z;
    input Real dx;
    input Real dy;
    input Real dz;
    output Integer v1;
    external "C" v1=retQSS_volume_adjacentVolumeID(v, x, y, z, dx, dy, dz) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_adjacentVolumeID;


  // Vertex queries

  function vertex_hasProperty
    input Integer u;
    input String tag;
    output Integer val;
    external "C" val=retQSS_vertex_hasProperty(u, tag) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end vertex_hasProperty;

  function vertex_getProperty
    input Integer u;
    input String tag;
    output Real val;
    external "C" val=retQSS_vertex_getProperty(u, tag) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end vertex_getProperty;

  function vertex_getPropertyVector
    input Integer u;
    input String tag;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_vertex_getPropertyVector(u,tag,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end vertex_getPropertyVector;

  function vertex_setProperty
    input Integer u;
    input String tag;
    input Real val;
    output Real rval;
    external "C" rval=retQSS_vertex_setProperty(u, tag, val) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end vertex_setProperty;

  function vertex_setPropertyVector
    input Integer u;
    input String tag;
    input Real value1;
    input Real value2;
    input Real value3;
    output Boolean status;
    external "C" status=retQSS_vertex_setPropertyVector(u, tag, value1, value2, value3) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end vertex_setPropertyVector;

  function vertex_coordinates
    input Integer u;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_vertex_coordinates(u,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end vertex_coordinates;


  // Neighborhood queries

  function volumeNeighborhood_toDefault
    output Integer status;
    external "C" status=retQSS_volumeNeighborhood_toDefault() annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volumeNeighborhood_toDefault;

  function volumeNeighborhood_toFile
    input String filename;
    output Integer status;
    external "C" status=retQSS_volumeNeighborhood_toFile(filename) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volumeNeighborhood_toFile;

  function volumeNeighborhood_toRadial
    input Real radius;
    output Integer status;
    external "C" status=retQSS_volumeNeighborhood_toRadial(radius) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volumeNeighborhood_toRadial;

  function volumeNeighborhood_toPeriodicRadial
    input Real radius;
    output Integer status;
    external "C" status=retQSS_volumeNeighborhood_toPeriodicRadial(radius) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volumeNeighborhood_toPeriodicRadial;

  function volumeNeighborhood_toVertexSharing
    output Integer status;
    external "C" status=retQSS_volumeNeighborhood_toVertexSharing() annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volumeNeighborhood_toVertexSharing;

  function volumeNeighborhood_precomputeAll
    output Integer status;
    external "C" status=retQSS_volumeNeighborhood_precomputeAll() annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volumeNeighborhood_precomputeAll;

  function volumeNeighborhood_countVolumes
    input Integer v;
    output Integer n;
    external "C" n=retQSS_volumeNeighborhood_countVolumes(v) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volumeNeighborhood_countVolumes;

  function volumeNeighborhood_getNeighborProperty
    input Integer v;
    input Integer i;
    input String tag;
    output Real val;
    external "C" val=retQSS_volumeNeighborhood_getNeighborProperty(v, i, tag) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volumeNeighborhood_getNeighborProperty;

  function volumeNeighborhood_setProperty
    input Integer v;
    input String tag;
    input Real val;
    output Real rval;
    external "C" rval=retQSS_volumeNeighborhood_setProperty(v, tag, val) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volumeNeighborhood_setProperty;

  function volumeNeighborhood_neighborID
    input Integer v;
    input Integer i;
    output Integer w;
    external "C" w=retQSS_volumeNeighborhood_neighborID(v, i) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volumeNeighborhood_neighborID;

  function particleNeighborhood_countParticles
    input Integer p;
    output Integer n;
    external "C" n=retQSS_particleNeighborhood_countParticles(p) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particleNeighborhood_countParticles;

  function particleNeighborhood_averagePosition
    input Integer p;
    output Integer count;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particleNeighborhood_averagePosition(p,count,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particleNeighborhood_averagePosition;

  function particleNeighborhood_averageVelocity
    input Integer p;
    output Integer count;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particleNeighborhood_averageVelocity(p,count,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particleNeighborhood_averageVelocity;

  function particleNeighborhood_repulsiveDirection
    input Integer p;
    output Integer count;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particleNeighborhood_repulsiveDirection(p,count,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particleNeighborhood_repulsiveDirection;

  function particleNeighborhood_forEachParticle
    input Integer p;
    input String func;
    output Integer count;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particleNeighborhood_forEachParticle(p,func,count,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particleNeighborhood_forEachParticle;

  function particleNeighborhood_forEachParticle_1
    input Integer p;
    input String func;
    input Real arg1;
    output Integer count;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particleNeighborhood_forEachParticle_1(p,func,arg1,count,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particleNeighborhood_forEachParticle_1;

  function particleNeighborhood_forEachParticle_2
    input Integer p;
    input String func;
    input Real arg1;
    input Real arg2;
    output Integer count;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particleNeighborhood_forEachParticle_2(p,func,arg1,arg2,count,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particleNeighborhood_forEachParticle_2;

  function particleNeighborhood_evaluate_1
    input Integer p;
    input String func;
    input Real arg1;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_particleNeighborhood_evaluate_1(p,func,arg1,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end particleNeighborhood_evaluate_1;


  // Point & vector queries

  function volume_randomPoint
    input Integer v;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_volume_randomPoint(v,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end volume_randomPoint;

  function vectorWithNorm
    input Real vx;
    input Real vy;
    input Real vz;
    input Real norm;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_vectorWithNorm(vx,vy,vz,norm,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end vectorWithNorm;

  function randomVector
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_randomVector(x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end randomVector;

  function randomVectorWithNorm
    input Real norm;
    output Real x;
    output Real y;
    output Real z;
    external "C" retQSS_randomVectorWithNorm(norm,x,y,z) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end randomVectorWithNorm;


  // Misc. functionality

  function random
    input Real from;
    input Real to;
    output Real val;
    external "C" val=retQSS_random(from, to) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end random;

  function modulus
    input Integer a;
    input Integer b;
    output Integer m;
    external "C" m=retQSS_modulus(a, b) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end modulus;

  function div
    input Integer a;
    input Integer b;
    output Integer d;
    external "C" d=retQSS_div(a, b) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end div;

  function max2
    input Real a;
    input Real b;
    output Real m;
    external "C" m=retQSS_max(a, b) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end max2;

  function min2
    input Real a;
    input Real b;
    output Real m;
    external "C" m=retQSS_min(a, b) annotation(
        Include="#include \"retqss/retqss_model_api.h\"");
  end min2;

end retQSS;
