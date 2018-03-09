#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	PID::Kp = Kp;
	PID::Ki = Ki;
	PID::Kd = Kd;

	d_error = i_error = p_error = 0.0;
	prev_cross_track_error = 0.0;
}

void PID::UpdateError(double cte) {
	p_error = cte;
	i_error += cte;
	d_error = cte - prev_cross_track_error;
	prev_cross_track_error = cte;
}

double PID::TotalError(void)
{
	return p_error * Kp + i_error * Ki + d_error * Kd;
}

