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
	error_sum = 0.0;

	counter = 0;
}

void PID::UpdateError(double cte) {
}

double PID::TotalError() {
}

