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
	/*
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
	*/
	// turn on the motors, turn off amigobot sounds
	robot.enableMotors();
	robot.comInt(ArCommands::SOUNDTOG, 0);

	while(1){
		// TODO: Conditionals Here
		string fromSwipe;
		//cout << "Hello and Welcome, send me an upswipe!\n";
		cin >> fromSwipe;
		if (!fromSwipe.compare("up")){
			robot.setVel(400);	// robot moves forward at 350 mm/s
			bool stop = false;
			int sonarMinRange = 800;	// should stop when encounter a wall or something at 500mm
			int numSonar = robot.getNumSonar();
			// Keep going forward until one of the sonars picks up something at the sonarMinRange
			//numSonar /= 2; // only used in Mobilesim to mimic the use of only the front sonar array
			while (!stop){
				for (int i = 0; i < numSonar; i++){
					if (robot.getSonarRange(i) <= sonarMinRange){
						robot.setVel(0);
						stop = true;
						cout << "It seems you've hit something!";
					}
				}
				cin >> fromSwipe; // this line seems to mess it all up. Causes the sonar array to be ignored
				if (fromSwipe.compare("up")){ // anything but up
					robot.setVel(0);
					stop = true;
					cout << "Stopping robot...";
				}
			}
		}
		else if (!fromSwipe.compare("down")){
			//ArUtil::sleep(2000);  // go to sleep for 2 seconds
			robot.setVel(0);
		}
		else if (!fromSwipe.compare("left")){
			// this is counter-clockwise
			robot.setRotVel(45);  // robot rotates at 45deg/s
			ArUtil::sleep(1000);  // go to sleep for 1 second
			robot.setRotVel(0);
		}
		else if (!fromSwipe.compare("right")){
			// this is clockwise
			robot.setRotVel(-45);  // robot rotates at 45deg/s
			ArUtil::sleep(1000);  // go to sleep for 1 second
			robot.setRotVel(0);
		}
		else {
			cout << "I have no idea what you want";
		}
	}
	// wait for robot task loop to end before exiting the program
	robot.waitForRunExit();

	Aria::exit(0);
	return 0;
}
