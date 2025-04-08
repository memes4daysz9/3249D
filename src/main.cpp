#include "main.h"

void Debug(){ // WIP, should return to SD card to debug the VEX control is Vex wants to screw with my auton

}
const int LowestBatt = 10;//base number, could be different could be different with each robot though

void BatteryChecker(){
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
		
		pros::screen::set_pen(0x00ee00); //green, Batteries Okay
		pros::screen::fill_rect(0,0,480,400);
		pros::screen::set_pen(0xffffff);
		pros::screen::print(pros::E_TEXT_SMALL, 1, "Battery: Nah, I'd win Battery Percentage: ",pros::battery::get_capacity());
		SpeedReduction = 0;
	}
}

void disabled() {
	pros::screen::print(pros::E_TEXT_SMALL, 3, "											");
	BatteryChecker();
	pros::Task suspend(Odometry); //holds off the task until needed
	
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
	Distance = 0;
	const int MinP = 650;
	
	float i;
	float Target = InchesToDegrees(dis);
	float error = Target - InchesToDegrees(Distance);
	float LError;
	while (abs(error) > InchesToDegrees(Tolerance)){

		error = Target - InchesToDegrees(Distance);
		pros::screen::print(pros::E_TEXT_SMALL, 8, "Error %f" , error);
		pros::screen::print(pros::E_TEXT_SMALL,9, "sgn %d" , sgn(error));
		pros::screen::print(pros::E_TEXT_SMALL,10, "T %d" , int(MinP * sgn(error)));
		i = (i + error) * kI;
		LeftMG.move_voltage((error * kP) + i + ((error - LError) * kD) + int(MinP * sgn(error)));
		RightMG.move_voltage((error * kP) + i + ((error - LError) * kD) + int(MinP * sgn(error)));
		LError = error;
		
	}
	LeftMG.set_brake_mode(pros::MotorBrake::brake);
	RightMG.set_brake_mode(pros::MotorBrake::brake);	
	LeftMG.brake();
	RightMG.brake();
	pros::delay(300); // allow motors to rest
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
		LError = error;
	}
	LeftMG.set_brake_mode(pros::MotorBrake::hold);
	RightMG.set_brake_mode(pros::MotorBrake::hold);	
	LeftMG.brake();
	RightMG.brake();
	pros::delay(300); // allow motors to rest
	return true;

}


void autonomous() {
	pros::MotorGroup LeftMG({FLP, MLP, BLP});
	pros::MotorGroup RightMG({FRP,MRP,BRP});

	pros::screen::print(pros::E_TEXT_SMALL, 3, "											");
	pros::screen::print(pros::E_TEXT_SMALL, 3, "Entering Autonomus Period");
	pros::Task Odom(Odometry);
	pros::delay(50);
	Move(5);
	Move(-5);
	Move(10);
	Move(-10);
	Move(20);
	Move(-20);
	pros::Task suspend(Odometry); //holds off the task until needed
	pros::Task remove(Odometry); // removes task, since X Y and Heading are all global variables, itll pass on to the next time odometry gets called
	LeftMG.set_brake_mode(pros::MotorBrake::coast);
	RightMG.set_brake_mode(pros::MotorBrake::coast);	
}

void opcontrol() {
	pros::Task Odom(Odometry);

	if (SpeedReduction == 0){ // if disabled() diddnt run OR if its at full power, check again after auton
		BatteryChecker();
	}
	//uncomment for testing

	float dir;
	float rot;
 	float left;
 	float right;
 	const float curve = 0.785;
	float lastdir;
	float MaxChange;
	float ChangeDir;
	float Change;
	float eLeft; // Extra FROM the left going TO the right
	float eRight; 
	float pLeft;
	float pRight;
	const float TModifier = 0.8; // turning modifier
	bool FullPower; //Overrides the acceleration control and the DT Modifier

	pros::Controller Cont(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup LeftMG({FLP, MLP, BLP});
	pros::MotorGroup RightMG({FRP,MRP,BRP});


	while (true) {
										/*				Drivetrain Movement				*/


		
		dir = Cont.get_analog(ANALOG_LEFT_Y);
		rot = (Cont.get_analog(ANALOG_RIGHT_X) * TModifier);
		//pros::screen::print(pros::E_TEXT_SMALL, 2, "forward: %f", dir);
		if (dir < 10){
			rot = rot/TModifier; //removing multiplier
		}
		left = dir + rot;
		right = dir - rot;


		LeftMG.move((100 * ((((1 - curve) * left) / 100 + (curve * pow(left / 100 , 7))))));
		RightMG.move((100 * ((((1 - curve) * right) / 100 + (curve * pow(right / 100 , 7))))));//input control curves go hard ngl

		
											/*			Battery Optimizing			*/
		//lastdir = dir; // trust me bro
		pros::delay((50 * SpeedReduction) + 5);
		//5, 55, 105


		

	}
}
