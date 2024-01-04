%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Simple Electrostatic Particle In Cell (PIC) code in MATLAB
% Flow of solar wind around a charged plate
%
% For more, visit http://www.particleincell.com/2010/es-pic-method/
% and http://www.particleincell.com/2011/particle-in-cell-example/
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
1;


function [x]=eval_2dpot_GS(phi)
global A den n0 phi0 Te phi_p EPS0 QE; % box

tol = 0.1;      %solver tolerance

%get nx from size of density
nx = size(den,1);
ny = size(den,2);
nn = numel(den);

%convert density and potential into column vectors
b0 = reshape(den, numel(den),1);
x = reshape(phi, numel(phi),1);

    b0(1:nx) = 0;                 %zero electric field on y=0;
    b0(nn-nx+1:nn) = 0;           %zero electric field on y=L;
    b0(nx:nx:nn) = 0;             %zero electric field on x=L;
    b0(1:nx:nn) = 0;           %fixed potential on x=0;

%solve
for it=1:2000
    
    %recalculate rhs
    b = b0 - n0*exp((x-phi0)/Te); %add boltzmann term for electrons
    b = -b*QE/EPS0;
 
    %set boundaries
    b(1:nx) = 0;                 %zero electric field on y=0;
    b(nn-nx+1:nn) = 0;           %zero electric field on y=L;
    b(nx:nx:nn) = 0;             %zero electric field on x=L;
    b(1:nx:nn) = phi0;           %fixed potential on x=0;

    %set potential on fixed nodes
    %for j=box(2,1):box(2,2)
    %    b([box(1,1):box(1,2)]+(j-1)*nx)=ones(box(1,2)-box(1,1)+1,1)*phi_p;      %wall potential
    %end

    %update nodes
	for i=1:nn
		x(i)=(b(i) - A(i,1:i-1)*x(1:i-1) - A(i,i+1:nn)*x(i+1:nn))/A(i,i);
    end
    
    %compute residue to check for convergence, do only every 10 iterations
    if mod(it,10)==0
        R=norm(b-A*x);      %residue
        if (R<=tol)         %converged
            %disp(sprintf('  GS converged in %i iterations with norm %g',it,R));
            break;
        end
    end
end

%check if the solver converged to the specified tolerance
if (R>tol)
    disp('  GS failed to converge!!');
end

%return solution as a nx*ny array
x=reshape(x,nx,ny);

endfunction




%reset variables
clear variables

page_screen_output(0);
page_output_immediately(1);

%identify globals needed by the potential solver
global EPS0 QE den A n0 phi0 phi_p Te box 

file = fopen('particles.txt', 'w');

%setup constants
EPS0 = 8.854e-12;		%permittivity of free space
QE = 1.602e-19;			%elementary charge
K = 1.381e-23;			%boltzmann constant
AMU = 1.661e-27;		%atomic mass unit
M = 32*AMU;             %ion mass (molecular oxygen)

%input settings
n0 = 1e12;				%density in #/m^3
phi0 = 0;               %reference potential 
Te = 1;                 %electron temperature in eV
Ti = 0.1;              	%ion velocity in eV
v_drift = 7000;        	%ion injection velocity, 7km/s
phi_p = -5;            	%wall potential

%calculate plasma parameters
lD = sqrt(EPS0*Te/(n0*QE));    	%Debye length
vth = sqrt(2*QE*Ti/M);			%thermal velocity with Ti in eV

%set simulation domain
nx = 16;               %number of nodes in x direction
ny = 10;               %number of nodes in y direction
ts = 200;              %number of time steps
dh = lD;               %cell size
np_idx = 15;
np_insert = (ny-1)*np_idx; %insert 15 particles per cell
nptot = 0;

%compute some other values
nn = nx*ny;             %total number of nodes
dt = 0.1*dh/v_drift;	%time step, at vdrift move 0.10dx
Lx = (nx-1)*dh;         %domain length in x direction
Ly = (ny-1)*dh;         %domain length in y direction

%specify plate dimensions
%box(1,:) = [floor(nx/3) floor(nx/3)+2]; %x range
%box(2,:) = [1 floor(ny/2)];             %y range

%create an object domain for visualization
%object = zeros(nx,ny);
%for j=box(2,1):box(2,2)
%    object(box(1,1):box(1,2),j)=ones(box(1,2)-box(1,1)+1,1);
%end

%calculate specific weight
flux = n0*v_drift*Ly;       %flux of entering particles
npt = flux*dt;              %number of real particles created per timestep
spwt = npt/np_insert;       %specific weight, real particles per macroparticle
mp_q = 1;                   %macroparticle charge
max_part=27000;             %buffer size

%allocate particle array
part_x = zeros(max_part,2); %particle positions
part_v = zeros(max_part,2); %particle velocities

%set up multiplication matrix for potential solver
%here we are setting up the Finite Difference stencil

A = zeros(nn);              %allocate empty nn * nn matrix

%set regular stencil on internal nodes
for j=2:ny-1                    %only internal nodes
    for i=2:nx-1
        u = (j-1)*nx+i;         %unknown (row index)
        
        A(u,u) = -4/(dh*dh);    %phi(i,j)
        A(u,u-1)=1/(dh*dh);     %phi(i-1,j)
        A(u,u+1)=1/(dh*dh);     %phi(i+1,j)
        A(u,u-nx)=1/(dh*dh);    %phi(i,j-1)
        A(u,u+nx)=1/(dh*dh);    %phi(i,j+1)
    end  
end

%neumann boundary on y=0
for i=1:nx
    u=i;
    A(u,u) = 1; %-1/dh;              %phi(i,j)
    A(u,u+nx) = 0; %1/dh;            %phi(i,j+1)
end

%neumann boundary on y=Ly
for i=1:nx
    u=(ny-1)*nx+i;
    A(u,u-nx) = 0; %1/dh;            %phi(i,j-1)
    A(u,u) = 1;%-1/dh;              %phi(i,j)
end

%neumann boundary on x=Lx
for j=1:ny
    u=(j-1)*nx+nx;
    A(u,:)=zeros(1,nn);         %clear row
    A(u,u-1) = 0; %1/dh;            %phi(i-1,j)
    A(u,u) = 1;%-1/dh;             %phi(i,j)
end

%dirichlet boundary on x=0
for j=1:ny
    u=(j-1)*nx+1;
    A(u,:)=zeros(1,nn);         %clear row
    A(u,u) = 1;                 %phi(i,j)
end

%dirichlet boundary on nodes corresponding to the plate
%for j=box(2,1):box(2,2)
%    for i=box(1,1):box(1,2)
%        u=(j-1)*nx+i;
%        A(u,:)=zeros(1,nn);     %clear row
%        A(u,u)=1;               %phi(i,j)
%    end
%end

%initialize
phi = ones(nx,ny)*phi0;         %set initial potential to phi0
np = 0;                         %clear number of particles

%disp(['Solving potential for the first time. Please be patient, this could take a while.']);

%%%%%%%%%%%%%%%%%%%%%%%%
% MAIN LOOP
%%%%%%%%%%%%%%%%%%%%%%%%
for it=1:ts                     %iterate for ts time steps
      	    
	%reset field quantities
	den = zeros(nx,ny);         %number density
	efx = zeros(nx,ny);         %electric field, x-component
    efy = zeros(nx,ny);         %electric field, y-component
	chg = zeros(nx,ny);         %charge distribution
    
    %*** 1. CALCULATE CHARGE DENSITY ***
	
    % deposit charge to nodes
	for p=1:np                          %loop over particles
		fi = 1+part_x(p,1)/dh;          %real i index of particle's cell
		i = floor(fi);                  %integral part
		hx = fi-i;                      %the remainder
        
        fj = 1+part_x(p,2)/dh;          %real j index of particle's cell
        j = floor(fj);                  %integral part
        hy = fj-j;                      %the remainder

        %interpolate charge to nodes
		chg(i,j) = chg(i,j) + (1-hx)*(1-hy);
		chg(i+1,j) = chg(i+1,j) + hx*(1-hy);
        chg(i,j+1) = chg(i,j+1) + (1-hx)*hy;
        chg(i+1,j+1) = chg(i+1,j+1) + hx*hy;
	end 

	%calculate density
	den = spwt*mp_q*chg/(dh*dh);
    
    %apply boundaries
	%den(1,:) = 2*den(1,:);      %double density since only half volume contributing
    %den(nx,:) = 2*den(nx,:);
    %den(:,1) = 2*den(:,1);
    %den(:,ny) = 2*den(:,ny);
    
    %add density floor for plotting and to help the solver
    den = den + 1e4;
	
	%*** 2. CALCULATE POTENTIAL ***    
    phi = eval_2dpot_GS(phi);
	
	%*** 3. CALCULATE ELECTRIC FIELD ***
	efx(2:nx-1,:) = phi(1:nx-2,:) - phi(3:nx,:);  %central difference on internal nodes
    efy(:,2:ny-1) = phi(:,1:ny-2) - phi(:,3:ny);  %central difference on internal nodes
    efx(1,:) = 2*(phi(1,:) - phi(2,:));           %forward difference on x=0
    efx(nx,:) = 2*(phi(nx-1,:) - phi(nx,:));      %backward difference on x=Lx
    efy(:,1) = 2*(phi(:,1) - phi(:,2));           %forward difference on y=0
    efy(:,ny) = 2*(phi(:,ny-1) - phi(:,ny));      %forward difference on y=Ly
    
    efx = efx / (2*dh);    %divide by dominator
    efy = efy / (2*dh);

    %*** 4. GENERATE NEW PARTICLE ***
    if (np+np_insert>=max_part)     %make sure we don't exceed array limits
  %      np_insert=max_part-np;
    end

    %insert particles randomly distributed in y and in the first cell
    part_x(np+1:np+np_insert,1)=0;%rand(np_insert,1)*dh;   %x position
    part_x(np+1:np+np_insert,2)=rand(np_insert,1)*Ly;   %y position

    %sample Maxwellian in x and y, add drift velocity in x
    part_v(np+1:np+np_insert,1)=v_drift+(-1.5+rand(np_insert,1)+rand(np_insert,1)+rand(np_insert,1))*vth;
    part_v(np+1:np+np_insert,2)=0.5*(-1.5+rand(np_insert,1)+rand(np_insert,1)+rand(np_insert,1))*vth;

	p = np+1;
    while(p <= np+np_insert)
        fprintf(file, '%.10f %.10f %f %.10f %.10f %f\n', part_x(p,1), part_x(p,2), 0.5, part_v(p,1), part_v(p,2), 0);
        p = p+1;
    end

    np=np+np_insert;    %increment particle counter
    nptot = nptot + np_insert;

    %*** 5. MOVE PARTICLES ***
	p=1;
    while(p<=np)                        %loop over particles
		fi = 1+part_x(p)/dh;            %i index of particle's cell
		i  = floor(fi);
		hx = fi-i;                      %fractional x position in cell
        
        fj = 1+part_x(p,2)/dh;          %j index of particle' cell
        j = floor(fj);
        hy = fj-j;                      %fractional y position in cell
        
        %gather electric field
        E=[0 0];
  		E = [efx(i,j) efy(i,j)]*(1-hx)*(1-hy);      %contribution from (i,j)
		E = E+ [efx(i+1,j) efy(i+1,j)]*hx*(1-hy);   %(i+1,j)
        E = E + [efx(i,j+1) efy(i+1,j)]*(1-hx)*hy;  %(i,j+1)
        E = E + [efx(i+1,j+1) efy(i+1,j+1)]*hx*hy;  %(i+1,j+1)
        
        %update velocity and position
        F = QE*E;                           %Lorentz force, F=qE
		a = F/M;                            %acceleration
		part_v(p,:) = part_v(p,:)+a*dt;     %update velocity
		part_x(p,:) = part_x(p,:)+part_v(p,:)*dt;   %update position

        %process boundaries
             
        %reflective boundary on bottom
		%if (part_x(p,2)<0)                  %y<0
		%	part_x(p,2) = -part_x(p,2);     %move particle back to domain
		%	part_v(p,2) = -part_v(p,2);     %reverse y-velocity
        %end

        %see if particle is inside the plate
        %in_box=false;
        %if ((i>=box(1,1) && i<box(1,2)) && ...
        %    (j>=box(2,1) && j<box(2,2)))
        %    in_box=true;
        %end
            
        %absorbing boundary on left, right, top or if in object
		if (part_x(p,2)<0 || part_x(p,1)<0 || part_x(p,1)>=Lx || part_x(p,2)>=Ly )%|| in_box)
			part_x(p,:) = part_x(np,:);         %kill particle by replacing it with last particle
			part_v(p,:) = part_v(np,:);
			np = np - 1;                        %reduce particle count
            p = p-1;                            %reduce particle index so this entry will be processed again
        end
        
        p=p+1;                                  %move to the next particle
    end

 %#{
    %*** 6. PLOT RESULTS ***
	if (mod(it,25) == 0 || it==ts)              %plot only every 25 time steps
       

        figure(1);                              %density
        contourf(den',1e11:1e11:1.1e12);
        hold on;
		c = colorbar;
        set(c, 'fontsize', 16);
        %contour(object',[1 1],'-k','LineWidth',2);
        title(sprintf('Charge density - MATLAB'), 'fontsize',18);
        if(it == 100)
            saveas(gcf, 'pic_matlab-rho-T=100.png');
        end
        if(it == ts)
            saveas(gcf, 'pic_matlab-rho-T=200.png');
        end
        hold off;
        
		figure(2);                              %potential
		contourf(phi');
        hold on;
        %contour(object',[1 1],'-k','LineWidth',2);
        hold off;
        title(sprintf('Potential - MATLAB'),'fontsize',18);
        c = colorbar;
        set(c, 'fontsize', 16);
        if(it == 100)
            saveas(gcf, 'pic_matlab-phi-T=100.png');
        end
        if(it == ts)
            caxis([-0.09 0.015]);
            saveas(gcf, 'pic_matlab-phi-T=200.png');
        end
		
        drawnow;                                %force draw
    end

    disp(sprintf('Time Step %i, Particles %i, Time %g',it, nptot, it*dt));
%#}  
    if(it == 50 || it == 100 || it == 150 || it == 200)
        tic;
        csvwrite(sprintf('phi-T=%d.txt', it), phi);
        toc;
        tic;
        csvwrite(sprintf('rho-T=%d.txt', it), den);
        toc;
    end


end

fclose(file);

disp(sprintf('Complete!\n'));                   %exit




