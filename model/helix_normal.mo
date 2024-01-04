model helix_normal

Real x, y, z, vx, vy, vz;

parameter Real Bx = 0;
parameter Real By = 0;
parameter Real Bz = 0.001;

parameter Real c  = 299.792458;
parameter Real m  = 0.510998910;
parameter Real q = 1.;
parameter Real w = 0.97;
parameter Real gamma2 = 22.36627204213;

parameter Real v0 = 0.999*c;

parameter Real invMGamma2 = 1 / (m * gamma2);
parameter Real qc2 = q*89875.5178737;
parameter Real coeff = qc2 * invMGamma2;

initial algorithm 
    x  := 0;
    y  := 0;
    z  := 0;

    vx := w*v0;
    vy := 0;
    vz := sqrt(1-w*w)*v0;

equation
    der(x)  = vx;
    der(y)  = vy;
    der(z)  = vz;
    der(vx) = coeff * (Bz*vy - By*vz);
    der(vy) = coeff * (Bx*vz - Bz*vx);
    der(vz) = coeff * (By*vx - Bx*vy);

annotation(
    experiment(
        MMO_Description="",
        MMO_Solver=QSS2,
        MMO_SymDiff=false,
        MMO_Period={1e-1},
        MMO_PartitionMethod=Metis,
        MMO_Output={x},
        MMO_OutputType=CI_Sampled,
        MMO_Scheduler=ST_Linear,
        Jacobian=Dense,
        StartTime=0,
        StopTime=1,
        Tolerance={0.3},
        AbsTolerance={0.5}
    ));

end helix_normal;
