#include "Aria.h"
/** @example teleopActionsExample.cpp  Example using actions for safe
* teleoperation with keyboard or joystick.

This example program creates several "limiting" actions (stop the robot
from hitting detected obstacles), as well as Joydrive and Keydrive
actions which request movement based on keys pressed or a joystick
attached to the computer. The limiting actions are added at a higher
priority than the teleoperation actions, so they prevent those actions
from driving the robot if nearby obstacles are detected; otherwise,
you can drive the robot using they joystick or keyboard.
*/

int derp(int argc, char **argv)//temporary renamed to derp from main
{
	Aria::init();
	ArArgumentParser parser(&argc, argv);
	parser.loadDefaultArguments();
	ArRobot robot;
	ArRobotConnector robotConnector(&parser, &robot);
	if (!robotConnector.connectRobot())
	{
		ArLog::log(ArLog::Terse, "ManualControl: Could not connect to the robot.");
		if (parser.checkHelpAndWarnUnparsed())
		{
			Aria::logOptions();
			Aria::exit(1);
		}
	}
	if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
	{
		Aria::logOptions();
		Aria::exit(1);
	}
	ArLog::log(ArLog::Normal, "ManualControl: Connected.");


	// limiter for close obstacles
	ArActionLimiterForwards limiter("speed limiter near", 300, 600, 250);
	// limiter for far away obstacles
	ArActionLimiterForwards limiterFar("speed limiter far", 300, 1100, 400);
	// limiter so we don't bump things backwards
	ArActionLimiterBackwards backwardsLimiter;
	// the keydrive action
	ArActionKeydrive keydriveAct;

	// sonar device, used by the limiter actions.
	ArSonarDevice sonar;

	printf("This program will allow you to use the keyboard to control the robot.\nYou can use the arrow keys to drive, and the spacebar to stop.\nPress escape to exit.\n");
	
	// add the sonar to the robot
	robot.addRangeDevice(&sonar);
	
	// set the robots maximum velocity (sonar don't work at all well if you're
	// going faster)
	robot.setAbsoluteMaxTransVel(400);

	// enable the motor
	robot.enableMotors();

	// Add the actions, with the limiters as highest priority, then the teleop.
	// actions.  This will keep the teleop. actions from being able to drive too 
	// fast and hit something
	robot.addAction(&limiter, 95);
	robot.addAction(&limiterFar, 90);
	robot.addAction(&backwardsLimiter, 85);
	robot.addAction(&keydriveAct, 45);

	// run the robot, true means that the run will exit if connection lost
	robot.run(true);

	Aria::exit(0);
	return 0;
}
