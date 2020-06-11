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


/**
 * This function returns the brightness of the pixel corresponding to
 * pixIndex.
 * 
 * @param cameraView - an ImagePPM object representing the robot's POV
 * @param pixIndex - the position of the pixel along the perimeter of
 *                   cameraView. e.g. pixIndex = 0 for the pixel in the
 *                   bottom-left hand corner (from the robot's POV).
 * @returns the brightness of the specified pixel
 */
int brightness(ImagePPM cameraView, int pixIndex) {
	int perimeterPixCount = 2 * (cameraView.width + cameraView.height) - 4;

	// Indices of three important pixels
	int topLeft = cameraView.height - 1;
	int topRight = cameraView.height + cameraView.width - 2;
	int bottomRight = 2 * cameraView.height + cameraView.width - 3;

	if (pixIndex < topLeft) { // If pixel on left edge
		return get_pixel(cameraView, -pixIndex + topLeft, 0, 3);
	} else if (topLeft <= pixIndex && pixIndex < topRight) { // If pixel on top edge
		return get_pixel(cameraView, 0, pixIndex - topLeft, 3);
	} else if (topRight <= pixIndex && pixIndex < bottomRight) { // If pixel on right edge
		return get_pixel(cameraView, pixIndex - topRight, cameraView.width - 1, 3);
	} else { // If pixel on bottom edge
		return get_pixel(cameraView, cameraView.height - 1, -pixIndex + perimeterPixCount, 3);
	}
}

/**
 * This function iterates over the pixels of the perimeter of
 * cameraView, starting from the bottom-left hand corner (from the
 * robot's POV). It calculates the distance between the average position
 * of white pixels along this perimeter and the perimeter pixel directly
 * in front of the robot. However, it only considers the first group of
 * white pixels it finds, as this group will be the one that it will try
 * to follow.
 * 
 * @param cameraView - an ImagePPM object representing the robot's FOV
 * @returns the aforementioned distance/offset
 */
double Robot::offset(ImagePPM cameraView){
	// Initialise variables to help calculate the error correction 
	double error = 0.0;
	double totWhitePix = 0.0;
	
	// Number of pixels on the image's perimeter
	int perimeterPixCount = 2 * (cameraView.width + cameraView.height) - 4;

	for(int pixIndex = 0; pixIndex < perimeterPixCount; pixIndex++){
		int currPixBrightness = brightness(cameraView, pixIndex);

		// If the pixel value is greater than 250 consider the pixel as white
		if(currPixBrightness > 250){
			error += pixIndex;
		 	totWhitePix += 1;
		// Break loop if a non-white pixel is found when at least one white pixel has already been found
		} else if (totWhitePix != 0) { break; }
	}

	// Index where offset = 0
	int relativeIndex = cameraView.height + (cameraView.width - 1) / 2 - 1;

	// Calculate average index of all white pixels found
	// If no white pixles were found, set this to a large number so that
	// the robot turns very quickly.
	double avWhiteIndex = (totWhitePix != 0) ? error / totWhitePix
											: 1000;
	return avWhiteIndex - relativeIndex;
}

void Robot::runRobot() {
	// Create an if statement to check if robot is initialised
	if (initClientRobot() !=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}

	double vLeft = getvLeft();
	double vRight = getvRight();
    double dV = vLeft - vRight; // Difference between the two speeds


    while(1){
		// Take a .ppm photo of the current 150x100 pixel box ahead of the robot
		takePicture();
		
		// Save the errorCorrection given by the function offset
		double errorCorrection = offset(cameraView);
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



