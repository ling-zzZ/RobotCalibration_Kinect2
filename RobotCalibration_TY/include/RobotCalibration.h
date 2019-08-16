﻿#pragma once

#include "stdafx.h"

#include "CalibrationMethods.h"
#include "ImgProcess_TY.h"
#include "CameraCalibration.h"

void printMenu() {
	printf("Commands:\n");
	printf("  g  采集高清图片\n");
	printf("  i  计算内参\n");
	printf("  e  计算外参\n");
	printf("  p  标定（棋盘格在机械臂上）\n");
	printf("  o  标定结果优化（棋盘格在机械臂上，自带精度测试）\n");
	printf("  s  三点法标定 \n");
	printf("  m  精度测试(探针) \n");
	printf("  q  退出\n");
}



void CalibrationFunc() {

	//文件存储位置（调试用）
	// 存储相机内参标定图片
	std::string img_internal_file = ".\\data\\Images_internal\\";
	// 存储相机内参标定结果
	std::string cam_internal_file = ".\\data\\internal_reference.txt";

	// 存储标定时图片
	std::string cam_cal_file = ".\\data\\Images_calibration\\";
	// 标定时机械臂末端的位姿
	std::string robot_pose_file = ".\\data\\robot.txt";
	// 相机外参
	std::string cam_external_file = ".\\data\\cam_cal_robot.txt";
	// 存储标定结果
	std::string result_file = ".\\data\\result.txt";

	double markerRealSize = 2;//cm
	CameraCalibration cal = CameraCalibration(markerRealSize);

	//相机内参
	Camera_Intrinsics camera_ins_H;
	camera_ins_H.FLX = 1154.6;
	camera_ins_H.FLY = 1155.5;
	camera_ins_H.PPX = 613.4968;
	camera_ins_H.PPY = 449.4059;

	// 相机畸变
	double distCoeffD[5] = { -0.263800155103853, 0.145978275196663, -0.0808361022675977, -0.000373220970137041,	0.000490335127893494 };

	// 标定结果
	cv::Mat res_mat;

	//打印菜单
	printMenu();
	string line;
	bool running = true;
	while (running)
	{
		printf("please input the command >>> \n");
		std::getline(std::cin, line);
		switch (line[0]) {
		case 'i': //棋盘格相机标定
		{
			std::cout << "======》》》相机内参标定， 开始" << std::endl;
			// 初始化相机，存储标定内参所用的图片
			std::cout << "-*-*-*-*-开始采集标定相机内参所需图片-*-*-*-*-" << std::endl;
			ImgProcess_TY::getImage(img_internal_file, false);

			std::cout << "-*-*-*-*-开始计算相机内参-*-*-*-*-" << endl;
			cal.internal_reference_calibration(img_internal_file, cam_internal_file, camera_ins_H, distCoeffD);
			break;
		}

		case 'g': //获得图片
		{
			// 初始化相机
			std::cout << "======》》》开始采集标定机械臂和相机所需图片" << std::endl;
			ImgProcess_TY::getImage(cam_cal_file, true);
			break;
		}

		case 'e': 
		{
			std::cout << "======》》》机械臂相机标定，开始" << endl;
			cout << "-*-*-*-*-是否重新计算相机内参（输入1/0）-*-*-*-*-" << endl;
			int ok = 0;
			cin >> ok;
			if (ok) {
				std::cout << "-*-*-*-*-开始获得相机内参-*-*-*-*-" << endl;
				cal.internal_reference_calibration(cam_cal_file, cam_internal_file, camera_ins_H, distCoeffD);
			}

			std::cout << "-*-*-*-*-开始计算外参-*-*-*-*-" << endl;
			vector<cv::Mat> res;
			cal.external_reference_calibration(camera_ins_H, distCoeffD, cam_cal_file, cam_external_file, res);
			break;
		}
			
		case 'p':
		{
			std::cout << "-*-*-*-*-计算机械臂相机标定结果-*-*-*-*-" << endl;
			CalibrationMethods c;
			c.Method_BoardOnRobot(robot_pose_file, cam_external_file, result_file, res_mat);
			break;
		}

		case 'o':
		{
			std::cout << "======》》》使用matlab中算法再次计算" << endl;
			cv::Mat result, endHcal;
			CalibrationMethods c;
			c.Calibration_Optimization(cam_cal_file, robot_pose_file, res_mat, result, endHcal);

			std::cout << "-*-*-*-*-计算结果-*-*-*-*-" << endl;
			std::cout << result << endl;
			break;
		}

		case 's':
		{
			std::cout << "======》》》开始三点法标定" << endl;
			std::cout << "-*-*-*-*-请移动示教机械臂到标定板坐标系原点，x轴一点，xoy平面内一点（不一定在y轴）-*-*-*-*-" << endl;
			cv::Point3d pos_O, pos_X, pos_Y;

			std::cout << "-*-*-*-*-请输入三个标定点(X,Y,Z):" << endl;
			std::cout << "-*-*-*-*-point O:-*-*-*-*-" << endl;
			cin >> pos_O.x >> pos_O.y >> pos_O.z;

			std::cout << "-*-*-*-*-point X:-*-*-*-*-" << endl;
			cin >> pos_X.x >> pos_X.y >> pos_X.z;

			std::cout << "-*-*-*-*-point Y:-*-*-*-*-" << endl;
			cin >> pos_Y.x >> pos_Y.y >> pos_Y.z;

			std::cout << "-*-*-*-*-用相机拍摄标定板,计算相机相对于标定板的外参（可拍摄多张）-*-*-*-*-" << endl;

			// 初始化相机
			ImgProcess_TY::getImage(cam_cal_file, true);

			vector<cv::Mat> res;
			cal.external_reference_calibration(camera_ins_H, distCoeffD, cam_cal_file, cam_external_file, res);

			cv::Mat ex_mat;
			for (auto mat_ : res)
				ex_mat += mat_;

			ex_mat = ex_mat / res.size();

			CalibrationMethods c;
			c.Method_ThreePointsCalibration(pos_O, pos_X, pos_Y, ex_mat, res_mat);

			std::cout << "-*-*-*-*-计算结果-*-*-*-*-" << endl;
			std::cout << res_mat << std::endl;
			break;
		}

		case 'q':
			running = false;
			break;

		default:
			printMenu();
			break;
		}
	}

	return;
}