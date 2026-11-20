model adr2D_boundaries
  parameter Real ax = 1;
  parameter Real ay = 1;
  parameter Real d = 0.01;
  parameter Real r = 1;
  constant Integer Nx = 4;
  constant Integer Ny = 4;
  parameter Real dx = 10 / (Nx - 1);
  parameter Real dy = dx;
  parameter Real u0 = 1;
  Real u[Nx, Ny];
equation
  // Interior domain
  for i in 2:Nx-1, j in 2:Ny-1 loop
    der(u[i,j]) = -ax*(u[i,j] - u[i-1,j])/dx - ay*(u[i,j] - u[i,j-1])/dy
                  + d*(u[i+1,j] - 2*u[i,j] + u[i-1,j])/dx^2
                  + d*(u[i,j+1] - 2*u[i,j] + u[i,j-1])/dy^2
                  + r*(u[i,j]^2 - u[i,j]^3);
  end for;
  // Left and Right boundaries (excluding corners)
  for j in 2:Ny-1 loop
    der(u[1,j]) = -ax*(u[1,j] - u0)/dx - ay*(u[1,j] - u[1,j-1])/dy
                  + d*(u[2,j] - 2*u[1,j] + u0)/dx^2
                  + d*(u[1,j+1] - 2*u[1,j] + u[1,j-1])/dy^2
                  + r*(u[1,j]^2 - u[1,j]^3);
    der(u[Nx,j]) = -ax*(u[Nx,j] - u[Nx-1,j])/dx - ay*(u[Nx,j] - u[Nx,j-1])/dy
                   + d*(2*u[Nx-1,j] - 2*u[Nx,j])/dx^2
                   + d*(u[Nx,j+1] - 2*u[Nx,j] + u[Nx,j-1])/dy^2
                   + r*(u[Nx,j]^2 - u[Nx,j]^3);
  end for;
  // Bottom and Top boundaries (excluding corners)
  for i in 2:Nx-1 loop
    der(u[i,1]) = -ax*(u[i,1] - u[i-1,1])/dx - ay*(u[i,1] - u0)/dy
                  + d*(u[i+1,1] - 2*u[i,1] + u[i-1,1])/dx^2
                  + d*(u[i,2] - 2*u[i,1] + u0)/dy^2
                  + r*(u[i,1]^2 - u[i,1]^3);
    der(u[i,Ny]) = -ax*(u[i,Ny] - u[i-1,Ny])/dx - ay*(u[i,Ny] - u[i,Ny-1])/dy
                   + d*(u[i+1,Ny] - 2*u[i,Ny] + u[i-1,Ny])/dx^2
                   + d*(2*u[i,Ny-1] - 2*u[i,Ny])/dy^2
                   + r*(u[i,Ny]^2 - u[i,Ny]^3);
  end for;
  // Corners
  der(u[1,1]) = -ax*(u[1,1] - u0)/dx - ay*(u[1,1] - u0)/dy
                + d*(u[2,1] - 2*u[1,1] + u0)/dx^2
                + d*(u[1,2] - 2*u[1,1] + u0)/dy^2
                + r*(u[1,1]^2 - u[1,1]^3);
  der(u[1,Ny]) = -ax*(u[1,Ny] - u0)/dx - ay*(u[1,Ny] - u[1,Ny-1])/dy
                 + d*(u[2,Ny] - 2*u[1,Ny] + u0)/dx^2
                 + d*(2*u[1,Ny-1] - 2*u[1,Ny])/dy^2
                 + r*(u[1,Ny]^2 - u[1,Ny]^3);
  der(u[Nx,1]) = -ax*(u[Nx,1] - u[Nx-1,1])/dx - ay*(u[Nx,1] - u0)/dy
                 + d*(2*u[Nx-1,1] - 2*u[Nx,1])/dx^2
                 + d*(u[Nx,2] - 2*u[Nx,1] + u0)/dy^2
                 + r*(u[Nx,1]^2 - u[Nx,1]^3);
  der(u[Nx,Ny]) = -ax*(u[Nx,Ny] - u[Nx-1,Ny])/dx - ay*(u[Nx,Ny] - u[Nx,Ny-1])/dy
                  + d*(2*u[Nx-1,Ny] - 2*u[Nx,Ny])/dx^2
                  + d*(2*u[Nx,Ny-1] - 2*u[Nx,Ny])/dy^2
                  + r*(u[Nx,Ny]^2 - u[Nx,Ny]^3);
  annotation(
    experiment(
      MMO_Description="",
      MMO_Solver=LIQSS1,
      MMO_PartitionMethod=Metis,
      MMO_Output={u[2,2], u[1,2], u[4,3], u[2,1], u[3,4]},
      MMO_OutputType=CI_Sampled,
      Jacobian=Dense,
      MMO_BDF_PDepth=1,
      MMO_BDF_Max_Step=0,
      StartTime=0.0,
      StopTime=10,
      Tolerance={1e-2},
      AbsTolerance={1e-4}
    ));
end adr2D_boundaries;
