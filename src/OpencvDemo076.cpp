#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像透视变换应用
int OpencvDemo076() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/st_02.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 二值化图像
	cv::Mat gray, binary;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

	// 定义结构元素
	cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));

    cv::Mat open;

    // 开操作
	cv::morphologyEx(binary, open, cv::MORPH_OPEN, se);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	// 寻找最大轮廓
	cv::findContours(open, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	int max = 0;
    int index = -1;

	for (size_t t = 0; t < contours.size(); t++) {
		double area = cv::contourArea(contours[t]);
		if (area > max) {
			max = area;
			index = t;
		}
	}

	// 寻找最小外接矩形
	cv::RotatedRect rect = cv::minAreaRect(contours[index]);
	int width = static_cast<int>(rect.size.height);
	int height = static_cast<int>(rect.size.width);
	CV_LOG_DEBUG(CV_LOGTAG_GLOBAL, cv::format("width: %d, height: %d, angle: %.3f", width, height, rect.angle));

	cv::Point2f vertices[4];
	rect.points(vertices);
	cv::Mat dst1 = src.clone();
	std::vector<cv::Point> src_pts;

	for (int i = 0; i < 4; i++) {
		CV_LOG_DEBUG(CV_LOGTAG_GLOBAL, cv::format("x: %.3f, y: %.3f", vertices[i].x, vertices[i].y));
		cv::drawMarker(dst1, vertices[i], cv::Scalar(0, 0, 255), cv::MARKER_CROSS, 20, 1, cv::LINE_8);
		src_pts.push_back(vertices[i]);
	}

	std::vector<cv::Point> dst_pts;

	dst_pts.push_back(cv::Point(width, height));
	dst_pts.push_back(cv::Point(0, height));
	dst_pts.push_back(cv::Point(0, 0));
	dst_pts.push_back(cv::Point(width, 0));

	// 透视变换
	cv::Mat M = cv::findHomography(src_pts, dst_pts);
	cv::Mat dst2;
	cv::warpPerspective(src, dst2, M, cv::Size(width, height));

	if (height < width) {
		cv::rotate(dst2, dst2, cv::ROTATE_90_CLOCKWISE);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    dst1.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "image with source points", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("perspective transformation - A", result);
	cv::putText(dst2, "image after perspective transformation", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("perspective transformation - B", dst2);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
