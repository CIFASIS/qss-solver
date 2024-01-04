clear variables
%identify globals needed by the potential solver
global EPS0 QE den A n0 phi0 phi_p Te box 

den = csvread('rho.txt');
phi = csvread('phi.txt');

arg_list = argv ();
tstep = arg_list{1};

it = 200

figure(1);                              %density
contourf(den',1e11:1e11:1.1e12);
hold on;
c = colorbar;
set(c, 'fontsize', 16);
%contour(object',[1 1],'-k','LineWidth',2);
title(sprintf('Charge density - retQSS'), 'fontsize',18);
saveas(gcf, sprintf('rho-T=%s.png',tstep));
hold off;

figure(2);                              %potential
contourf(phi');
hold on;
caxis([-0.09 0.015]);
%contour(object',[1 1],'-k','LineWidth',2);
hold off;
title(sprintf('Potential - retQSS'),'fontsize',18);
c = colorbar;
set(c, 'fontsize', 16);
saveas(gcf, sprintf('phi-T=%s.png',tstep));


#%{
den = csvread('../../model/pic/rho.txt');
phi = csvread('../../model/pic/phi.txt');


figure(3);                              %density
contourf(den',1e11:1e11:1.1e12);
hold on;
c = colorbar;
set(c, 'fontsize', 16);
%contour(object',[1 1],'-k','LineWidth',2);
title(sprintf('Charge density - MATLAB'), 'fontsize',18);
saveas(gcf, sprintf('rho-matlab-T=%s.png',tstep));
hold off;


figure(4);                              %potential
contourf(phi');
hold on;
caxis([-0.09 0.015]);
%contour(object',[1 1],'-k','LineWidth',2);
hold off;
title(sprintf('Potential - MATLAB'),'fontsize',18);
c = colorbar;
set(c, 'fontsize', 16);
saveas(gcf, sprintf('phi-matlab-T=%s.png',tstep));


#%}

%drawnow;  

pause;
