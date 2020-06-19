#include "robot.hpp"
#include "robot2core.hpp"
#include "robot2completion.hpp"
#include "robot2challenge.hpp"

int main() {
	
  int level;
  // Get use input on what class of robot to use
  std::cout<<"Please select what Robot Algorithm to use:\nCore: Enter '0'\nCompletion: Enter '1'\nChallenge: Enter '2'\n";
  std::cin>>level;
  // Create an instance of the corresponding robot based off user input
  if(level == 0){
    RobotCore r; 
	// Start running the robot
	r.runRobotCore();
  } else if(level == 1){
    RobotCompletion r(26.0, 26.0, 26.0);	 
	// Start running the robot
	r.runRobotCompletion(); 
  } else if(level == 2){
    RobotChallenge r(26.0, 26.0, 26.0);	
	// Start running the robot
	r.runRobotChallenge();  
  }

  return 0;
}
