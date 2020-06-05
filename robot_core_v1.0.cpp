
#include "robot.hpp"

double offset(int* rowOfPixels, int error, int totWhitePix){
	// Find the middle index positioing of the white pixels
	double currMiddleWhitePix = error / totWhitePix;
	// Take away 75 as that is half of the image width to find the error amount
	double errorCorrection = currMiddleWhitePix - 75;
	return errorCorrection;
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
    
    
    while(1){
		// Take a .ppm photo of the current 150x100 pixel box ahead of the robot
		takePicture();
		// Initialise variables to help calculate the error correction 
		double error = 0.0;
		double totWhitePix = 0.0;
		// Create an array to store all the pixels in the current row
		int* rowOfPixels = new int[150]; 

		// Create a for loop that runs 150 times for each pixel in a row
		for(int currRow = 0; currRow < 150; currRow++){
			/* DEVELOPER COMMENTS
			 * 
			 * cameraView is the picture in ppm format we just took
			 * 50 is half of the height of the camera view image
			 * i is current pixel in the row
			 * 3 so we can get all 3 RGB values */
			int middlePix = get_pixel(cameraView, 50, currRow, 3);
			
			// Initialise new variable to determine if it is white
			int isWhite;
			
			// If the pixel value is greater than 250 consider the pixel as white
			if(middlePix > 250){
				isWhite = 1;
				// Add the currRow to the error so we can find the average (mid point) of the current white pixels
				error += currRow;
				// Add one to the total amount of white pixels
				totWhitePix += 1;
				// Add to the array
				rowOfPixels[currRow] = isWhite;
			// The pixel is NOT white
			} else{
				isWhite = 0;
				// Add to the array
				rowOfPixels[currRow] = isWhite;
			}
		}
		// Save the errorCorrection given by the function offset
		double errorCorrection = offset(rowOfPixels, error, totWhitePix);
		
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
	} //while
} // main
