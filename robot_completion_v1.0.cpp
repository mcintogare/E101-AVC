
#include "robot.hpp"

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
double offset(ImagePPM cameraView){
	// Initialise variables to help calculate the error correction 
	double whiteIndexSum = 0.0;
	double whiteCount = 0.0;
	
	// Number of pixels on the image's perimeter
	int perimeterPixCount = 2 * (cameraView.width + cameraView.height) - 4;
	
	for(int pixIndex = 0; pixIndex < perimeterPixCount; pixIndex++){
		int currPixBrightness = brightness(cameraView, pixIndex);
		
		// If the pixel value is greater than 250 consider the pixel as white
		if(currPixBrightness > 250){
			whiteIndexSum += pixIndex;
		 	whiteCount += 1;
		// Break loop if a non-white pixel is found when at least one white pixel has already been found
		} else if (whiteCount != 0) { break; }
	}
	
	// Index where offset = 0
	int relativeIndex = cameraView.height + (cameraView.width - 1) / 2 - 1;
	
	// Calculate average index of all white pixels found
	// If no white pixles were found, set this to a large number so that
	// the robot turns very quickly.
	double avWhiteIndex = (whiteCount != 0) ? whiteIndexSum / whiteCount
											: 1000;
	return avWhiteIndex - relativeIndex;
}

int main(){
	// Create an if statement to check if robot is initialised
	if (initClientRobot() !=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}
	// Initialise multiple variables to determine current speed
    double vLeft = 12.0;
    double vRight = 12.0;
	double v_go = 12.0;
	double kp = 0.1; // Create a constant to help caculate change in speed
    double dV = vLeft - vRight; // Difference between the two speeds
    
    int i = 0; // TESTING
    while(1){
		// Take a .ppm photo of the current 150x100 pixel box ahead of the robot
		takePicture();
		
		// Save the errorCorrection given by the function offset
		double errorCorrection = offset(cameraView);
		
		/* DEVELOPER COMMENTS 
		 * An important thing to note is that the initial dV is calculated via vLeft - vRight
		 * So, if the vRight is greater than vLeft, the difference would be negative. Therefore,
		 * when we go to calculate the new vRight, we - dV as - a negative number gives a positive number.
		 * And if vLeft is greater than vRight, dV is positive so we can just add that to vLeft */
		 
		// Caculate the difference between the left and right speeds
		dV = kp * errorCorrection;
		// Change the speeds based off the difference
		vRight = v_go - dV;
		vLeft = v_go + dV;
		// Set the motors to the same speed
		setMotors(vLeft,vRight);
		std::cout<<" vLeft="<<vLeft<<" vRight="<<vRight<<std::endl;
		usleep(10000);
		/* TESTING */
		std::string filename = "test" + std::to_string(i) + ".ppm"; 
		std::cout<<"Error correction: "<<errorCorrection<<"; Filename: "<<filename<<std::endl;
		SavePPMFile(filename, cameraView);
		i++;
	} //while
} // main
