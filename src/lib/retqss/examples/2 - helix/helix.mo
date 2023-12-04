// Particle describing a helical trajectory inside a 3D cube mesh, reacting to
// volume properties by changing its equations of motion.
model helix

// Import package containing retQSS API definition.
import retQSS;

Real x, y, z, vx, vy, vz;

// Static magnetic field in the z axis.
parameter Real Bx = 0;
parameter Real By = 0;
parameter Real Bz = 0.001;

// Parameters for the equations of motion.
parameter Real c  = 299.792458;
parameter Real m  = 0.510998910;
parameter Real q = 1.;
parameter Real gamma2 = 22.36627204213;
parameter Real v0 = 0.999*c;
parameter Real invMGamma2 = 1 / (m * gamma2);
parameter Real qc2 = q*89875.5178737;
parameter Real coeff = qc2 * invMGamma2;

parameter Real delta = 0.1;

discrete Boolean status;
discrete Integer volumeID;
discrete Real cx, cy, cz, K, next_check;

initial algorithm 
    // Initial position and velocity.
    x  := 0;
    y  := 0;
    z  := 0;

    vx := 0.9797958971132712*v0;
    vy := 0;
    vz := 0.2*v0;

    // Initialize retQSS geometry and set up one particle.
    status := retQSS_setUp(1, "geom/helix.vtk", "helix");

    if status == false then
        status := retQSS_fail();
    end if;

    volumeID := particle_currentVolumeID(1);

    // Multiplicative constant affecting the velocity in the z axis.
    // It changes according to the volume currently traversed by the
    // particle.
    K := 1;

    // Sanity checks performed at regular time intervals.
    next_check := delta;

equation
    // Equations of motion of a charged particle in a magnetic field.
    der(x)  = vx;
    der(y)  = vy;
    der(z)  = vz*K;
    der(vx) = coeff * (Bz*vy - By*vz);
    der(vy) = coeff * (Bx*vz - Bz*vx);
    der(vz) = coeff * (By*vx - Bx*vy);

algorithm
    // When the event is triggered, the particle has just crossed a volume
    // boundary, entering a new volume.
    when time > particle_nextCrossingTime(1,x,y,z,vx,vy,vz) then
        volumeID := particle_currentVolumeID(1);

        // Volume 0 represents the exterior of the geometry.
        // So, in this case, the particle entered one of the polyhedral volumes
        // in the geometry.
        if volumeID > 0 then
            (cx, cy, cz) := volume_centroid(volumeID);

            // When the z coordinate of the volume centroid is >= 120, K is set
            // to 2.
            if cz >= 120 then
                K := 2;
            end if;
            // When the z coordinate of the volume centroid is >= 400, K is set
            // to 0.5.
            if cz >= 400 then
                K := 0.5;
            end if;
        end if;
    end when;

    // Sanity checks at regular time intervals: current particle position is
    // located in the geometry and the volume obtained is tested against the
    // current volume tracked by retQSS.
    when time > next_check then
        status := particle_checkPosition(1);
        next_check := time + delta;
    end when;
    
annotation(
    experiment(
        MMO_Description="Helical trajectory in a 3D cube mesh",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_PartitionMethod=Metis,
        MMO_Output={volumeID, cz},
        MMO_Scheduler=ST_Linear,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=20,
        Tolerance={1e-5},
        AbsTolerance={1e-8}
    ));

end helix;
