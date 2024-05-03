#include <math.h>

#include <retqss_pic.h>

#include <retqss_model_api.h>
#include <retqss_types.hh>


extern "C"
{

int PIC_solvePotential(
		int ny,
		double q_eps,
		double n0,
		double phi0,
		double Te)
{
	int n_cells = retQSS_geometry_countVolumes(), n_neighbors;
	double phi_i, rho_i;
	double dx2 = retQSS_volume_capacity(1), Ex, Ey;
	double dx = sqrt(dx2);
	int status = 0;

	LA_Matrix A(n_cells, n_cells);
	LA_Vector b0(n_cells), b(n_cells), x(n_cells);

	for(VolumeID i = 1; i <= n_cells; i++)
	{
		A(i-1, i-1) = 1;
		rho_i = phi_i = 0.;
		n_neighbors = retQSS_volumeNeighborhood_countVolumes(i);

		if(n_neighbors == 5)
		{
			phi_i = retQSS_volume_getProperty(i, "phi");
			rho_i = retQSS_volume_getProperty(i, "rho");

			A(i-1, i-1)    = -4/dx2;
			A(i-1, i)      = 1/dx2;
			A(i-1, i-2)    = 1/dx2;
			A(i-1, i-1+ny) = 1/dx2;
			A(i-1, i-1-ny) = 1/dx2;
		}

		b0[i-1] = rho_i + 1e4;
		x[i-1]  = phi_i;
	}

	for(int k = 0; k < 3001; k++)
	{
		for(VolumeID i = 0; i < n_cells; i++)
		{
			b[i] = 0;
			n_neighbors = retQSS_volumeNeighborhood_countVolumes(i+1);
			if(n_neighbors == 5)
				b[i] = -q_eps * (b0[i] - n0*exp((x[i]-phi0)/Te));
		}

		for(int i = 0; i < n_cells; i++)
		{
			double s1 = 0, s2 = 0;

			for(int j = 0; j < i; j++)
				s1 += A(i,j) * x[j];
			for(int j = i+1; j < n_cells; j++)
				s2 += A(i,j) * x[j];

			x[i] = (b[i] - s1 - s2) / A(i,i);
		}

		if(k % 10 == 0)
		{
			double norm = 0;
			LA_Vector n = b - A*x;

			for(int j = 0; j < n.dimension(); j++)
				norm += n[j]*n[j];

			if(sqrt(norm) < 0.1)
			{
				for(VolumeID i = 0; i < x.dimension(); i++)
				{
					retQSS_volume_setProperty(i+1, "phi", x[i]);

					Ex = Ey = 0;
					n_neighbors = retQSS_volumeNeighborhood_countVolumes(i+1);

					if(n_neighbors == 5)
					{
						Ex = (x[i-ny] - x[i+ny])/(2*dx);
						Ey = (x[i-1] - x[i+1])/(2*dx);
					}

					retQSS_volume_setProperty(i+1, "Ex", Ex);
					retQSS_volume_setProperty(i+1, "Ey", Ey);
				}

				status = 1;

				break;
			}
		}
	}

	return status;
}

int PIC_solvePotentialInNodes(
		int ny,
		double q_eps,
		double n0,
		double phi0,
		double Te)
{
	int n_nodes = retQSS_geometry_countVertices()/2;
	int n_cells = retQSS_geometry_countVolumes();
	double phi_j, rho_j;
	double dx2 = retQSS_volume_capacity(1), Ex, Ey;
	double dx = sqrt(dx2);
	int j;
	int status = 0;

	LA_Matrix A(n_nodes, n_nodes);
	LA_Vector b0(n_nodes), b(n_nodes), x(n_nodes);

	j = 0;
	for(VolumeID i = 1; i <= n_cells; i++)
	{
		A(j, j) = 1;
		rho_j = phi_j = 0.;

		if(j % ny != 0 && j >= ny)
		{
			A(j, j)    = -4/dx2;
			A(j, j+1)  = 1/dx2;
			A(j, j-1)  = 1/dx2;
			A(j, j+ny) = 1/dx2;
			A(j, j-ny) = 1/dx2;

			phi_j = retQSS_volume_getVertexProperty(i, 5, "phi");
			rho_j = retQSS_volume_getVertexProperty(i, 5, "rho") + 1e4;
		}

		b0[j] = rho_j;
		x[j]  = phi_j;
		j++;

		if(i % (ny-1) == 0)
		{
			A(j, j) = 1;
			b0[j] = 0;
			x[j]  = retQSS_volume_getVertexProperty(i, 7, "phi");
			j++;
		}
	}

	for(VolumeID i = n_cells-ny+2; i <= n_cells; i++)
	{
		A(j, j) = 1;
		b0[j] = 0;
		x[j]  = retQSS_volume_getVertexProperty(i, 6, "phi");
		j++;
	}

	A(j, j) = 1;
	b0[j] = 0;
	x[j]  = retQSS_volume_getVertexProperty(n_cells, 8, "phi");

	for(int k = 0; k < 3001; k++)
	{
		for(int i = 0; i < n_nodes; i++)
		{
			b[i] = 0;
			if(b0[i] != 0)
				b[i] = -q_eps * (b0[i] - n0*exp((x[i]-phi0)/Te));
		}

		for(int i = 0; i < n_nodes; i++)
		{
			double s1 = 0, s2 = 0;

			for(int j = 0; j < i; j++)
				s1 += A(i,j) * x[j];
			for(int j = i+1; j < n_nodes; j++)
				s2 += A(i,j) * x[j];

			x[i] = (b[i] - s1 - s2) / A(i,i);
		}

		if(k % 10 == 0)
		{
			double norm = 0;
			LA_Vector n = b - A*x;

			for(int j = 0; j < n.dimension(); j++)
				norm += n[j]*n[j];

			if(sqrt(norm) < 0.1)
			{
				j = 0;
				for(VolumeID i = 1; i <= n_cells; i++)
				{
					retQSS_volume_setVertexProperty(i, 5, "phi", x[j]);

					if(j < ny)
						Ex = (x[j] - x[j+ny])/dx;
					else
						Ex = (x[j-ny] - x[j+ny])/(2*dx);

					if(j % ny == 0)
						Ey = (x[j] - x[j+1])/dx;
					else
						Ey = (x[j-1] - x[j+1])/(2*dx);

					retQSS_volume_setVertexProperty(i, 5, "Ex", Ex);
					retQSS_volume_setVertexProperty(i, 5, "Ey", Ey);

					j++;

					if(i % (ny-1) == 0)
					{
						retQSS_volume_setVertexProperty(i, 7, "phi", x[j]);

						if(j < ny)
							Ex = (x[j] - x[j+ny])/dx;
						else
							Ex = (x[j-ny] - x[j+ny])/(2*dx);
						Ey = (x[j-1] - x[j])/dx;

						retQSS_volume_setVertexProperty(i, 7, "Ex", Ex);
						retQSS_volume_setVertexProperty(i, 7, "Ey", Ey);

						j++;
					}
				}

				for(VolumeID i = n_cells-ny+2; i <= n_cells; i++)
				{
					retQSS_volume_setVertexProperty(i, 6, "phi", x[j]);

					Ex = (x[j-ny] - x[j])/dx;
					if(j % ny == 0)
						Ey = (x[j] - x[j+1])/dx;
					else
						Ey = (x[j-1] - x[j+1])/(2*dx);

					retQSS_volume_setVertexProperty(i, 6, "Ex", Ex);
					retQSS_volume_setVertexProperty(i, 6, "Ey", Ey);

					j++;
				}

				retQSS_volume_setVertexProperty(n_cells, 8, "phi", x[j]);

				Ex = (x[j-ny] - x[j])/dx;
				Ey = (x[j-1] - x[j])/dx;

				retQSS_volume_setVertexProperty(n_cells, 8, "Ex", Ex);
				retQSS_volume_setVertexProperty(n_cells, 8, "Ey", Ey);

				status = 1;

				break;
			}
		}
	}

	return status;
}

}
