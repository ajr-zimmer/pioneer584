#include "Aria.h"
#include <iostream>
using namespace std;
class Hellow {
public:
	void perform_test(ArRobot robot){
		cout << "Starting the Hellow World!\n";
		//ArRobot robot = r;

		// turn on the motors, turn off amigobot sounds
		robot.enableMotors();
		robot.comInt(ArCommands::SOUNDTOG, 0);
		cout << "What is going on1?!\n";

		robot.lock();
		robot.setVel(400);	// robot moves forward at 350 mm/s
		robot.unlock();
		bool stop = false;
		int sonarMinRange = 700;	// should stop when encounter a wall or something at 500mm
		int numSonar = robot.getNumSonar();
		// Keep going forward until one of the sonars picks up something at the sonarMinRange
		cout << "What is going on2?!\n";
		while (!stop){			
			for (int i = 0; i < numSonar; i++){
				cout << "What is going on3?!" << i << "\n";
				if (robot.getSonarRange(i) <= sonarMinRange){
					robot.lock();
					robot.setVel(0);
					robot.unlock();
					stop = true;
				}
			}
		}
		robot.lock();
		robot.setRotVel(90);  // robot rotates at 120deg/s
		robot.unlock();
		ArUtil::sleep(2000);  // go to sleep in 2 seconds
		robot.lock();
		robot.setRotVel(0);
		robot.unlock();
		ArUtil::sleep(200);
	}
};
