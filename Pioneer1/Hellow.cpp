#include "Aria.h"
/*
int main(int argc, char **argv)
{
	Aria::init();
	ArRobot robot;

	ArArgumentParser argParser(&argc, argv);
	argParser.loadDefaultArguments();
	ArRobotConnector robotConnector(&argParser, &robot);

	if (!robotConnector.connectRobot())
	{
		ArLog::log(ArLog::Terse, "Could not connect to the robot.");
		if (argParser.checkHelpAndWarnUnparsed())
		{
			Aria::logOptions();
			Aria::exit(1);
			return 1;
		}
	}

	// Trigger argument parsing
	if (!Aria::parseArgs() || !argParser.checkHelpAndWarnUnparsed())
	{
		Aria::logOptions();
		Aria::exit(1);
		return 1;
	}

	ArKeyHandler keyHandler;
	Aria::setKeyHandler(&keyHandler);
	robot.attachKeyHandler(&keyHandler);

	ArSonarDevice sonar;
	robot.addRangeDevice(&sonar);

	robot.runAsync(true);

	// turn on the motors, turn off amigobot sounds
	robot.enableMotors();
	robot.comInt(ArCommands::SOUNDTOG, 0);

	robot.setVel(400);	// robot mvoes forward at 350 mm/s
	bool stop = false;
	int sonarMinRange = 700;	// should stop when encounter a wall or something at 500mm
	int numSonar = robot.getNumSonar();
	// Keep going forward until one of the sonars picks up something at the sonarMinRange
	while (!stop){
		for (int i = 0; i < numSonar; i++){
			if (robot.getSonarRange(i) <= sonarMinRange){
				robot.setVel(0);
				stop = true;
			}

		}
	}
	robot.setRotVel(90);  // robot rotates at 120deg/s
	ArUtil::sleep(2000);  // go to sleep in 2 seconds
	robot.setRotVel(0);


	// wait for robot task loop to end before exiting the program
	robot.waitForRunExit();

	Aria::exit(0);
	return 0;
}
*/