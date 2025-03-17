#include "main.h"

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::Motor FL (FLP,pros::MotorGears::blue,pros::v5::MotorUnits::degrees);
	pros::Motor FR (FRP,pros::MotorGears::blue,pros::v5::MotorUnits::degrees);
	pros::Motor ML (MLP,pros::MotorGears::blue,pros::v5::MotorUnits::degrees);//in case of needed finer Control
	pros::Motor MR (MRP,pros::MotorGears::blue,pros::v5::MotorUnits::degrees);
	pros::Motor BL (BLP,pros::MotorGears::blue,pros::v5::MotorUnits::degrees);
	pros::Motor BR (BRP,pros::MotorGears::blue,pros::v5::MotorUnits::degrees);


    pros::MotorGroup LeftMG ({FLP,MLP,BLP},pros::MotorGears::blue,pros::v5::MotorUnits::degrees);
    pros::MotorGroup RightMG ({FRP,MRP,BRP},pros::MotorGears::blue,pros::v5::MotorUnits::degrees);

}