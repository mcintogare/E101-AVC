class Robot {

public:
	double vLeft, vRight, v_go;
	double kp = 0.1;
	
	Robot(double velocityLeft, double velocityRight, double goSpeed){
		vLeft = velocityLeft;
		vRight = velocityRight;
		v_go = goSpeed;
	}
	
	double getvLeft(){ return Robot::vLeft; }
	double getvRight(){ return Robot::vRight; }
	double getvGo(){ return Robot::v_go; }
	
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
	
	int brightness(ImagePPM cameraView, int pixIndex, int depth);
	double offset(ImagePPM cameraView, int depth);
	
	void runRobot();
};


/**
 * This function returns the brightness of the pixel corresponding to
 * pixIndex.
 * 
 * @param cameraView - an ImagePPM object representing the robot's POV
 * @param pixIndex - the position of the pixel along the perimeter of
 *                   cameraView. e.g. pixIndex = 0 for the pixel in the
 *                   bottom-left hand corner (from the robot's FOV).
 * @param depth - how far forward the robot can see within the full FOV.
 *                the robot's actual FOV is determined by this
 * @returns the brightness of the specified pixel
 */
int Robot::brightness(ImagePPM cameraView, int pixIndex, int depth) {
	// Total number of pixels that will be analysed
	int totalPixCount = 2 * depth + cameraView.width - 3;

	// Indices of three important pixels
	int topLeft = depth - 1;
	int topRight = depth + cameraView.width - 2;
	int bottomRight = 2 * depth + cameraView.width - 3;

	if (pixIndex < topLeft) { // If pixel on left edge
		return get_pixel(cameraView, -pixIndex + topLeft, 0, 3);
	} else if (topLeft <= pixIndex && pixIndex < topRight) { // If pixel on top edge
		return get_pixel(cameraView, cameraView.height - depth, pixIndex - topLeft, 3);
	} else if (topRight <= pixIndex && pixIndex < bottomRight) { // If pixel on right edge
		return get_pixel(cameraView, pixIndex - topRight + cameraView.height - depth, cameraView.width - 1, 3);
	} // else { // If pixel on bottom edge
		// return get_pixel(cameraView, cameraView.height - 1, -pixIndex + totalPixCount, 3);
	// }
}

/**
 * This function iterates over the pixels of the perimeter of the
 * robot's FOV, starting from the bottom-left hand corner (from the
 * robot's FOV). It calculates the distance between the average position
 * of white pixels along this perimeter and the perimeter pixel directly
 * in front of the robot. However, it only considers the first group of
 * white pixels it finds, as this group will be the one that it will try
 * to follow.
 * 
 * @param cameraView - an ImagePPM object representing the robot's
 *                     full FOV
 * @param depth - how far forward the robot can see within the full FOV.
 *                the robot's actual FOV is determined by this
 * @returns the aforementioned distance/offset
 */
double Robot::offset(ImagePPM cameraView, int depth){
	// Initialise variables to help calculate the error correction 
	double error = 0.0;
	double totWhitePix = 0.0;
	double totFOVWhitePix = 0.0;
	
	// Total number of pixels that will be analysed
	int totalPixCount = 2 * depth + cameraView.width - 3;
	
	for (int i = 0; i < cameraView.height; i++) {
		for (int j = 0; j < cameraView.width; j++) {
			if (get_pixel(cameraView, i, j, 3) > 250) {
				totFOVWhitePix += 1;
			}
		}
	}

	for(int pixIndex = 0; pixIndex < totalPixCount; pixIndex++){
		int currPixBrightness = brightness(cameraView, pixIndex, depth);

		// If the pixel value is greater than 250 consider the pixel as white
		if(currPixBrightness > 250){
			error += pixIndex;
		 	totWhitePix += 1;
		// Break loop if a non-white pixel is found when at least one white pixel has already been found
		} else if (totWhitePix != 0) { break; }
	}

	// Index where offset = 0
	int relativeIndex = depth + (cameraView.width - 1) / 2 - 1;

	// Calculate average index of all white pixels found. If no white
	// pixels exist within the robot's FOV, set it to a large number.
	// If there aren't any white pixels within the scanned lines, set it
	// so the robot goes straight.
	
	double avWhiteIndex;
	if (totFOVWhitePix == 0) {
		avWhiteIndex = 500;
	} else if (totWhitePix == 0) {
		avWhiteIndex = relativeIndex;
	} else {
		avWhiteIndex = error / totWhitePix;
	}
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

	int i = 0; // TESTING

    while(1){
		// Take a .ppm photo of the current 150x100 pixel box ahead of the robot
		takePicture();
		
		int depth = (cameraView.height + 1) / 2;
		// int depth = cameraView.height;
		
		// Save the errorCorrection given by the function offset
		double errorCorrection = offset(cameraView, depth);
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
		i++; // TESTING
		//if (i == 1) { break; } // TESTING
	} //while
}



