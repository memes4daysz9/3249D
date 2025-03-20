#include "main.h"

/*                  Motor Config                */

extern std::int8_t FLP;
extern std::int8_t FRP;
extern std::int8_t MLP;//       unsigned 8 bit integer
extern std::int8_t MRP;
extern std::int8_t BLP;
extern std::int8_t BRP;

/*              Tracking                */

extern int RDeg;
extern int LDeg;

extern double X;
extern double Y;

extern double InchesToDegrees(float distance);
extern double RotationToDegrees(double degrees);
extern double DegToRad(double x);
extern double RadToDeg(double x);
extern int sgn(double x);
extern void AutoTune();
extern void Odometry();
extern const double diameter;
extern const double TrackLength;
extern const double Tolerance;
extern const double radius;
extern double DHeading;

extern int8_t ControllerBatt;

/*              Autonomus               */

extern void MoveAllMotors(float Voltage);
extern void MoveXMotorsF(float Voltage);

extern const double kP;
extern const double kI;
extern const double kD;


/*              Pure Pursuit                */

extern const int PathLimit;
extern int CurrLine;
extern float Paths[][2];

/*              Battery Saving Functions                */

extern int8_t SpeedReduction;

/*              Drivetrain          */
extern float TurnFactor;