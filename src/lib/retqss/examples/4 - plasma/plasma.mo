// Simple model to simulate the flow of uniform plasma in a 2D grid without
// obstructions, using the particle-in-cell method.
//
// Particles start in the left-hand side of the grid and move through the
// right. They are partitioned into particle clouds, each of them containing
// np_insert particles (defined below). One such cloud is initialized after
// deltaT time units. 
model plasma

// Import package containing retQSS API definition.
import retQSS;
// Import custom package containing user-supplied external functions for this
// model (in particular, a function to solve the linear system to compute
// the electric field in the grid).
import retQSS_PIC;

constant Integer
    // Number of nodes (vertices) in x and y, respectively.
    NX = 16, NY = 10,
    N_NODES = NX*NY,
    // Total number of cells (2D squares).
    N_CELLS = (NX-1)*(NY-1),
    // Total number of particles.
    N_PART = 27000;

Real x[N_PART], y[N_PART], z[N_PART], vx[N_PART], vy[N_PART], vz[N_PART];

// Definition of constants and parameters for the model.
parameter Real Q = 1.602e-19;
parameter Real M = 32*1.661e-27;
parameter Real EPS0 = 8.854e-12;
parameter Real n0 = 1e12;
parameter Real phi0 = 0;
parameter Real Te = 1;
parameter Real Ti = 0.1;
parameter Real lD = sqrt(EPS0*Te/(n0*Q));
parameter Real dx = lD;
parameter Real dx2 = dx*dx;
parameter Real Ly = (NY-1)*dx;
parameter Real vth = sqrt(2*Q*Ti/M);
parameter Real v_drift = 7000;
parameter Real deltaT = 0.1*dx/v_drift;
parameter Real np = 15;
parameter Real flux = n0*v_drift*Ly;
parameter Real npt = flux*deltaT;
parameter Real np_insert = (NY-1)*np; // Number of particles in a cloud.
parameter Real spwt = npt/np_insert;
parameter Real INF = 1e20;

// Auxiliary arrays to store charge density contributions for each particle
// in the four vertices of a cell (used to remember them so that they can
// be removed from previous cell when particle leaves it).
discrete Real rho_vertex1[N_PART];
discrete Real rho_vertex2[N_PART];
discrete Real rho_vertex3[N_PART];
discrete Real rho_vertex4[N_PART];

// Electric field affecting each particle (computed by differencing the
// electric potential).
discrete Real Ex[N_PART], Ey[N_PART];

// Start time for each particle (particles belonging to the same cloud will
// have the same start time).
discrete Real particleStartTime[N_PART];
// Time at which the cell of a particle should be updated (removing charge from
// previous cell and depositing it in the new cell).
discrete Real particleCellUpdateTime[N_PART];
// Whether a particle has just entered a new cell.
discrete Boolean particleEnteringNewCell[N_PART];
// Output arrays: charge density and electric potential on each node.
discrete Real RHO[2*N_NODES], PHI[2*N_NODES];
// Time of next output event: charge density and electric potential in the grid
// are copied to the output arrays. 
discrete Real nextOutputTime;
// Time at which the linear system will be solved.
discrete Real nextSystemSolvingTime;

// Other auxiliary discrete variables.
discrete Boolean status, solved;
discrete Integer volumeID[N_PART], prevVolumeID[N_PART];
discrete Real rho, phi, cap, E1, area;
discrete Real dummy1, dummy2, x1, x2, y1, y2, vertexZ;

initial algorithm
    // Define initial position and velocity for the particles.
    for i in 1:N_PART loop
        Ex[i] := 0;
        Ey[i] := 0;

        particleEnteringNewCell[i] := false;

        // Initialization of first particle cloud.
        if i <= np_insert then
            x[i] := random(0, dx);
            y[i] := random(0, (NY-1)*dx);
            z[i] := 0.5;

            vx[i] := v_drift + (-1.5 + random(0,1) + random(0,1) + random(0,1))*vth;
            vy[i] := (-1.5 + random(0,1) + random(0,1) + random(0,1))*vth*0.5;
            vz[i] := 0;

            particleCellUpdateTime[i] := 0;
        // Further clouds will be initialized in an event defined below.
        else
            particleStartTime[i] := div(i-1,np_insert)*deltaT;
            particleCellUpdateTime[i] := INF;
        end if;

        rho_vertex1[i] := 0;
        rho_vertex2[i] := 0;
        rho_vertex3[i] := 0;
        rho_vertex4[i] := 0;
    end for;

    for i in 1:2*N_NODES loop
        PHI[i] := -1;
        RHO[i] := -1;            
    end for;

    status := retQSS_setUp(N_PART, "geom/plasma.vtk", "plasma");

    if status == false then
        status := retQSS_fail();
    end if;

    for u in 1:2*N_NODES loop
        // Properties are flagged with -1 in those vertices that belong to
        // the lower plane of the grid, which will not be used in this
        // model.
        (dummy1, dummy2, vertexZ) := vertex_coordinates(u);

        if vertexZ > 0 then
            phi := 0;
            rho := 0;
            Ex := 0;
            Ey := 0;
        else
            phi := -1;
            rho := -1;
            Ex := -1;
            Ey := -1;
        end if;

        phi := vertex_setProperty(u, "phi", phi);
        rho := vertex_setProperty(u, "rho", rho);
        Ex := vertex_setProperty(u, "Ex", Ex);
        Ey := vertex_setProperty(u, "Ey", Ey);
    end for;

    nextOutputTime := INF;
    nextSystemSolvingTime := deltaT;

equation
    // Equations of motion of plasma particles.
    for i in 1:N_PART loop
        der(x[i]) = vx[i];
        der(y[i]) = vy[i];
        der(z[i]) = vz[i];
        der(vx[i]) = Q*Ex[i]/M;
        der(vy[i]) = Q*Ey[i]/M;
        der(vz[i]) = 0;
    end for;

algorithm
    for i in 1:N_PART loop
        // Event 1: particle enters a new cell.
        // Actions:
        //  * Find approximate time of half of its travel through current cell.
        //  * If it is exiting the geometry, kill it and remove charge from
        //    previous cell.
        when time > particle_nextCrossingTime(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) then
            volumeID[i] := particle_currentVolumeID(i);
            particleCellUpdateTime[i] := time + 0.1*deltaT;
            particleEnteringNewCell[i] := true;

            // Particle exiting geometry.
            if volumeID[i] == 0 then
                reinit(vx[i], 0);
                reinit(vy[i], 0);
                reinit(vz[i], 0);

                particleCellUpdateTime[i] := time;
            end if;
        end when;

        // Event 2: particle is halfway through current cell.
        // Actions:
        //  * Remove charge from previous cell (since we are about to deposit
        //    charge in current cell).
        //  * Deposit charge in current cell by computing the areas of the
        //    rectangles determined by current particle position and cell
        //    limits.
        when time >= particleCellUpdateTime[i] then
            if particleEnteringNewCell[i] then
                volumeID[i] := particle_previousVolumeID(i);
            else
                volumeID[i] := particle_currentVolumeID(i);
            end if;

            // Remove charge contribution from previous cell.
            rho := volume_getVertexProperty(volumeID[i], 5, "rho") - rho_vertex1[i];
            rho := volume_setVertexProperty(volumeID[i], 5, "rho", max2(rho,0));

            rho := volume_getVertexProperty(volumeID[i], 6, "rho") - rho_vertex2[i];
            rho := volume_setVertexProperty(volumeID[i], 6, "rho", max2(rho,0));

            rho := volume_getVertexProperty(volumeID[i], 7, "rho") - rho_vertex3[i];
            rho := volume_setVertexProperty(volumeID[i], 7, "rho", max2(rho,0));

            rho := volume_getVertexProperty(volumeID[i], 8, "rho") - rho_vertex4[i];
            rho := volume_setVertexProperty(volumeID[i], 8, "rho", max2(rho,0));

            Ex[i] := 0;
            Ey[i] := 0;
            volumeID[i] := particle_currentVolumeID(i);

            if particleEnteringNewCell[i] then
                particleCellUpdateTime[i] := time + (particle_nextCrossingTime(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) - time) / 2;
                particleEnteringNewCell[i] := false;
            else
                particleCellUpdateTime[i] := INF;
            end if;

            // Deposit charge only if particle has not exited the geometry.
            if volumeID[i] > 0 then
                (x1, y1, dummy1) := volume_vertexCoordinates(volumeID[i], 5);
                (x2, dummy1, dummy2) := volume_vertexCoordinates(volumeID[i], 6);
                (dummy1, y2, dummy2) := volume_vertexCoordinates(volumeID[i], 7);

                cap := volume_capacity(volumeID[i]);

                area := (x2-x[i])*(y2-y[i]) / cap;
                rho := volume_getVertexProperty(volumeID[i], 5, "rho");
                rho_vertex1[i] := spwt * (area/cap);
                rho := volume_setVertexProperty(volumeID[i], 5, "rho", rho + rho_vertex1[i]);
                Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 5, "Ex") * area;
                Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 5, "Ey") * area;

                area := (x[i]-x1)*(y2-y[i]) / cap;
                rho := volume_getVertexProperty(volumeID[i], 6, "rho");
                rho_vertex2[i] := spwt * (area/cap);
                rho := volume_setVertexProperty(volumeID[i], 6, "rho", rho + rho_vertex2[i]);
                Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 6, "Ex") * area;
                Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 6, "Ey") * area;

                area := (x2-x[i])*(y[i]-y1) / cap;
                rho := volume_getVertexProperty(volumeID[i], 7, "rho");
                rho_vertex3[i] := spwt * (area/cap);
                rho := volume_setVertexProperty(volumeID[i], 7, "rho", rho + rho_vertex3[i]);
                Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 7, "Ex") * area;
                Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 7, "Ey") * area;

                area := (x[i]-x1)*(y[i]-y1) / cap;
                rho := volume_getVertexProperty(volumeID[i], 8, "rho");
                rho_vertex4[i] := spwt * (area/cap);
                rho := volume_setVertexProperty(volumeID[i], 8, "rho", rho + rho_vertex4[i]);
                Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 8, "Ex") * area;
                Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 8, "Ey") * area;
            end if;
        end when;

        // Event 3: particle creation.
        // Actions:
        //  * Set random initial conditions (position along the first column of
        //    the grid).
        //  * Relocate particle in the geometry so that retQSS can properly
        //    track it.
        when time >= particleStartTime[i] then
            reinit(x[i], random(0, dx));
            reinit(y[i], random(0, (NY-1)*dx));
            reinit(z[i], 0.5);

            reinit(vx[i], v_drift + (-1.5 + random(0,1) + random(0,1) + random(0,1))*vth);
            reinit(vy[i], (-1.5 + random(0,1) + random(0,1) + random(0,1))*vth*0.5);
            reinit(vz[i], 0);

            volumeID[i] := particle_relocate(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]);

            (x1, y1, dummy1) := volume_vertexCoordinates(volumeID[i], 5);
            (x2, dummy1, dummy2) := volume_vertexCoordinates(volumeID[i], 6);
            (dummy1, y2, dummy2) := volume_vertexCoordinates(volumeID[i], 7);

            // Deposit charge in current cell nodes.
            cap := volume_capacity(volumeID[i]);

            area := (x2-x[i])*(y2-y[i]) / cap;
            Ex[i] := 0;
            Ey[i] := 0;

            Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 5, "Ex") * area;
            Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 5, "Ey") * area;

            area := (x[i]-x1)*(y2-y[i]) / cap;
            Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 6, "Ex") * area;
            Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 6, "Ey") * area;

            area := (x2-x[i])*(y[i]-y1) / cap;
            Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 7, "Ex") * area;
            Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 7, "Ey") * area;

            area := (x[i]-x1)*(y[i]-y1) / cap;
            Ex[i] := Ex[i] + volume_getVertexProperty(volumeID[i], 8, "Ex") * area;
            Ey[i] := Ey[i] + volume_getVertexProperty(volumeID[i], 8, "Ey") * area;
        end when;
    end for;

    // Event 4: solving of the linear system that approximates the potential
    // from the charge density on each node. The electric field is also updated
    // inside the external function called here.
    when time > nextSystemSolvingTime then
        solved := solvePotentialInNodes(NY, Q/EPS0, n0, phi0, Te);
        if not solved then
            status := retQSS_fail();
        end if;
        nextOutputTime := time;
        nextSystemSolvingTime := time + deltaT;
    end when;

    // Event 5: sampling of charge density and potential for output purposes.
    when time > nextOutputTime then
        for u in 1:2*N_NODES loop
            (dummy1, dummy2, vertexZ) := vertex_coordinates(u);
            if vertexZ > 0 then
                PHI[u] := vertex_getProperty(u, "phi");
                RHO[u] := vertex_getProperty(u, "rho") + 1e4;
            end if;
        end for;
        nextOutputTime := INF;
    end when;

annotation(
    experiment(
        MMO_Description="Simple plasma model (particle-in-cell method)",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={5e-3},
        MMO_LPS=0,
        MMO_DT_Min=2,
        MMO_Output={PHI,RHO},
        MMO_OutputType=CI_Step,
        Jacobian=Dense,
        StartTime=0.0,
        StopTime=2.126e-5,
        Tolerance={1e-6},
        AbsTolerance={1e-9}
));

end plasma;
