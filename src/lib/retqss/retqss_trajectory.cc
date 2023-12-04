#include "retqss_trajectory.hh"
#include "retqss_exceptions.hh"


retQSS::Trajectory *retQSS::Trajectory::for_order(
		int order,
		double *x, double *y, double *z,
		double *tx, double *ty, double *tz,
		double *t0)
{
	if(order == 2)
		return new retQSS::Order2Trajectory(x,y,z,tx,ty,tz,t0);
	else if(order == 3)
		return new retQSS::Order3Trajectory(x,y,z,tx,ty,tz,t0);
	else
	{
		std::stringstream ss;
		ss << "Order not supported: " << order;
		throw retQSS::Exception(ss.str());
	}
}

retQSS::Trajectory::Trajectory(
		double *x, double *y, double *z,
		double *tx, double *ty, double *tz,
		double *t0) :
		x(x), y(y), z(z),
		t0(t0), tx(tx), ty(ty), tz(tz),
		last_tx(-1), last_ty(-1), last_tz(-1),
		was_updated(false)
{
}

retQSS::Order2Trajectory::Order2Trajectory(
		double *px, double *py, double *pz,
		double *tx, double *ty, double *tz,
		double *t0) :
		retQSS::Trajectory(px,py,pz,tx,ty,tz,t0)
{
	this->_x3_coeff = 0;
	this->_y3_coeff = 0;
	this->_z3_coeff = 0;
}

retQSS::Order3Trajectory::Order3Trajectory(
		double *px, double *py, double *pz,
		double *tx, double *ty, double *tz,
		double *t0) :
		retQSS::Trajectory(px,py,pz,tx,ty,tz,t0)
{
}

retQSS::Trajectory::~Trajectory()
{
}

Point_3 retQSS::Trajectory::operator()(double t) const
{
	double xt, yt, zt;

	this->at(t, &xt, &yt, &zt);

	return Point_3(xt, yt, zt);
}

int retQSS::Order2Trajectory::order() const
{
	return 2;
}

void retQSS::Order2Trajectory::at(
		double time,
		double *x,
		double *y,
		double *z) const
{
	*x = this->evaluate_poly(time - this->last_tx, this->x);
	*y = this->evaluate_poly(time - this->last_ty, this->y);
	*z = this->evaluate_poly(time - this->last_tz, this->z);
}

bool retQSS::Order2Trajectory::update()
{
	double new_tx = *this->tx;
	double new_ty = *this->ty;
	double new_tz = *this->tz;
	this->was_updated = false;

	if(this->last_tx != new_tx)
	{
		double x2_tx = x[2]*new_tx;
		this->_x0_coeff = x[0] - x[1]*new_tx + x2_tx*new_tx;
		this->_x1_coeff = x[1] - 2*x2_tx;
		this->_x2_coeff = x[2];
		this->last_tx = new_tx;
		this->was_updated = true;
	}

	if(this->last_ty != new_ty)
	{
		double y2_ty = y[2]*new_ty;
		this->_y0_coeff = y[0] - y[1]*new_ty + y2_ty*new_ty;
		this->_y1_coeff = y[1] - 2*y2_ty;
		this->_y2_coeff = y[2];
		this->last_ty = new_ty;
		this->was_updated = true;
	}

	if(this->last_tz != new_tz)
	{
		double z2_tz = z[2]*new_tz;
		this->_z0_coeff = z[0] - z[1]*new_tz + z2_tz*new_tz;
		this->_z1_coeff = z[1] - 2*z2_tz;
		this->_z2_coeff = z[2];
		this->last_tz = new_tz;
		this->was_updated = true;
	}

	this->current_t0 = *this->t0;

	return this->was_updated;
}

int retQSS::Order3Trajectory::order() const
{
	return 3;
}

void retQSS::Order3Trajectory::at(
		double time,
		double *x,
		double *y,
		double *z) const
{
	*x = this->evaluate_poly(time - this->last_tx, this->x);
	*y = this->evaluate_poly(time - this->last_ty, this->y);
	*z = this->evaluate_poly(time - this->last_tz, this->z);
}

bool retQSS::Order3Trajectory::update()
{
	double new_tx = *this->tx;
	double new_ty = *this->ty;
	double new_tz = *this->tz;
	this->was_updated = false;

	if(this->last_tx != new_tx)
	{
		double x2_tx = x[2]*new_tx;
		double x3_tx_tx = x[3]*new_tx*new_tx;
		this->_x0_coeff = x[0] - x[1]*new_tx + x2_tx*new_tx - x3_tx_tx*new_tx;
		this->_x1_coeff = x[1] - 2*x2_tx + 3*x3_tx_tx;
		this->_x2_coeff = x[2] - 3*x[3]*new_tx;
		this->_x3_coeff = x[3];
		this->last_tx = new_tx;
		this->was_updated = true;
	}

	if(this->last_ty != new_ty)
	{
		double y2_ty = y[2]*new_ty;
		double y3_ty_ty = y[3]*new_ty*new_ty;
		this->_y0_coeff = y[0] - y[1]*new_ty + y2_ty*new_ty - y3_ty_ty*new_ty;
		this->_y1_coeff = y[1] - 2*y2_ty + 3*y3_ty_ty;
		this->_y2_coeff = y[2] - 3*y[3]*new_ty;
		this->_y3_coeff = y[3];
		this->last_ty = new_ty;
		this->was_updated = true;
	}

	if(this->last_tz != new_tz)
	{
		double z2_tz = z[2]*new_tz;
		double z3_tz_tz = z[3]*new_tz*new_tz;
		this->_z0_coeff = z[0] - z[1]*new_tz + z2_tz*new_tz - z3_tz_tz*new_tz;
		this->_z1_coeff = z[1] - 2*z2_tz + 3*z3_tz_tz;
		this->_z2_coeff = z[2] - 3*z[3]*new_tz;
		this->_z3_coeff = z[3];
		this->last_tz = new_tz;
		this->was_updated = true;
	}

	this->current_t0 = *this->t0;

	return this->was_updated;
}
