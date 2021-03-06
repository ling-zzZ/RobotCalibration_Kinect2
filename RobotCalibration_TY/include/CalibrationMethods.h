#pragma once
#include <opencv2/core/eigen.hpp>
#include "CalCamArm_64.h"

// #pragma comment(lib,"CalCamArm_64.lib")

/*
*	@brief	手眼标定的方法，包括六点法和标定板法（标定板固定在机械臂末端）
*/

using namespace cv;
using namespace std;

class CalibrationMethods
{
public:
	CalibrationMethods();
	~CalibrationMethods();

public:


	/*
	*	@Method		Method_BoardOnRobot
	*	@brief  	计算相机到机械臂的变换矩阵(棋盘格固定在相机)
	*	@param		std::string robot			机械臂末端不同位置对应变换矩阵 baseHend
	*	@param		std::string cam_cal			相机到棋盘格变换矩阵 calHcam
	*	@param		std::string result_file		结果保存文件
	*	@param		cv::Mat & m_result			返回相机到机械臂坐标系变换矩阵 robHcam
	*	@return		void
	*/
	void Method_BoardOnRobot(std::string robot, std::string cam_cal, std::string result_file, cv::Mat& m_result);


	/*
	*	@brief		计算机械臂末端和标定板的变换关系 endHcal
	*	@param		robot		机械臂末端不同位置对应变换矩阵 baseHend
	*   @param		cam_cal		相机到棋盘格变换矩阵 calHcam
	*   @return		返回相机到机械臂坐标系变换矩阵 endHcal
	*/
	void Method_get_endHcal(std::string robot, std::string cam_cal, cv::Mat& endHcal);


	/*
	*	@brief		三点法标定
	*	@param		pointO		标定板坐标系原点
	*	@param		pointX		标定板坐标系X轴上一点
	*	@param		pointXOY	标定板坐标系XOY平面上一点
	*   @param		camHcal		标定板到棋盘格变换矩阵
	*   @param		camera_ins_H 相机内参
	*	@return					返回相机到机械臂坐标系变换矩阵 baseHcam
	*/
	void Method_ThreePointsCalibration(cv::Point3d pointO, cv::Point3d pointX, cv::Point3d pointXOY, cv::Mat calHcam, cv::Mat& baseHcam);


	/*
	*	@brief		标定结果优化
	*	@param		img_path		图片路径
	*	@param		armMat_path		胳膊末端变换矩阵路径
	*	@param		baseHcam		手眼变换矩阵初始
	*	@param		baseHcam_op		优化结果
	*/
	void Calibration_Optimization(string img_path, string armMat_path, const cv::Mat& baseHcam, cv::Mat& baseHcam_op, cv::Mat& endHcal_op);


	/*
	*	@brief		标定精度测量
	*	@param		baseHcam_op		
	*	@param		endHcal_op		
	*	@param		camera_ins_H		
	*	@param		distCoeffD		
	*/
	void Calibration_PrecisionTest(const cv::Mat& baseHcam_op, const cv::Mat& endHcal_op, const Camera_Intrinsics& camera_ins_H, double distCoeffD[5]);


private:

	/*
	*	@brief		求解方程组 Ax=xB
	*	@param		Hgij		相机到标定板的变换矩阵 calHcam
	*   @param		Hcij		机械臂末端到机械臂基座的变换矩阵 baseHend
	*   @return		返回机械臂到相机的变换矩阵 camHbase
	*/
	cv::Mat HandEyeMethod(const vector<Mat>Hgij, const vector<Mat>Hcij);


	/*
	*	@brief		三点法标定
	*   @brief		step1		输入三点，得到标定板到机械臂的变换矩阵  baseHcal
	*	@param		pointO		标定板坐标系原点
	*	@param		pointX		标定板坐标系X轴上一点
	*	@param		pointXOY	标定板坐标系XOY平面上一点
	*	@return					返回标定板到机械臂的变换矩阵  baseHcal
	*/
	cv::Mat ThreePointsCalibration(cv::Point3d pointO, cv::Point3d pointX, cv::Point3d pointXOY);


	//cv::Mat 转 mwArray
	void eigenMat2mwArray(const Eigen::Matrix4d& mat, mwArray& arr);

	//cv::Mat 转 mwArray
	void mwArray2eigenMat(const mwArray& arr, Eigen::Matrix4d& mat);
};

