#include "Aria.h"
#include "Hellow.cpp"
#include <thread>
#include <mutex>
#include <condition_variable>
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

	// turn on the motors, turn off amigobot sounds
	robot.enableMotors();
	robot.comInt(ArCommands::SOUNDTOG, 0);

	// Thread bullshit
	std::mutex m;
	std::condition_variable cv;
	std::string new_string;
	bool error = false;

	auto io_thread = std::thread([&]{
		std::string s;
		while (!error && std::getline(std::cin, s, '\n'))
		{
			auto lock = std::unique_lock<std::mutex>(m);
			new_string = std::move(s);
			if (new_string == "quit") {
				error = true;
			}
			else if (new_string == "up"){
				robot.setVel(400);	// robot moves forward at 400 mm/s				
			}
			else if (new_string == "down"){
				robot.setVel(0);
				cout << "Stopping robot...";
			}
			else if (new_string == "left"){
				// this is counter-clockwise
				robot.setRotVel(45);  // robot rotates at 45deg/s
				ArUtil::sleep(1000);  // go to sleep for 1 second
				robot.setRotVel(0);
			}
			else if (new_string == "right"){
				// this is clockwise
				robot.setRotVel(-45);  // robot rotates at 45deg/s
				ArUtil::sleep(1000);  // go to sleep for 1 second
				robot.setRotVel(0);
			}
			lock.unlock();
			cv.notify_all();
		}
		auto lock = std::unique_lock<std::mutex>(m);
		error = true;
		lock.unlock();
		cv.notify_all();
	});

	auto current_string = std::string();

	// Command loop
	while(1){
		// Thread code
		auto lock = std::unique_lock<std::mutex>(m);
		cv.wait(lock, [&] { return error || (current_string != new_string); });
		if (error)
		{
			break;
		}
		current_string = new_string;
		lock.unlock();

		// Sense obstructions		
		bool stop = false;
		int sonarMinRange = 800;	// should stop when encounter a wall or something at 800mm
		int numSonar = robot.getNumSonar();
		// Keep going forward until one of the sonars picks up something at the sonarMinRange
		//numSonar /= 2; // only used in Mobilesim to mimic the use of only the front sonar array
		while (!stop){
			for (int i = 0; i < numSonar; i++){
				if (robot.getSonarRange(i) <= sonarMinRange){
					robot.setVel(0);
					//register the stopping as a down command, so that the user cannot push forward
					new_string = "down";
					stop = true;
					cout << "It seems you've hit something! \n";
					break;
				}
			}
		}
		// now use the string that arrived from our non-blocking stream
		/*
		std::cout << "new string: " << current_string;
		std::cout.flush();
		for (int i = 0; i < 10; ++i) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << " " << i;
			std::cout.flush();
		}
		std::cout << ". done. next?\n";
		std::cout.flush();
		*/
	}
	io_thread.join();
	// wait for robot task loop to end before exiting the program
	//robot.waitForRunExit();
	Aria::exit(0);
	return 0;
}
