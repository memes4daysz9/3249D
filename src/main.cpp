#include "main.h"

void disabled() {



} //Log pre match data

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
	//pros::Task Odom(Odometry);
	//uncomment for testing

	float dir;
	float rot;
 	float left;
 	float right;
 	float curve;
	float lastdir;
	float MaxChange;
	float ChangeDir;
	float Change;

	pros::Controller Cont(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup LeftMG({FLP, MLP, BLP});
	pros::MotorGroup RightMG({FRP,MRP,BRP});


	while (true) {
										/*				Basic Movement				*/


		dir = Cont.get_analog(ANALOG_LEFT_Y);
		rot = Cont.get_analog(ANALOG_RIGHT_X);

		left = dir + rot;
		right = dir - rot;

		LeftMG.move_voltage((120 * (100 * ((((1 - curve) * left) / 100 + (curve * pow(left / 100 , 7)))))));
		RightMG.move_voltage((120 * (100 * ((((1 - curve) * left) / 100 + (curve * pow(left / 100 , 7)))))));//input control curves go hard ngl

		// if the change in controller Direction is too much, then slow down the deaccel to keep the robot from tipping / breaking components
		pros::delay(20);


											/*			Acceleration Curve			*/

											
		Change = dir - lastdir;
		if(abs(Change) > MaxChange){
			dir = (Change + (dir/2));
		}
		lastdir = dir;

	}
}