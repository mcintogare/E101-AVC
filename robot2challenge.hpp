#include <stdlib.h>
#include <math.h>
#include <set>

class RobotChallenge {

public:
	double vLeft, vRight, v_go;
	double kp = 0.1;
	
	RobotChallenge(double velocityLeft, double velocityRight, double goSpeed){
		vLeft = velocityLeft;
		vRight = velocityRight;
		v_go = goSpeed;
	}
	
	double getvLeft(){ return RobotChallenge::vLeft; }
	double getvRight(){ return RobotChallenge::vRight; }
	double getvGo(){ return RobotChallenge::v_go; }
	
	void changevLeft(double dV){
		double v_go = getvGo();
		RobotChallenge::vLeft = v_go + dV;
		std::cout<<"left "<<RobotChallenge::vLeft<<std::endl;
	}
	
	void changevRight(double dV){
		double v_go = getvGo();
		RobotChallenge::vRight = v_go - dV;
		std::cout<<"right "<<RobotChallenge::vRight<<std::endl;
	}
	
	int brightness(ImagePPM cameraView, int pixIndex, int depth);
	double offset(ImagePPM cameraView, int depth);
	
	void runRobotChallenge();
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
int RobotChallenge::brightness(ImagePPM cameraView, int pixIndex, int depth) {
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
	}
}

/**
 * Calculates the angle centred at the nearest pixel to the robot,
 * measured between the current pixel (row, col) of cameraView and the
 * bottom-left-hand corner of the FOV
 * @param cameraView - an ImagePPM object representing the robot's
 *                     full FOV
 * @param i - the row of the red pixel
 * @param j - the column of the red pixel
 * @returns the angle
 */
double angleFromLeft(ImagePPM cameraView, int i, int j) {
	int i0 = cameraView.height;
	int j0 = (cameraView.width - 1) / 2;
	
	double di = i0 - i;
	double dj = j - j0;
	
	if (dj == 0) return (M_PI / 2);
	
	double alpha = atan(di / dj);
	return (alpha >= 0) ? M_PI - alpha : -alpha;
}

/**
 * Finds the total number of white pixels in cameraView
 * @param cameraView - an ImagePPM object representing the robot's
 *                     full FOV
 * @returns the total
 */
double getTotFOVWhitePix(ImagePPM cameraView) {
	double totFOVWhitePix = 0.0;
	for (int i = 0; i < cameraView.height; i++) {
		for (int j = 0; j < cameraView.width; j++) {
			if (get_pixel(cameraView, i, j, 3) > 250) {
				totFOVWhitePix += 1;
			}
		}
	}
	return totFOVWhitePix;
}

double dist(int i, int j) {
	int i0 = cameraView.height;
	int j0 = (cameraView.width - 1) / 2;
	double di = i0 - i;
	double dj = j0 - j;
	return sqrt(di * di + dj * dj);
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
double RobotChallenge::offset(ImagePPM cameraView, int depth){
	// Initialise variables to help calculate the error correction 
	double error = 0.0;
	double totMatchingPix = 0.0;
	double totFOVWhitePix = getTotFOVWhitePix(cameraView);
	double angleThreshold = 0.3;
	double distThreshold = cameraView.height / 2;
	bool redWithinDistThreshold = false;
	
	int kp0 = 6.8;
	
	std::set<double> angleSet;
	std::set<double>::reverse_iterator it;
	angleSet.insert(0);
	angleSet.insert(M_PI);
	
	// Total number of pixels that will be analysed
	int totalPixCount = 2 * depth + cameraView.width - 3;
	
	if (totFOVWhitePix == 0) { // If no white line exists within FOV
		for (int i = 0; i < cameraView.height; i++) {
			for (int j = 0; j < cameraView.width; j++) {
				if (get_pixel(cameraView, i, j, 0) == 255) {
					angleSet.insert(angleFromLeft(cameraView, i, j));
					if (dist(i, j) < distThreshold) redWithinDistThreshold = true;
				}
			}
		}
		
		this->kp = (redWithinDistThreshold) ? kp0 * 2 : kp0;
		
		double prevAngle = *angleSet.rbegin();
		for (it = angleSet.rbegin(); it != angleSet.rend(); it++) {
		//  double angle : angleSet) {
			double angle = *it;
			if (prevAngle - angle > angleThreshold) {
				double targetAngle = (angle + prevAngle) / 2;
				std::cout<<"Previous angle: "<<prevAngle<<std::endl;
				std::cout<<"Current angle: "<<angle<<std::endl;
				std::cout<<"Angle difference: "<<angle - prevAngle<<std::endl;
				std::cout<<"Target angle: "<<targetAngle<<std::endl;
				return targetAngle - M_PI / 2;
			}
			prevAngle = angle;
		}
		std::cout<<"Can't find gap!"<<std::endl;
		
		// Return largest possible offset if no significant gaps are found, to make the robot turn rapidly
		return -3 * M_PI / 4;
	} else { // If white line does exist within FOV
		// ALGORITHM FOR CORE AND COMPLETION
		this->kp = 0.1;
		for(int pixIndex = 0; pixIndex < totalPixCount; pixIndex++){
			int currPixBrightness = brightness(cameraView, pixIndex, depth);
			// If the pixel value is greater than 250 consider the pixel as white
			if(currPixBrightness > 250){
				error += pixIndex;
				totMatchingPix += 1;
			// Break loop if a red pixel is found when at least one white pixel has already been found
			} else if (totMatchingPix != 0) { break; }
		}
		
		// Index where offset = 0
		int relativeIndex = depth + (cameraView.width - 1) / 2 - 1;
		
		double avWhiteIndex = (totMatchingPix == 0) ? relativeIndex
		                                            : error / totMatchingPix;
		return avWhiteIndex - relativeIndex;
	}
}

void RobotChallenge::runRobotChallenge() {
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
		
		int depth = cameraView.height;
		
		// Save the errorCorrection given by the function offset
		double errorCorrection = offset(cameraView, depth);
		std::cout<<"Error correction: "<<errorCorrection<<std::endl;
		std::cout<<"kp: "<<kp<<std::endl;
		// Caculate the difference between the left and right speeds
		//dV = kp * errorCorrection * errorCorrection * errorCorrection;
		if (errorCorrection == 0) {
			dV = 0;
		} else {
			dV = kp * errorCorrection * errorCorrection * errorCorrection / abs(errorCorrection);
		}
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
