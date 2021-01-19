#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—凸包检测
int OpencvDemo055() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/hand.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 二值化
	cv::Mat gray, binary;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// 删除干扰块（开运算）
	cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));
	cv::morphologyEx(binary, binary, cv::MORPH_OPEN, k);

	// 轮廓发现
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());

    cv::Mat dst = src.clone();

    for (size_t t = 0; t < contours.size(); t++) {
		bool isHull = cv::isContourConvex(contours[t]);
        CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("A convex contour? %c", isHull ? 'Y' : 'N'));

        std::vector<cv::Point> hull;
		cv::convexHull(contours[t], hull);

		size_t size = hull.size();
		for (size_t i = 0; i < size; i++) {
			cv::circle(dst, hull[i], 4, cv::Scalar(255, 0, 0), 2, cv::LINE_8, 0);
			cv::line(dst, hull[i], hull[(i + 1) % size], cv::Scalar(0, 0, 255), 2, cv::LINE_8, 0);
		}
    }

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of convex hull detection", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("convex hull detection", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
