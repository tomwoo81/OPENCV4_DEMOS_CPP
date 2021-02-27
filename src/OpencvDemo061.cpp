#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—霍夫圆检测
int OpencvDemo061() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/coins.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    cv::Mat gray;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(gray, gray, cv::Size(9, 9), 2, 2);

	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 2, 10, 100, 100, 20, 100);

    cv::Mat dst = src.clone();

	for (size_t i = 0; i < circles.size(); i++)
	{
		cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);

		// 绘制圆
		cv::circle(dst, center, 3, cv::Scalar(0, 255, 0), cv::FILLED, cv::LINE_8, 0);
		cv::circle(dst, center, radius, cv::Scalar(0, 0, 255), 3, cv::LINE_8, 0);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of circular detection", cv::Point(10, h + 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("circular detection", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
