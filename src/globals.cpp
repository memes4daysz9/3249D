#include "main.h"

/*                  Motor Config                */

extern std::int8_t FLP = 1;
extern std::int8_t FRP = 2;
extern std::int8_t MLP = 3;
extern std::int8_t MRP = 4; //unsigned 8 bit integer
extern std::int8_t BLP = 5; //for ports
extern std::int8_t BRP = 6;

/*              Tracking                */

extern int8_t ControllerBatt = 0;

extern int RDeg = 0;
extern int LDeg = 0; // Tracks the Degree from the motors

extern double X = 0;
extern double Y = 0; // Local X , Y

extern constexpr double diameter = 3.25;
extern constexpr double radius = 3.25/2;
extern constexpr double TrackLength = 8;
extern constexpr double Tolerance = 0.1; // 0.1 inches and 0.1 degrees
extern double DHeading = 0; // Heading in Degrees
extern const double kP = 10;
extern const double kI = 0.7;
extern const double kD = 3;

extern double InchesToDegrees(float distance){return distance/((M_PI * diameter)/360);}

extern double RotationToDegrees(double degrees){return (((degrees * (M_PI / 180))*(TrackLength/2))/diameter*360)/2;}


extern double DegToRad(double x) {
    return (x * (M_PI/180));
}
extern double RadToDeg(double x) {
    return (x* (180/M_PI));
}

extern int sgn(double x){return (x>0) - (x<0);}

/*              Battery Saving Functions                */

extern int8_t SpeedReduction = 0;

/*              Drivetrain          */
extern float TurnFactor = 0.75;