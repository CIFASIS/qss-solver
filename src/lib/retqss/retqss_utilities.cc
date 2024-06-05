#include <assert.h>
#include <algorithm>
#include <cstdlib>
#include <random>
#include <math.h>
#include <string.h>

#include "retqss_utilities.hh"
#include "retqss_particle_tracker.hh"
#include "retqss_exceptions.hh"


retQSS::InitialConditionArray *retQSS::read_initial_conditions(
		const std::string &filename,
		int n_particles)
{
	auto ic_array = new retQSS::InitialConditionArray;

	std::ifstream file(filename);
	if(!file.good())
		throw retQSS::FileAccessException(filename);

	for(int i = 0; i < n_particles; i++)
	{
		retQSS::InitialCondition ic;
		if(file >> ic.x >> ic.y >> ic.z >> ic.vx >> ic.vy >> ic.vz >> ic.volumeID)
		{
			ic_array->push_back(ic);
		}
		else
		{
			delete ic_array;

			std::stringstream ss_msg;
			ss_msg
				<< "Failed to initialize particles: only "
				<< i << " initial conditions available (of a total of "
				<< n_particles << ")"
				<< std::endl;

			throw retQSS::Exception(ss_msg.str());
		}
	}

	return ic_array;
}

// Includes negative roots for proper surface crossing detection.
void retQSS::poly_roots_sq(double *coeff, double *roots, int *n_roots)
{
	double sd, r1;

	*n_roots = 0;

	if (coeff[2] == 0)
	{
		if(coeff[1] != 0)
		{
			r1 = -coeff[0] / coeff[1];
			roots[(*n_roots)++] = r1;
		}
	}
	else
	{
		double disc;
		disc = coeff[1] * coeff[1] - 4 * coeff[2] * coeff[0];
		if (disc >= 0)
		{
			sd = sqrt(disc);

			r1 = (-coeff[1] + sd) / 2 / coeff[2];
			roots[(*n_roots)++] = r1;

			r1 = (-coeff[1] - sd) / 2 / coeff[2];
			roots[(*n_roots)++] = r1;

			// Sort roots.
			if(*n_roots == 2 && roots[0] > roots[1])
				std::swap(roots[0], roots[1]);
		}
	}
}

// TODO: works for QSS2 and QSS3.
void retQSS::poly_roots(double *coeff, double *roots, int *n_roots)
{
	*n_roots = 0;

	if (coeff[3] == 0)
		retQSS::poly_roots_sq(coeff, roots, n_roots);

	else if(coeff[0] == 0)
	{
		if(coeff[1] == 0)
			roots[(*n_roots)++] = -coeff[2] / coeff[3];
		else
		{
			coeff[0] = coeff[1];
			coeff[1] = coeff[2];
			coeff[2] = coeff[3];
			retQSS::poly_roots_sq(coeff, roots, n_roots);
		}
	}

	else
	{
		double q, r, disc;
		q = (3 * coeff[3] * coeff[1] - coeff[2] * coeff[2]) / 9 / coeff[3]
		  / coeff[3];
		r = (9 * coeff[3] * coeff[2] * coeff[1]
		  - 27 * coeff[3] * coeff[3] * coeff[0]
		  - 2 * coeff[2] * coeff[2] * coeff[2]) / 54 / coeff[3] / coeff[3]
		  / coeff[3];
		disc = q * q * q + r * r;
		if (disc >= 0)
	    {
			//only one real root
			double sd, s, t, r1;
			sd = sqrt (disc);
			if (r + sd > 0)
				s = pow (r + sd, 1.0 / 3);
			else
				s = -pow (fabs (r + sd), 1.0 / 3);

			if (r - sd > 0)
				t = pow (r - sd, 1.0 / 3);
			else
				t = -pow (fabs (r - sd), 1.0 / 3);

	      r1 = s + t - coeff[2] / 3 / coeff[3];
	      if (r1 > 0)
	    	  roots[(*n_roots)++] = r1;
	    }
		else
	    {
			//three real roots
			double rho, th, rho13, costh3, sinth3, spt, smti32, r1;
			rho = sqrt (-q * q * q);
			th = acos (r / rho);
			rho13 = pow (rho, 1.0 / 3);
			costh3 = cos (th / 3);
			sinth3 = sin (th / 3);
			spt = rho13 * 2 * costh3;
			smti32 = -rho13 * sinth3 * sqrt (3);

			r1 = spt - coeff[2] / 3 / coeff[3];
			if(r1 > 0)
				roots[(*n_roots)++] = r1;

			r1 = -spt / 2 - coeff[2] / 3 / coeff[3] + smti32;
			if(r1 > 0)
				roots[(*n_roots)++] = r1;;

			r1 = r1 - 2 * smti32;
			if(r1 > 0)
				roots[(*n_roots)++] = r1;

			// Sort roots.
			if(*n_roots >= 2 && roots[0] > roots[1])
				std::swap(roots[0], roots[1]);
			if(*n_roots == 3 && roots[1] > roots[2])
				std::swap(roots[1], roots[2]);
	    }
	}
}

double retQSS::angle_between(const Vector_3 &u, const Vector_3 &v)
{
	// Returns angle (in radians) in range [0, pi].

	auto s = CGAL::sqrt(CGAL::cross_product(u, v).squared_length());
	auto c = u * v;

	return std::atan2(s, c);
}

std::mt19937 __retqss_random_generator(0);

bool retQSS::random_reseed(int seed)
{
	__retqss_random_generator = std::mt19937(seed);
	return true;
}

double retQSS::random_double(double from, double to)
{
	std::uniform_real_distribution<double> dist(from, to);
	return dist(__retqss_random_generator);
}

double retQSS::random_lognormal(double m, double s)
{
	std::lognormal_distribution<double> dist(m, s);
	return dist(__retqss_random_generator);
}

double retQSS::random_normal(double m, double s)
{
	std::normal_distribution<double> dist(m, s);
	return dist(__retqss_random_generator);
}

int retQSS::random_int(int from, int to)
{
	std::uniform_int_distribution<int>dist(from, to);
	return dist(__retqss_random_generator);
}
