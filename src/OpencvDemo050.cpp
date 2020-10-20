#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—轮廓面积与弧长
int OpencvDemo050() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/zhifang_ball.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 去噪声与二值化
	cv::Mat binary;
	cv::Canny(src, binary, 80, 160, 3, false);

	// 形态学操作-膨胀
	cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));
	cv::dilate(binary, binary, k);

	// 轮廓发现
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());

	cv::Mat dst1 = src.clone();
	cv::Mat dst2 = src.clone();

	for (auto& contour: contours) {
		// 面积过滤
		double area = cv::contourArea(contour);
		double arcLen = cv::arcLength(contour, true);
		if (area < 100 || arcLen < 100) {
			continue;
		}

		// 最大外接矩形
		cv::Rect rect = cv::boundingRect(contour);

		// 绘制矩形
		cv::rectangle(dst1, rect, cv::Scalar(0, 255, 0), 1, cv::LINE_8, 0);

		// 最小外接矩形
		cv::RotatedRect rotRect = cv::minAreaRect(contour);
		cv::Point2f pts[4];
		rotRect.points(pts);

		// 绘制旋转矩形与中心位置
		for (int i = 0; i < 4; i++) {
			cv::line(dst2, pts[i % 4], pts[(i + 1) % 4], cv::Scalar(0, 0, 255), 2, cv::LINE_8, 0);
		}
		cv::Point2f center = rotRect.center;
		cv::circle(dst2, center, 2, cv::Scalar(255, 0, 0), cv::FILLED, cv::LINE_8, 0);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 3, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst1.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	dst2.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.7, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with up-right bounding rectangles", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.7, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with rotated rectangles of the minimum areas", cv::Point(w * 2 + 10, 30), cv::FONT_ITALIC, 0.7, cv::Scalar(0, 0, 255), 1);
	cv::imshow("bounding rectangles of contours (area >= 100 and arcLen >= 100)", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
