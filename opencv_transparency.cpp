

/*

How to compile:

"F:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat" x64

cl /EHsc /std:c++17 /I C:\Python312\include /I F:\OpenCV_Course\myenv\Lib\site-packages\numpy\_core\include /I F:\AI_Componets\OpenCV\build\install\include Using_Arithmetic_Operations.cpp /link /LIBPATH:F:\AI_Componets\OpenCV\build\install\x64\vc17\lib /LIBPATH:C:\Python312\libs python312.lib opencv_core500.lib opencv_imgcodecs500.lib opencv_highgui500.lib opencv_imgproc500.lib

*/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "dataPath.hpp"

using namespace std;
using namespace cv;

int main(void)
{
    // Read the grayscale mask image for the sunglasses.
    // The mask is used to define the transparent areas of the sunglasses.
    Mat glassMask1 = imread("../results/sunglassAlpha.png", 0);

    // Read the color image of the sunglasses.
    Mat glassBGR = imread("../results/sunglassRGB.png");

    // Declare a Mat to store the three-channel mask.
    Mat glassMask;

    // Create an array of three channels, each being the same grayscale mask.
    // This is done to match the number of channels in the color image.
    Mat glassMaskChannels[] = {glassMask1, glassMask1, glassMask1};

    // Merge the three single-channel masks into a three-channel mask.
	// "glassMask" - This is the resulting multi-channel Mat object that will STORE the merged channels. 
	// After the merge, glassMask will be a three-channel image where each channel is identical and contains the grayscale mask data.
    merge(glassMaskChannels, 3, glassMask);

	imshow("glassMask without normalization", glassMask);

    // Normalize the mask values to the range [0, 1] for arithmetic operations.
    glassMask = glassMask / 255;

	imshow("glassMask with normalization", glassMask);

    // Load the face image where the sunglasses will be overlaid.
    string faceImagePath = DATA_PATH + "/images/musk.jpg";
    Mat faceImage = imread(faceImagePath);

    // Create a copy of the face image to apply the sunglasses.
    Mat faceWithGlassesArithmetic = faceImage.clone();

	imshow("faceWithGlassesArithmetic", faceWithGlassesArithmetic);

    // Define the region of interest (ROI) for the eyes in the face image.
    // This is where the sunglasses will be placed.
	// Copy a rectangular area from the image and paste it into "eyeROI".
    Mat eyeROI = faceWithGlassesArithmetic(Range(150, 250), Range(140, 440));

	imshow("eyeROI", eyeROI);

    // Split the eye ROI into its three color channels.
	// The "split" function takes a multi-channel array (in this case, eyeROI) and separates it into individual single-channel arrays.
	// "eyeROIChannels" - is an array that will STORE these individual channels after the split.
    Mat eyeROIChannels[3];
    split(eyeROI, eyeROIChannels);

    // Declare arrays to store the masked eye channels and the final masked eye image.
    Mat maskedEyeChannels[3];
    Mat maskedEye;

    // Loop over each channel to apply the mask.
    for (int i = 0; i < 3; i++)
    {
        // Multiply the eye ROI channel by the inverse of the mask to retain only the non-glasses part.
        multiply(eyeROIChannels[i], (1 - glassMaskChannels[i]), maskedEyeChannels[i]);
    }

	imshow("maskedEyeChannels_0", maskedEyeChannels[0]);
	imshow("maskedEyeChannels_1", maskedEyeChannels[1]);
	imshow("maskedEyeChannels_2", maskedEyeChannels[2]);

    // Merge the masked eye channels back into a single image.
    merge(maskedEyeChannels, 3, maskedEye);

	imshow("maskedEye", maskedEye);

    // Declare a Mat to store the masked sunglasses.
    Mat maskedGlass;

    // Multiply the sunglasses image by the mask to retain only the glasses part.
    multiply(glassBGR, glassMask, maskedGlass);

	Mat eyes;

	subtract(eyeROI, maskedEye, eyes);

	imshow("eyes", eyes);

	// Adjust the opacity of the masked sunglasses to 50%
	// maskedGlass = maskedGlass * 1;
	maskedGlass = maskedGlass * .5;

	Mat eyeROI_fo;							// <------ final result of the eyes opacity.
	Mat eyes_o = eyes.clone();				// <------ controlling the opacity of the eyes

	// eyes_o = eyes_o * 0.5;					// <------ Test if the masked worked.
	eyes_o = eyes_o * 0.5;

    add(maskedEye, eyes_o, eyeROI_fo);

	imshow("eyeROI_o", eyeROI_fo);

    // Declare a Mat to store the final eye ROI with sunglasses.
    Mat eyeRoiFinal;

    // Add the masked eye region and the masked sunglasses to combine them.
    //add(maskedEye, maskedGlass, eyeRoiFinal);

    add(eyeROI_fo, maskedGlass, eyeRoiFinal);

    // Save the intermediate results for debugging or visualization.
    imwrite("../results/maskedEyeRegion.png", maskedEye);
    imwrite("../results/maskedSunglassRegion.png", maskedGlass);
    imwrite("augmentedEyeAndSunglass.png", eyeRoiFinal);

    // Display the intermediate results.
    imshow("maskedGlass", maskedGlass);
    imshow("eyeRoiFinal", eyeRoiFinal);

    // Replace the original eye ROI with the final eye ROI containing sunglasses.
    eyeRoiFinal.copyTo(eyeROI);

    // Save the final image with sunglasses.
    imwrite("../results/withSunglasses.png", faceWithGlassesArithmetic);

    // Display the final image.
    imshow("With Sunglasses", faceWithGlassesArithmetic);

    // Wait for a key press indefinitely.
    waitKey(0);

    return 0;
}
