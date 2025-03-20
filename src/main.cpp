#include "main.h"

void Debug(){ // WIP, should return to SD card to debug the VEX control is Vex wants to screw with my auton

}
const int LowestBatt = 30;//base number, could be different could be different with each robot though
void disabled() {
	pros::screen::print(pros::E_TEXT_SMALL, 3, "											");
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
		pros::screen::print(pros::E_TEXT_SMALL, 1, "Battery: Nah, I'd win Battery Percentage: ",pros::battery::get_capacity());
		SpeedReduction = 0;
	}
	

	while (true){
		if (SpeedReduction > 0){
			pros::delay(500 * SpeedReduction);
		}else {
			pros::screen::print(pros::E_TEXT_SMALL, 1, "Battery OK. Battery Percentage: ",pros::battery::get_capacity());
			pros::screen::print(pros::E_TEXT_SMALL, 2, "Controller Battery",ControllerBatt);
		}
	}	
	pros::screen::print(pros::E_TEXT_SMALL, 3, "Leaving Disabled Period!");
	

} //Log pre match data

void competition_initialize() {
	pros::screen::print(pros::E_TEXT_SMALL, 3, "											");
	pros::screen::print(pros::E_TEXT_SMALL, 3, "Went Through Competition Initialize Period!");
}

									/*			Auton Defining			*/

bool Move(float dis){ /*			Forward			*/
	pros::Motor ML(MLP);
	pros::Motor MR(MRP);
	pros::MotorGroup LeftMG({FLP, MLP, BLP});
	pros::MotorGroup RightMG({FRP,MRP,BRP});

	
	float i;
	float Target = InchesToDegrees(dis) + ML.get_position();
	float error = Target - ML.get_position();
	float LError;
	while (abs(error) > InchesToDegrees(Tolerance)){

		error = Target - ML.get_position();
		i = (i+error) * kI;
		LeftMG.move_voltage((error * kP)+ i + ((error - LError) * kD));
		RightMG.move_voltage((error * kP)+ i + ((error - LError) * kD));
		pros::delay(10 * SpeedReduction);
		LError = error;
		
	}
	return true;
}

bool Rotate(float deg){ /*			Rotation			*/

	pros::MotorGroup LeftMG({FLP, MLP, BLP});
	pros::MotorGroup RightMG({FRP,MRP,BRP});
	float LError;
	float i;
	float Target = DHeading + deg;
	float error = Target - DHeading;

	while (abs(error) > RotationToDegrees(Tolerance)){
		i = (i+error) * kI;

		LeftMG.move_voltage((error * kP)+ i + ((error - LError) * kD));
		RightMG.move_voltage(((error * kP)+ i + ((error - LError) * kD)) * -1);
		pros::delay(10 * SpeedReduction);
		LError = error;
	}

}


void autonomous() {
	pros::screen::print(pros::E_TEXT_SMALL, 3, "											");
	pros::screen::print(pros::E_TEXT_SMALL, 3, "Entering Autonomus Period");
	pros::Task Odom(Odometry);
}

void opcontrol() {
	//pros::Task Odom(Odometry);
	//uncomment for testing

	float dir;
	float Lrot;
	float Rrot;
 	float left;
 	float right;
 	float curve;
	float lastdir;
	float MaxChange;
	float ChangeDir;
	float Change;
	float ContX;
	float ContY;


	pros::Controller Cont(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup LeftMG({FLP, MLP, BLP});
	pros::MotorGroup RightMG({FRP,MRP,BRP});


	while (true) {

														/*			Drivetrain			*/

		ContX = Cont.get_analog(ANALOG_RIGHT_X);
		ContY = Cont.get_analog(ANALOG_LEFT_Y);

		dir = ContY;

		Lrot = abs(ContX * (ContX > 0)) * (TurnFactor); // if its greater than 0, then the left can add a certain amount and reduce it by a factor. abs isnt needed in this area, just makes it look right
		Rrot = abs(ContX * (ContX < 0)) * (TurnFactor);// does the negative values of controller Left X

		left = dir + Lrot;
		right = dir + Rrot;


		LeftMG.move_voltage((120 * (100 * ((((1 - curve) * left) / 100 + (curve * pow(left / 100 , 7)))))));
		RightMG.move_voltage((120 * (100 * ((((1 - curve) * left) / 100 + (curve * pow(left / 100 , 7)))))));//input control curves go hard ngl

		
											/*			Battery Optimizing			*/
										
		pros::delay(150 * SpeedReduction);
		// High = 0 , Middle = 250 , Low = 500

											/*			Acceleration Curve			*/

		// if the change in controller Direction is too much, then slow down the deaccel to keep the robot from tipping / breaking components

		Change = dir - lastdir;
		if(abs(Change) > MaxChange){
			dir = (Change + (dir/2));
		}
		lastdir = dir;
		

	}
}