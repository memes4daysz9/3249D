#include "main.h"

const double SL = 4;// "the left-right distance from the tracking center to the left tracking wheel"
const double SR = 4;// "the left-right distance from the tracking center to the right tracking wheel"
const double WheelD = 3.25;
constexpr double WheelR = 3.25 / 2; // does the math for me during compilation, the brain doesnt compute this value, my PC does
const double Ratio = 0.8; // gear ratio from motor to wheel
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

    while (true){
        LDeg = ML.get_position() * Ratio;
        RDeg = MR.get_position() * Ratio;
        
        DeltaL = (LDeg - LastL);
        DeltaR = (RDeg - LastR);

        LDis = DegToRad(DeltaL) * (WheelR / 2);
        RDis = DegToRad(DeltaR) * (WheelR / 2);

        Heading = LastH + double(((LDis - RDis)/(TrackLength))) * -1;

        DeltaH = Heading - LastH;
        DHeading = RadToDeg(Heading);
        DeltaH = Heading - LastH;
        if (DeltaH < 0.1){
            if (RDis > double(1)){
                LocalOffset[1] = RDis;
            }else{
                LocalOffset[0] = LDis;
            }
        }else {
            if (RDis == 0){
                LocalOffset[1] = double(2*sin(RHeading/2)*((RDis/DeltaH) + (TrackLength/2)));
            }else{
                LocalOffset[1] = double(2*sin(RHeading/2)*((LDis/DeltaH) + (TrackLength/2)));
            }
        }

        X += LocalOffset[0];
        Y += LocalOffset[1];

        LastH = Heading;
        LastL = LDeg;
        LastR = RDeg;
    }
}