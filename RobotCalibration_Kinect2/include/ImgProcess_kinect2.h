#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;


class ImgProcess_kinect2
{
public:
	ImgProcess_kinect2();
	~ImgProcess_kinect2();

public:
	HRESULT init_kinect();
	
	bool getHDImage(string str);

private:
	Camera_Intrinsics getCameIns(ICoordinateMapper* pCoordinateMapper);
	

private:
	// Create Sensor Instance
	IKinectSensor * pSensor;
	// Retrieved Coordinate Mapper 
	ICoordinateMapper* pCoordinateMapper;
	// Retrieved Color Frame Source
	IColorFrameSource* pColorFrameSource;
	// Retrieved Depth Frame Source
	IDepthFrameSource* pDepthFrameSource;
	// Retrieved Infrared Frame Source
	IInfraredFrameSource* pInfraredFrameSource;
	// Open Color Frame Reader
	IColorFrameReader* pColorReader;
	// Open Depth Frame Reader
	IDepthFrameReader* pDepthReader;
	// Open Infrared Frame Reader
	IInfraredFrameReader* pInfraredReader;
	// Retrieved Color Frame Size
	IFrameDescription* pColorDescription;
	// Retrieved Depth Frame Size
	IFrameDescription* pDepthDescription;
	// Retrieved Infrared Frame Size
	IFrameDescription* pInfraredDescription;
	//Camera Intrinsics
	Camera_Intrinsics camera_ins;

	//��ɫ֡�����֡
	const int ColorWidth = 1920;
	const int ColorHeight = 1080;
	const int DepthWidth = 512;
	const int DepthHeight = 424;
};

