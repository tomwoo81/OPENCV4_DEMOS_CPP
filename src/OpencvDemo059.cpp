#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—霍夫直线检测
int OpencvDemo059() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/sudoku.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 去噪声与二值化
	cv::Mat binary;
	cv::Canny(src, binary, 80, 160, 3, false);

	// 标准霍夫直线检测
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(binary, lines, 1, CV_PI / 180, 150, 0, 0);

    cv::Mat dst = src.clone();

	// 绘制直线
	cv::Point pt1, pt2;
	for (size_t i = 0; i < lines.size(); i++) {
		float rho = lines[i][0];
		float theta = lines[i][1];
		double a = cos(theta), b = sin(theta);
		double x0 = rho * a, y0 = rho * b;
		pt1 = cv::Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
		pt2 = cv::Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
		cv::line(dst, pt1, pt2, cv::Scalar(255, 0, 0), 1, cv::LINE_AA);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of linear detection", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("linear detection", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
