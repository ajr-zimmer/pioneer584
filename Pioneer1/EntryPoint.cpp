#include "Aria.h"
#include "Hellow.cpp"
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

	//robot.runAsync(true);
	robot.runAsync(false);

	char input;
	cout << "Hello and Welcome, press 'w' or 'W' for the Hello World!\n";
	cin >> input;
	cout << "The value you entered is " << input << "\n";
	
	//Call Hello World
	if (input == 'w' || input == 'W'){
		cout << "About to start Hellow World...\n";
		Hellow w;
		w.perform_test(robot);
	}


	// wait for robot task loop to end before exiting the program
	robot.waitForRunExit();

	Aria::exit(0);
	return 0;
}
