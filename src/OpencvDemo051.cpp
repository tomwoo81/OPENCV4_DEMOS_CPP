#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—使用轮廓逼近
int OpencvDemo051() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/contours.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 二值化
	cv::Mat gray, binary;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// 轮廓发现
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());

	cv::Mat dst = src.clone();

	for (auto& contour: contours) {
		// 最小外接矩形
		cv::RotatedRect rotRect = cv::minAreaRect(contour);

		// 最小外接矩形的中心位置
		cv::Point2f center = rotRect.center;

		// 多边形逼近
		cv::Mat result;
		cv::approxPolyDP(contour, result, 4, true);
		int numVertices = result.rows;
		CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("number of vertices: %d", numVertices));

		// 形状判断
		if (numVertices == 3) {
			cv::putText(dst, "triangle", center, cv::FONT_HERSHEY_SIMPLEX, .7, cv::Scalar(255, 0, 0), 2, cv::LINE_8);
		}
		else if (numVertices == 4) {
			cv::putText(dst, "rectangle", center, cv::FONT_HERSHEY_SIMPLEX, .7, cv::Scalar(255, 0, 0), 2, cv::LINE_8);
		}
		else if (numVertices == 6) {
			cv::putText(dst, "polygon", center, cv::FONT_HERSHEY_SIMPLEX, .7, cv::Scalar(255, 0, 0), 2, cv::LINE_8);
		}
		else if (numVertices > 10) {
			cv::putText(dst, "circle", center, cv::FONT_HERSHEY_SIMPLEX, .7, cv::Scalar(255, 0, 0), 2, cv::LINE_8);
		}
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of polygonal approximation", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("polygonal approximation", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
