#include "main.h"

void Debug(){ // WIP, should return to SD card to debug the VEX control is Vex wants to screw with my auton

}
const int LowestBatt = 30;//base number, could be different could be different with each robot though
void disabled() {
	if (pros::battery::get_capacity() <= LowestBatt){ //FILL SCREEN with Pen color

		pros::screen::set_pen(0xff0000); //red, CHANGE BATTERY NOW
		pros::screen::fill_rect(0,0,480,400);
		pros::screen::set_pen(0xffffff);
		pros::screen::print(pros::E_TEXT_SMALL, 1, "WARNING! BATTERY LOW	Battery Percentage: ",pros::battery::get_capacity());
		SpeedReduction = 2;


	}else if (pros::battery::get_capacity() <= (LowestBatt * 2)){

		pros::screen::set_pen(0xffff00); //yellow, May wanna change
		pros::screen::fill_rect(0,0,480,400);
		pros::screen::set_pen(0xffffff);
		pros::screen::print(pros::E_TEXT_SMALL, 1, "Take Caution last round	Battery Percentage: ",pros::battery::get_capacity());
		SpeedReduction = 1;

	}else{
		
		pros::screen::set_pen(0x00ff00); //green, Batteries Okay
		pros::screen::fill_rect(0,0,480,400);
		pros::screen::set_pen(0xffffff);
		pros::screen::print(pros::E_TEXT_SMALL, 1, "Battery OK. Battery Percentage: ",pros::battery::get_capacity());
		SpeedReduction = 0;
	}
	

	while (true){
		if (SpeedReduction > 0){
			pros::delay(500);
		}else {
			pros::screen::print(pros::E_TEXT_SMALL, 1, "Battery OK. Battery Percentage: ",pros::battery::get_capacity());
			//pros::screen::print(pros::E_TEXT_SMALL, 2, "Controller Battery",pros::battery::get_capacity());
		}
	}

} //Log pre match data

void competition_initialize() {}

void autonomous() {
	pros::Task Odom(Odometry);
}

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