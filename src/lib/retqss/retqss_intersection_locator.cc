#include "retqss_intersection_locator.hh"
#include "retqss_utilities.hh"
#include "retqss_exceptions.hh"


retQSS::IntersectionLocator *retQSS::IntersectionLocator::for_order(
		int order)
{
	if(order == 2)
		return new retQSS::Order2IntersectionLocator;
	else if(order == 3)
		return new retQSS::Order3IntersectionLocator;
	else
	{
		std::stringstream ss;
		ss << "Order not supported: " << order;
		throw retQSS::Exception(ss.str());
	}
}

void retQSS::IntersectionLocator::evaluate_roots(
		const retQSS::Trajectory &trajectory,
		const retQSS::PolyhedronFace &face,
		double *roots,
		int n_roots)
{
	double x, y, z;

	this->intersection.invalidate();

	for(int i = 0; i < n_roots; ++i)
	{
		if(!trajectory.in_domain(roots[i]))
			continue;

		trajectory.at(roots[i],&x,&y,&z);

		if(face.is_point_inside(x,y,z))
		{
			this->intersection.set(roots[i],x,y,z);
			return;
		}
	}
}

const retQSS::IntersectionData&
retQSS::Order2IntersectionLocator::intersection_of(
		const retQSS::Trajectory &trajectory,
		const retQSS::PolyhedronFace &face)
{
	double x, y, z;
	double p0, p1, p2;
	double r1;

	this->intersection.invalidate();

	p0 =
		face._nx * trajectory.x0_coeff() +
		face._ny * trajectory.y0_coeff() +
		face._nz * trajectory.z0_coeff() +
		face._sub_n_o;

	p1 =
		face._nx * trajectory.x1_coeff() +
		face._ny * trajectory.y1_coeff() +
		face._nz * trajectory.z1_coeff();

	p2 =
		face._nx * trajectory.x2_coeff() +
		face._ny * trajectory.y2_coeff() +
		face._nz * trajectory.z2_coeff();

	if(p2 == 0)
	{
		if(p1 != 0)
		{
			r1 = -p0 / p1;

			if(trajectory.in_domain(r1))
			{
				trajectory.at(r1,&x,&y,&z);

				if(face.is_point_inside(x,y,z))
				{
					this->intersection.set(r1,x,y,z);
					return this->intersection;
				}
			}
		}
	}
	else
	{
		double disc;
		disc = p1 * p1 - 4 * p2 * p0;

		if(disc >= 0)
		{
			double sd, p22, r2;

			sd = sqrt(disc);
			p22 = 2*p2;

			r1 = (-p1 + sd) / p22;
			r2 = (-p1 - sd) / p22;

			if(r1 > r2)
				std::swap(r1, r2);

			if(trajectory.in_domain(r1))
			{
				trajectory.at(r1,&x,&y,&z);

				if(face.is_point_inside(x,y,z))
				{
					this->intersection.set(r1,x,y,z);
					return this->intersection;
				}
			}

			if(trajectory.in_domain(r2))
			{
				trajectory.at(r2,&x,&y,&z);

				if(face.is_point_inside(x,y,z))
				{
					this->intersection.set(r2,x,y,z);
					return this->intersection;
				}
			}
		}
	}

	return this->intersection;
}

const retQSS::IntersectionData&
retQSS::Order3IntersectionLocator::intersection_of(
		const retQSS::Trajectory &trajectory,
		const retQSS::PolyhedronFace &face)
{
	int n_roots;
	double roots[3], poly[4];

	poly[0] =
		face._nx * trajectory.x0_coeff() +
		face._ny * trajectory.y0_coeff() +
		face._nz * trajectory.z0_coeff() +
		face._sub_n_o;

	poly[1] =
		face._nx * trajectory.x1_coeff() +
		face._ny * trajectory.y1_coeff() +
		face._nz * trajectory.z1_coeff();

	poly[2] =
		face._nx * trajectory.x2_coeff() +
		face._ny * trajectory.y2_coeff() +
		face._nz * trajectory.z2_coeff();

	poly[3] =
		face._nx * trajectory.x3_coeff() +
		face._ny * trajectory.y3_coeff() +
		face._nz * trajectory.z3_coeff();

	retQSS::poly_roots(poly, roots, &n_roots);

	this->evaluate_roots(trajectory, face, roots, n_roots);

	return this->intersection;
}
