#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—提取最大轮廓与编码关键点
int OpencvDemo074() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/case6.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 二值化图像
	cv::Mat gray, binary;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// 定义结构元素
	cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));

    cv::Mat close;

    // 闭操作
	cv::morphologyEx(binary, close, cv::MORPH_CLOSE, se);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

    // 轮廓发现/轮廓分析
	cv::findContours(close, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat dst1 = src.clone();

    int height = src.rows;
    int width = src.cols;
    int max = 0;
    int index = -1;

    for (size_t t = 0; t < contours.size(); t++) {
		cv::Rect rect = cv::boundingRect(contours[t]);
		if (rect.height >= height || rect.width >= width) {
			continue;
		}

		double area = cv::contourArea(contours[t]);
		if (area > max) {
			max = area;
			index = t;
		}
	}

	// 绘制轮廓及其关键点
	cv::Mat dst2 = cv::Mat::zeros(src.size(), src.type());

	cv::drawContours(dst1, contours, index, cv::Scalar(0, 0, 255), 1, cv::LINE_8);
    cv::drawContours(dst2, contours, index, cv::Scalar(0, 0, 255), 1, cv::LINE_8);

    cv::Mat pts;

	cv::approxPolyDP(contours[index], pts, 4, true);

	for (size_t i = 0; i < pts.rows; i++) {
		cv::Vec2i pt = pts.at<cv::Vec2i>(i, 0);

		cv::circle(dst1, cv::Point(pt[0], pt[1]), 2, cv::Scalar(0, 255, 0), cv::FILLED, cv::LINE_8, 0);
        cv::circle(dst2, cv::Point(pt[0], pt[1]), 2, cv::Scalar(0, 255, 0), cv::FILLED, cv::LINE_8, 0);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    cv::cvtColor(binary, binary, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
    binary.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	dst1.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
    dst2.copyTo(result(cv::Range(h, h * 2), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "binarised image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image 1 with results of contour analysis", cv::Point(10, h + 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "image 2 with results of contour analysis", cv::Point(w + 10, h + 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
	std::string windowTitle = cv::format("retrieval of the maximum contour and its key points");
	cv::namedWindow(windowTitle, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowTitle, cv::Size(w, h));
	cv::imshow(windowTitle, result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
