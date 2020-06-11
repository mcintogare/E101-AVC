class Robot {

public:
	double vLeft, vRight, v_go;
	double kp = 0.1;
	Robot(double velocityLeft, double velocityRight, double goSpeed){
		vLeft = velocityLeft;
		vRight = velocityRight;
		v_go = goSpeed;
	}

	
	double getvLeft(){
		return Robot::vLeft;
	}
	
	double getvRight(){
		return Robot::vRight;
	}
	
	double getvGo(){
		return Robot::v_go;
	}
	
	void changevLeft(double dV){
		double v_go = getvGo();
		Robot::vLeft = v_go + dV;
		std::cout<<"left "<<Robot::vLeft<<std::endl;
	}
	
	void changevRight(double dV){
		double v_go = getvGo();
		Robot::vRight = v_go - dV;
		std::cout<<"right "<<Robot::vRight<<std::endl;
	}
	
	void runRobot();
	double offset(ImagePPM cameraView);
};

double Robot::offset(ImagePPM cameraView){
	// Initialise variables to help calculate the error correction 
	double error = 0.0;
	double totWhitePix = 0.0;

	// Create a for loop that runs 150 times for each pixel in a row
	for(int currRow = 0; currRow < cameraView.width; currRow++){
		/* DEVELOPER COMMENTS
		 * 
		 * cameraView is the picture in ppm format we just took
		 * i is current pixel in the row
		 * 3 so we can get all 3 RGB values */
		int middlePix = get_pixel(cameraView, cameraView.height / 2, currRow, 3);

		// If the pixel value is greater than 250 consider the pixel as white
		if(middlePix > 250){
			// Add the currRow to the error so we can find the average (mid point) of the current white pixels
			error += currRow;
			// Add one to the total amount of white pixels
			totWhitePix += 1;
		}
	}

	// Find the middle index positioing of the white pixels
	double currMiddleWhitePix = error / totWhitePix;
	// Take away 75 as that is half of the image width to find the error amount
	return currMiddleWhitePix - 75;
}

void Robot::runRobot() {
	// Create an if statement to check if robot is initialised
	if (initClientRobot() !=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}
	// Initialise multiple variables to determine current speed
	
    //double vLeft = 23.0;
    //double vRight = 23.0;
	//double v_go = 23.0;
	//double kp = 0.1; // Create a constant to help caculate change in speed
   
   
   
   
	double vLeft = getvLeft();
	double vRight = getvRight();
    double dV = vLeft - vRight; // Difference between the two speeds


    while(1){
		// Take a .ppm photo of the current 150x100 pixel box ahead of the robot
		takePicture();
		
		// Save the errorCorrection given by the function offset
		double errorCorrection = offset(cameraView);
		std::cout<<"errorCorrection "<<errorCorrection<<std::endl;
		/* DEVELOPER COMMENTS 
		 * An important thing to note is that the initial dV is calculated via vLeft - vRight
		 * So, if the vRight is greater than vLeft, the difference would be negative. Therefore,
		 * when we go to calculate the new vRight, we - dV as - a negative number gives a positive number.
		 * And if vLeft is greater than vRight, dV is positive so we can just add that to vLeft */

		// Caculate the difference between the left and right speeds
		dV = kp * errorCorrection;
		// Change the speeds based off the difference
		changevRight(dV);
		changevLeft(dV);
		// Set the motors to the same speed
		double vLeft = getvLeft();
		double vRight = getvRight();
		setMotors(vLeft,vRight);
		std::cout<<" vLeft="<<vLeft<<" vRight="<<vRight<<std::endl;
		usleep(10000);
	} //while
}
