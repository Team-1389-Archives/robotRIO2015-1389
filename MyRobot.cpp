#include "MyRobot.h"



MyRobot::MyRobot()
{
	mainInstance();
	//ConfigureEncoders();
}

void MyRobot::Autonomous() {

}

/**
 * Teleop Code
 */
void MyRobot::OperatorControl() {
	threadTime->Start(); //mr chen was here
	while (IsOperatorControl()) {
		//testMotor->Set(1);
		elevatorOne->Set(rampUp(1,12));
		FourCIMDrive();
	}

	threadTime->Reset();
}

/**
 * LiftControl
 * checks if lift is at top with infrared sensor
 *
 */
void MyRobot::LiftControl() {
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
void MyRobot::ConfigureEncoders() {
	cody->SetMaxPeriod(.1);
	cody->SetMinRate(10);
	cody->SetDistancePerPulse(5);

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
int MyRobot::abs(float num)
{
	if (num < 0)
		return num * -1;
	return num;
}


float MyRobot::rampUp(float maxPower, float time)
{

	currTime = threadTime->Get();
	float power = 0;
	bool finished = currTime >= time;
	if (!finished)
	{
		power = (-2 * maxPower / pow(time, 3) * pow(currTime, 3)) + (3 * maxPower / pow(time,2) * pow(currTime, 2));
		return power;
	}
	else if (finished)
	{
		return maxPower;
	}
	return 0;
}


//main Drive code
void MyRobot::FourCIMDrive() {
	float x, y;
	x = driver->GetRawAxis(LeftX);
	y = driver->GetRawAxis(LeftY);

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
		if(turn!=0)
			x=turn;
	}

	FRdrive -> Set(modifier * rampUp((x - y) / FOUR_WHEEL_LIMITER, .125));
	FLdrive -> Set(modifier * rampUp((x + y) / FOUR_WHEEL_LIMITER, .125));
	BRdrive -> Set(modifier * rampUp((x - y) / FOUR_WHEEL_LIMITER, .125));
	BLdrive -> Set(modifier * rampUp((x + y) / FOUR_WHEEL_LIMITER, .125));
}

//Turns robot appropriately when either one of the limit switches are turned on
float MyRobot::LimitTeleOp(){
	float turn = 0.0;


	if(limitOne->Get()){
		turn += 1;
	}
	if(limitTwo->Get()){
		turn -= 1;
	}
	return turn;
}

void MyRobot::mainInstance()
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
