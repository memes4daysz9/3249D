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
		pros::delay(1000);
		pros::screen::print(pros::E_TEXT_SMALL,1,"Left Side Motor temps: %3d, %3d, %3d", LeftMG.get_temperature(0),LeftMG.get_temperature(1),LeftMG.get_temperature(2));
		pros::screen::print(pros::E_TEXT_SMALL,1,"Right Side Motor temps: %3d, %3d, %3d", RightMG.get_temperature(0),RightMG.get_temperature(1),RightMG.get_temperature(2));
	}
}