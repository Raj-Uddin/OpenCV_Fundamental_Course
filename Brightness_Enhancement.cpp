

/*

How to compile:

"F:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64

cl /EHsc /std:c++17 /I C:\Python312\include /I F:\OpenCV_Course\myenv\Lib\site-packages\numpy\_core\include /I F:\AI_Componets\OpenCV\build\install\include Brightness_Enhancement.cpp /link /LIBPATH:F:\AI_Componets\OpenCV\build\install\x64\vc17\lib /LIBPATH:C:\Python312\libs python312.lib opencv_core500.lib opencv_imgcodecs500.lib opencv_highgui500.lib opencv_imgproc500.lib

*/

#include <iostream>  // Include standard input-output library
#include "dataPath.hpp"  // Include a custom header file that likely defines DATA_PATH
#include <opencv2/opencv.hpp>  // Include OpenCV core functions and operations
#include <opencv2/core.hpp>  // Include OpenCV core module for matrix operations
#include <opencv2/highgui.hpp>  // Include OpenCV module for GUI display functions

using namespace std;  // Use the standard namespace to simplify code
using namespace cv;  // Use the OpenCV namespace

// Function to convert OpenCV Mat type to a string representation
string type2str(int type)
{
    string r;

    // Extract the depth (data type) and number of channels from the type
    uchar depth = type & CV_MAT_DEPTH_MASK;  // Get the depth using a bit mask
    uchar chans = 1 + (type >> CV_CN_SHIFT); // Get the number of channels

    // Determine the image bit depth type
    switch (depth) 
    {
        case CV_8U:  r = "8U"; break;   // 8-bit unsigned integer
        case CV_8S:  r = "8S"; break;   // 8-bit signed integer
        case CV_16U: r = "16U"; break;  // 16-bit unsigned integer
        case CV_16S: r = "16S"; break;  // 16-bit signed integer
        case CV_32S: r = "32S"; break;  // 32-bit signed integer
        case CV_32F: r = "32F"; break;  // 32-bit floating point
        case CV_64F: r = "64F"; break;  // 64-bit floating point
        default:     r = "User"; break; // Unknown type
    }

    // Append the number of channels to the type string
    r += "C";
    r += (chans + '0');  // Convert the integer channel count to a char and append

    return r;  // Return the formatted type string
}

int main(void)
{
    // Load images from file
     Mat image = imread(DATA_PATH + "images/boy.jpg");  // Read the original image 24bit
    // Mat image = imread("F:/OpenCV_Course/CVIP/week_1/week1-cpp/data/images/girl_32_bit.jpg");  // Read the original image 32 bit
    Mat contrastHigh = imread("../results/highContrast.png");  // Read a high-contrast image

    // Define a brightness offset value (50 intensity levels)
    int brightnessOffset = 100;

    // Create a copy of the original image to apply brightness changes
    Mat brightHigh = image;
    Mat brightHighChannels[3];  // Array to store individual color channels

	brightHigh.convertTo(brightHigh, CV_32F, 1/255.0, 0);	// We need to now convert the image to a 32 bit float so that it can hold more pixel information, such as obtaining brightness details.

    // Split the original image into its three color channels (BGR)
    split(brightHigh, brightHighChannels);

    // Increase brightness by adding the brightnessOffset to each channel
    for (int i = 0; i < 3; i++) 
    {		// Input				 add					output
        add(brightHighChannels[i], brightnessOffset/255.0, brightHighChannels[i]);  // Increase brightness
    }

    // Merge the modified channels back into a single image
    merge(brightHighChannels, 3, brightHigh);

	// Performing normalization for float data type images:

	Mat normalization = brightHigh.clone();	// <------------------------------------ normalization to the brightness enhancement.

    // Variables to store min and max pixel intensity values
    double min, max;
    minMaxLoc(normalization, &min, &max);		    // Find the maximum pixel intensity in the original image
    cout << "Normalization Image Highest Pixel Intensity : " << max << endl;

	normalization = normalization/max ;

    // Display the modified brightened image in a window
    imshow("Original", image);
    imshow("High Brightness", brightHigh);
    imshow("normalization High Brightness", normalization);

    // Save the brightness-adjusted image to a file
    imwrite("../results/highBrightness.png", brightHigh);


	// Output some metrics:

    // Output the data type of each image using type2str function
    cout << "Original Image Datatype : " << type2str(image.type()) << endl;
    cout << "Contrast Image Datatype : " << type2str(contrastHigh.type()) << endl;
    cout << "Brightness Image Datatype : " << type2str(brightHigh.type()) << endl;
    cout << "normalization Image Datatype : " << type2str(normalization.type()) << endl;

    // Find and print the maximum pixel intensity in the original image
    minMaxLoc(image, &min, &max);
    cout << "Original Image Highest Pixel Intensity : " << max << endl;

    // Find and print the maximum pixel intensity in the contrast-adjusted image
    minMaxLoc(contrastHigh, &min, &max);
    cout << "Contrast Image Highest Pixel Intensity : " << max << endl;

    // Find and print the maximum pixel intensity in the brightness-adjusted image
    minMaxLoc(brightHigh, &min, &max);
    cout << "Brightness Image Highest Pixel Intensity : " << max << endl;


    waitKey(0);  // Wait for a key press before closing the window

    return 0;  // Exit the program successfully
}


