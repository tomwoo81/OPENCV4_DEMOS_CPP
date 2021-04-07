#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—缺陷检测一
int OpencvDemo072() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/ce_02.jpg");
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

    // 轮廓发现/轮廓分析
	cv::findContours(open, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat dst = src.clone();

    int height = src.rows;

	for (size_t t = 0; t < contours.size(); t++) {
		cv::Rect rect = cv::boundingRect(contours[t]);
		double area = cv::contourArea(contours[t]);

		if (rect.height > (height / 2)) {
			continue;
		}

		if (area < 150) {
            continue;
        }

		cv::rectangle(dst, rect, cv::Scalar(0, 0, 255), 1, cv::LINE_8, 0);
		cv::drawContours(dst, contours, t, cv::Scalar(0, 255, 0), 2, cv::LINE_8);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of contour analysis", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("defect detection - 1", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
