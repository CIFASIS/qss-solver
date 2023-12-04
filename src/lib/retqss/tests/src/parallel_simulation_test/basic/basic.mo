model basic_parallel_test
 
import retQSS;

constant Integer N = 4;

	Real x[N], y[N], z[N], vx[N], vy[N], vz[N];

	parameter Real Bx[N], By[N], Bz[N];
    
	parameter Real c  = 299.792458;
	parameter Real m  = 0.510998910;
	parameter Real q = 1.;
	parameter Real gamma2 = 22.36627204213;

parameter Real v0 = 0.999*c;

	parameter Real invMGamma2 = 1 / (m * gamma2);
	parameter Real qc2 = q*89875.5178737;
	parameter Real coeff = qc2 * invMGamma2;

 discrete Boolean status;
 discrete Integer volumeID[N];

	initial algorithm 

for i in 1:N loop
Bx[i] := 0;
By[i] := 0;
Bz[i] := 0.01;

x[i]  := 0;
y[i]  := 0;
z[i]  := 0;
vx[i] := 0.99498743710662*v0;
vy[i] := 0;
vz[i] := 0.1*v0;
end for;

status := retQSS_setUp(N, "../../data/cube/geom/cube.vtk", "basic");

if status == false then
  status := retQSS_fail();
end if;

for i in 1:N loop
volumeID[i] := particle_currentVolumeID(i);
end for;

	equation

for i in 1:N loop
		der(x[i])  = vx[i];
		der(y[i])  = vy[i];
		der(z[i])  = vz[i];
		der(vx[i]) = coeff * (Bz[i]*vy[i] - By[i]*vz[i]);
		der(vy[i]) = coeff * (Bx[i]*vz[i] - Bz[i]*vx[i]);
		der(vz[i]) = coeff * (By[i]*vx[i] - Bx[i]*vy[i]);
end for;

 algorithm
     for i in 1:N loop
       when time > particle_nextCrossingTime(i,x[i],y[i],z[i],vx[i],vy[i],vz[i]) then
           volumeID[i] := particle_currentVolumeID(i);
        end when;
    end for;
	annotation(

	experiment(
		MMO_Description="",
		MMO_Solver=QSS2,
		MMO_SymDiff=false,
		MMO_Parallel=true,
		MMO_PartitionMethod=Manual,
		MMO_LPS=3,
		MMO_Output={volumeID},
		MMO_Scheduler=ST_Linear,
		Jacobian=Dense,
		StartTime=0.0,
		StopTime=3,
		Tolerance={1e-7},
		AbsTolerance={1e-8}
	));
end basic_parallel_test;
