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
	//Victor *testMotor;
	Victor *elevatorOne, *elevatorTwo;
	RobotDrive *drive;
	PowerDistributionPanel* smart;
	Encoder *cody, *codec;

public:
	/**Constructor
	 *
	 */
	MyRobot()

{
		mainInstance();
		//ConfigureEncoders();
}

	/**
	 * Autonomous code
	 */
	void Autonomous() {
		const double defaultSpeed = -0.5;
		while(!limitOne->Get() && ! limitTwo->Get() && IsAutonomous()){
			drive->ArcadeDrive(defaultSpeed, 0);
		}

		while(IsAutonomous()){
			double turn = 0.0;
			double speed = defaultSpeed;
			if(!limitOne->Get()){
				turn -= 0.25;
				speed = 0.2;
			}
			if(!limitTwo->Get()){
				turn += 0.25;
				speed = 0.2;
			}
			drive->ArcadeDrive(speed, turn);
		}
	}

	/**
	 * Teleop Code
	 */
	void OperatorControl() {
		threadTime->Start(); //mr chen was here
		while (IsOperatorControl()) {
			//testMotor->Set(1);

			SmartDashboard::PutNumber("PWMValue", codec->GetRate());
			//SmartDashboard::PutNumber("PWM Value", tester->GetRaw());
			SmartDashboard::PutNumber("Limit One value", limitOne->Get());
			SmartDashboard::PutNumber("Limit Two value", limitTwo->Get());
			//	SmartDashboard::PutNumber("Encoder One value", cody->GetRate());
			//	SmartDashboard::PutNumber("Encoder Two value", codec->GetRate());
			SmartDashboard::PutNumber("LeftAnalog X axis",
					driver->GetRawAxis(LeftX));
			SmartDashboard::PutNumber("LeftAnalog Y axis",
					driver->GetRawAxis(LeftY));

			elevatorOne->Set(rampUp(1,12));

			//LiftControl();
			FourCIMDrive();
		}

		threadTime->Reset();
	}

	/**
	 * LiftControl
	 * checks if lift is at top with infrared sensor
	 *
	 */
	void LiftControl() {
		float rightAnalogPush = func->GetRawAxis(RightY) * -1;
		int direction = 0;
		bool locOne = IRa->Get();
		bool locTwo = IRb->Get();
		bool locThree = IRc->Get();
		bool locFour = IRd->Get();
		bool locFive = IRe->Get();


		if (rightAnalogPush < -.5
				&& !locOne) {
			direction = -1;
			elevatorOne->Set(ELEVATOR_SPEED * -1);
		}
		if (rightAnalogPush > .5 && !locTwo)
		{
			direction = 1;
			elevatorOne->Set(ELEVATOR_SPEED * -1);
		}
	}

/**
 * ConfigureEncoders
 * sets parameters of Encoders
 */
void ConfigureEncoders() {
	cody->SetMaxPeriod(.1);
	cody->SetMinRate(10);
	cody->SetDistancePerPulse(5);
	cody->SetReverseDirection(true);
	cody->SetSamplesToAverage(7);

	codec->SetMaxPeriod(.1);
	codec->SetMinRate(10);
	codec->SetDistancePerPulse(5);
	codec->SetReverseDirection(true);
	codec->SetSamplesToAverage(7);
}

/**
 * absolute value
 * returns the absolute value of a number
 **/
int abs(float num)
{
	if (num < 0)
		return num * -1;
	return num;
}

bool nuetralToPush()
{

	float magnitude = sqrt(pow(driver->GetRawAxis(LeftX), 2) + pow(driver->GetRawAxis(LeftY), 2));
	neutral = (magnitude < .2);
	if (neutral)
		temp = true;
	up = (magnitude > .2);

	if (up && temp)
	{
		return true;
		temp = false;
	}
	return false;
}

float rampUp(float maxPower, float time)
{

	currTime = threadTime->Get();
	float power = 0;
	SmartDashboard::PutNumber("TIME", currTime);
	bool fin = currTime >= time;
	if (!fin)
	{
		power = (-2 * maxPower / pow(time, 3) * pow(currTime, 3)) + (3 * maxPower / pow(time,2) * pow(currTime, 2));
		SmartDashboard::PutNumber("MotorPower", power);
		return power;
	}
	else if (fin)
	{
		SmartDashboard::PutNumber("fin power: " , power);
		return maxPower;
	}
	return 0;
}


//main Drive code
void FourCIMDrive() {
	float x, y;
	bool ramp = false;
	x = driver->GetRawAxis(LeftX);
	y = driver->GetRawAxis(LeftY);
	float time;

	float turn=LimitTeleOp();
	SmartDashboard::PutNumber("turn: ",turn);

	if (driver->GetRawButton(BumperR))
	{
		modifier = .5;
	}
	if (driver->GetRawButton(BumperL))
	{
		modifier = .75;
	}
	if (driver->GetRawAxis(RightTrigger) > .4)
	{
		modifier = 1;
	}

	if (driver->GetRawButton(ButtonB))
	{
		//BLdrive ->Set((y + x) / FOUR_WHEEL_LIMITER);
		if(turn!=0)
			x=turn;
	}
	SmartDashboard::PutBoolean("B Button Press", driver->GetRawButton(ButtonB));
	SmartDashboard::PutNumber("x: ",x);
	SmartDashboard::PutNumber("Speed modifier", modifier);

	if (nuetralToPush())
	{
		time = threadTime->Get();
	}
	if ((threadTime->Get() - time < .125) && nuetralToPush())
	{
		FRdrive -> Set(modifier * rampUp((x - y) / FOUR_WHEEL_LIMITER, .125));
		FLdrive -> Set(modifier * rampUp((x + y) / FOUR_WHEEL_LIMITER, .125));
		BRdrive -> Set(modifier * rampUp((x - y) / FOUR_WHEEL_LIMITER, .125));
		BLdrive -> Set(modifier * rampUp((x + y) / FOUR_WHEEL_LIMITER, .125));
	}
	else
	{
		FRdrive -> Set(modifier * ((x - y) / FOUR_WHEEL_LIMITER));
		FLdrive -> Set(modifier * ((x + y) / FOUR_WHEEL_LIMITER));
		BLdrive -> Set(modifier * ((x - y) / FOUR_WHEEL_LIMITER));
		BRdrive -> Set(modifier * ((x + y) / FOUR_WHEEL_LIMITER));
	}
	//drive->ArcadeDrive(y , x);
}

//Turns robot appropriately when either one of the limit switches are turned on
float LimitTeleOp(){
	float turn = 0.0;


	if(limitOne->Get()){
		turn += 1;
	}
	if(limitTwo->Get()){
		turn -= 1;
	}
	return turn;
}

void mainInstance()
{
	threadTime = new Timer();
	threadTime->Reset();
	cody = new Encoder(ENCODER_ONE_DIGIPORTS, false, Encoder::k4X);
	codec = new Encoder(ENCODER_TWO_DIGIPORTS, false, Encoder::k4X);
	limitOne = new DigitalInput(LIMIT_ONE_DIGIPORT);
	limitTwo = new DigitalInput(LIMIT_TWO_DIGIPORT);
	FRdrive = new Talon(FR_PWM_DRIVE_PORT);
	FLdrive = new Talon(FL_PWM_DRIVE_PORT);
	BRdrive = new Talon(BR_PWM_DRIVE_PORT);
	BLdrive = new Talon(BL_PWM_DRIVE_PORT);
	elevatorOne = new Victor(ELEVATOR_PWM_PORT);
	elevatorTwo = new Victor(ELEVATOR_TWO_PWM_PORT);
	func = new Joystick(FUNC_JOYSTICK_PORT);
	driver = new Joystick(DRIVE_JOYSTICK_PORT);
	smart = new PowerDistributionPanel();
	//testMotor = new Victor(6);
	drive = new RobotDrive(FLdrive, BLdrive, FRdrive, BRdrive);
}

};

START_ROBOT_CLASS(MyRobot);
