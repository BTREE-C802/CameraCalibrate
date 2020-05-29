/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 9 of the cookbook:  
   Computer Vision Programming using the OpenCV Library. 
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify, 
   and distribute this source code, or portions thereof, for any purpose, without fee, 
   subject to the restriction that the copyright notice may not be removed 
   or altered from any source or altered source distribution. 
   The software is released on an as-is basis and without any warranties of any kind. 
   In particular, the software is not guaranteed to be fault-tolerant or free from failure. 
   The author disclaims all warranties with regard to this software, any use, 
   and any consequent failure, is purely the responsibility of the user.
 
   Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <CameraCalibrator.h>
#include <string>

int main()
{
	cv::namedWindow("Image");
	cv::Mat image,imgResized;
	std::vector<std::string> filelist;
	// generate list of chessboard image filename
	for (int i=1; i<=16; i++) 
	{
		std::stringstream str;
		str << "../chessboards4/chessboard" << std::setw(2) << std::setfill('0') << i << ".png";
		std::cout << str.str() << std::endl;
		filelist.push_back(str.str());
		image= cv::imread(str.str(),0);
		int Image_size_Max = fmax(image.rows,image.cols);
		if(Image_size_Max > 640)
		{
			float scale1 = (float)640.0f/(float)image.cols; 
			float scale2 = (float)480.0f/(float)image.rows; 
			cv::resize(image,imgResized,cv::Size(),scale1,scale2);
			image = imgResized;
			std::stringstream  ImageName;
			ImageName << "/chessboards" << std::setw(2) << std::setfill('0') << i << ".jpg";
			std::string SaveFilesName = "../SaveKeyFrameImage/"+ImageName.str();
			cv::imwrite(SaveFilesName,image);
		}
		cv::imshow("Image",image);
		cv::waitKey(100);
	}
	// Create calibrator object
	CameraCalibrator cameraCalibrator;
	// add the corners from the chessboard
	cv::Size boardSize(4,6);
	cameraCalibrator.addChessboardPoints(
		filelist,	// filenames of chessboard image
		boardSize);	// size of chessboard
	// calibrate the camera
	//cameraCalibrator.setCalibrationFlag(true,true);
	cv::Size ImageSize = image.size();
	cameraCalibrator.calibrate(ImageSize);
	
	// Image Undistortion
	image = cv::imread(filelist[6]);
	cv::Mat uImage= cameraCalibrator.remap(image);
	
	// display camera matrix
	cv::Mat cameraMatrix= cameraCalibrator.getCameraMatrix();
	std::cout << " Camera intrinsic: " << cameraMatrix.rows << "x" << cameraMatrix.cols << std::endl;
	std::cout << cameraMatrix.at<double>(0,0) << " " << cameraMatrix.at<double>(0,1) << " " << cameraMatrix.at<double>(0,2) << std::endl;
	std::cout << cameraMatrix.at<double>(1,0) << " " << cameraMatrix.at<double>(1,1) << " " << cameraMatrix.at<double>(1,2) << std::endl;
	std::cout << cameraMatrix.at<double>(2,0) << " " << cameraMatrix.at<double>(2,1) << " " << cameraMatrix.at<double>(2,2) << std::endl;

	// display  DistCoeffs matrix
	cv::Mat DistCoeffs_ = cameraCalibrator.getDistCoeffs();
	std::cout << " DistCoeffs_: " << DistCoeffs_.rows << "x" << DistCoeffs_.cols << std::endl;
	std::cout << DistCoeffs_ << std::endl;
	//std::cout << cameraMatrix.at<double>(0,0) << " " << cameraMatrix.at<double>(0,1) << " " << cameraMatrix.at<double>(0,2) << std::endl;
	//std::cout << cameraMatrix.at<double>(1,0) << " " << cameraMatrix.at<double>(1,1) << " " << cameraMatrix.at<double>(1,2) << std::endl;
	//std::cout << cameraMatrix.at<double>(2,0) << " " << cameraMatrix.at<double>(2,1) << " " << cameraMatrix.at<double>(2,2) << std::endl;
	
	imshow("Original Image", image);
	imshow("Undistorted Image", uImage);
	std::cout << "完成矫正与标定" << std::endl;
	cv::waitKey(0);
	return 0;
}
