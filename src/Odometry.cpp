#include "main.h"

const double SL = 7;// "the left-right distance from the tracking center to the left tracking wheel"
const double SR = 8;// "the left-right distance from the tracking center to the right tracking wheel"
const double WheelD = 3.25;
constexpr double WheelR = 3.25 / 2; // does the math for me during compilation, the brain doesnt compute this value, my PC does
const double Ratio = 0.6; // gear ratio from motor to wheel
void Odometry(){

    pros::Motor ML(MLP); // middle motors with the idea that they average out the front and back motor movements
    pros::Motor MR(MRP);

    double DeltaL;
    double DeltaR;
    int RDeg;
    int LDeg; 
    double RDis;
    double LDis;
    double Heading = 0;
    double RHeading = 0;
    int LastL = 0;
    int LastR = 0;
    double LastH = 0;
    double DeltaH = 0;
    double LocalOffset[2]; // delta variants of x,y
    double AvgHeading;
    double DisOffset;
    

    while (true){
        LDeg = ML.get_position() * Ratio;
        RDeg = MR.get_position() * Ratio;
        
        DeltaL = (LDeg - LastL);
        DeltaR = (RDeg - LastR);

        LDis = DegToRad(DeltaL) * (WheelR);
        RDis = DegToRad(DeltaR) * (WheelR);

        Heading = LastH + double(((LDis - RDis)/(TrackLength))) * -1;

        DeltaH = Heading - LastH;
        DHeading = RadToDeg(Heading);
        DeltaH = Heading - LastH;

        if(DeltaH < 0.1) {
            if (RDis == 0) { 
                DisOffset = RDis;
            }
            else {
                DisOffset = LDis;
            }
        }else {

            if (RDis == 0) { 
                DisOffset = 2 * sin(Heading / 2) * (RDis / DeltaH + (TrackLength / 2));
            }
            else {
                DisOffset = 2 * sin(Heading / 2) * (LDis / DeltaH + (TrackLength / 2));
            }    
        }

        AvgHeading = LastH + DeltaH / 2;

        X = DisOffset * cos(-AvgHeading) + X;
        Y = DisOffset * sin(-AvgHeading) * -1 + Y;
        Distance += DisOffset; // for forward PIDs to work with a Rotation


        LastH = Heading;
        LastL = LDeg;
        LastR = RDeg;
        pros::delay(1);
        
        pros::screen::print(pros::E_TEXT_SMALL, 4, "X = %f" , X);
        pros::screen::print(pros::E_TEXT_SMALL, 5, "Y = %f" , Y);
        pros::screen::print(pros::E_TEXT_SMALL, 6, "Heading = %f" , DHeading);
        pros::screen::print(pros::E_TEXT_SMALL, 7, "Distance Offset %f" , Distance);
    }
}