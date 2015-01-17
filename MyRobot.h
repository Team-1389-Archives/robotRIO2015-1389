#ifndef MY_ROBOT_H
#define MY_ROBOT_H

#include "WPILib.h"
#include "controller-macros.h"
#include <ctime>
#include <math.h>

//Port Numbers
#define ENCODER_ONE_DIGIPORTS 6, 7
#define ENCODER_TWO_DIGIPORTS 8, 9
#define LIMIT_ONE_DIGIPORT 0
#define	LIMIT_TWO_DIGIPORT 1
#define FR_PWM_DRIVE_PORT 0
#define FL_PWM_DRIVE_PORT 1
#define BR_PWM_DRIVE_PORT 2
#define BL_PWM_DRIVE_PORT 3
#define ELEVATOR_PWM_PORT 4
#define ELEVATOR_TWO_PWM_PORT 5
#define DRIVE_JOYSTICK_PORT 0
#define FUNC_JOYSTICK_PORT 1
//#define TEST_VICTOR_PORT 4
//Constants used
#define ELEVATOR_SPEED 1
#define FOUR_WHEEL_LIMITER 1.41421356237


class MyRobot: public SampleRobot {

	const bool isDEBUGGING=true;
	int iterationNumber;
	float currTime = 0;
	Timer* threadTime;
	bool fin = false;
	bool neutral, up, temp;
	float modifier = 1;
	Joystick *driver, *func;
	DigitalInput *IRa, *IRb, *IRc, *IRd, *IRe, *limitOne, *limitTwo;
	Talon *FRdrive, *FLdrive, *BRdrive, *BLdrive;
	Victor *elevatorOne, *elevatorTwo;
	RobotDrive *drive;
	PowerDistributionPanel* smart;
	Encoder *cody, *codec;
public:
	MyRobot();
	void Autonomous();
	void OperatorControl();
	void LiftControl();
	void ConfigureEncoders();
	int abs(float num);
	float rampUp(float maxPower, float time);
	void FourCIMDrive();
	float LimitTeleOp();
	void mainInstance();
};


#endif //MY_ROBOT_H
