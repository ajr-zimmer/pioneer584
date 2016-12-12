/*
#include "Aria.h"
#include <iostream>
using namespace std;

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
	robot.enableMotors();
	robot.comInt(ArCommands::SOUNDTOG, 0);

	double speed = 400;
	double actions[33] = { 1, 0, 0, 0, 0, 0, 0, 1,
		0, 0, 0, 0, 0, 0, 1,
		0, 0, 0, 0, 0, 0, 1,
		0, 0, 0, 0, 0, 0, 1 };
	bool done = false;
	int blocked;
	int numSonar = robot.getNumSonar();
	int sonarMinRange = 800;
	// Robot Code here
	for (int x = 0; x < sizeof(actions) / sizeof(*actions);){
		blocked = 0;
		for (int i = 0; i < numSonar; i++) {
			if (robot.getSonarRange(i) <= sonarMinRange) {
				blocked++;
				robot.setVel(0);
				robot.setRotVel(0);
			}
		}
		if (blocked <1){
			if (actions[x] == 0){
				robot.setVel(speed);
				ArUtil::sleep(500);
			}
			else if (actions[x] == 1){
				robot.setRotVel(-90);
				ArUtil::sleep(1000);
				robot.setRotVel(0);
			}
			x++;
		}
		else{
			cout << "It seems you've hit something! \n";
		}
	}
	robot.setVel(0);
	robot.setRotVel(0);
	ArUtil::sleep(1000);

	// wait for robot task loop to end before exiting the program
	//robot.waitForRunExit();
	Aria::exit(0);
	return 0;
}
*/