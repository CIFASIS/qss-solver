package retQSS_boids

  function updateBoid
    input Integer boid_ID;
    output Integer status;
    external "C" status=boids_update(boid_ID) annotation(
        Library="boids",
        Include="#include \"retqss_boids.h\"");
  end updateBoid;

end retQSS_boids;
