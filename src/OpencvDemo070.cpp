#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 形态学应用—用基本梯度实现轮廓分析
int OpencvDemo070() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/kd02.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 定义结构元素
	cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));

    cv::Mat basic, gray, binary;

	// 形态学梯度-基本梯度
	cv::morphologyEx(src, basic, cv::MORPH_GRADIENT, se);

    // 转灰度图像
	cv::cvtColor(basic, gray, cv::COLOR_BGR2GRAY);

    // 全局阈值二值化
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // 定义结构元素
	se = getStructuringElement(cv::MORPH_RECT, cv::Size(1, 5), cv::Point(-1, -1));

    cv::Mat dilate;

    // 膨胀
	cv::morphologyEx(binary, dilate, cv::MORPH_DILATE, se);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

    // 轮廓分析
	cv::findContours(dilate, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat dst = src.clone();

	for (size_t i = 0; i < contours.size(); i++) {
		cv::Rect rect = cv::boundingRect(contours[i]);
		double area = cv::contourArea(contours[i]);

		if (area < 200) {
            continue;
        }

        int h = rect.height;
		int w = rect.width;
		if (h > (w * 3) || h < 20) {
            continue;
        }

        cv::rectangle(dst, rect, cv::Scalar(0, 0, 255), 1, cv::LINE_8, 0);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    cv::cvtColor(binary, binary, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
    binary.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
    cv::cvtColor(dilate, dilate, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	dilate.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
    dst.copyTo(result(cv::Range(h, h * 2), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "binarised image with basic gradients", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "binarised image after dilation", cv::Point(10, h + 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "image with results", cv::Point(w + 10, h + 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
	cv::imshow("basic gradients and coutour analysis", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
