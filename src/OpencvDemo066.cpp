#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static int open_demo();
static int close_demo();

// 图像形态学—开闭操作时候结构元素应用演示
int OpencvDemo066() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

    if (cv::Error::StsOk != open_demo()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "open_demo() failed!");
		return cv::Error::StsError;
    }

    if (cv::Error::StsOk != close_demo()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "close_demo() failed!");
		return cv::Error::StsError;
    }

	return cv::Error::StsOk;
}

static int open_demo() {
	cv::Mat src = cv::imread("images/fill.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 二值化图像
	cv::Mat gray, binary;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

	// 定义结构元素
	cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(20, 1), cv::Point(-1, -1));

    cv::Mat open;

    // 开操作
	cv::morphologyEx(binary, open, cv::MORPH_OPEN, se);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

    // 提取轮廓
	cv::findContours(open, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());

    cv::Mat dst = src.clone();

	for (size_t t = 0; t < contours.size(); t++) {
		cv::Rect roi = cv::boundingRect(contours[t]);
		roi.y = roi.y - 10;
		roi.height = 12;
		cv::rectangle(dst, roi, cv::Scalar(0, 0, 255), 1, cv::LINE_8, 0);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    cv::cvtColor(binary, binary, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
    binary.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
    cv::cvtColor(open, open, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	open.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
    dst.copyTo(result(cv::Range(h, h * 2), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "binarised image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image after opening", cv::Point(10, h + 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "image with results", cv::Point(w + 10, h + 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("opening demo", result);

    cv::waitKey(0);

	cv::destroyWindow("opening demo");

    return cv::Error::StsOk;
}

static int close_demo() {
	cv::Mat src = cv::imread("images/morph3.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 二值化图像
	cv::Mat gray, binary;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// 定义结构元素
	cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(25, 25), cv::Point(-1, -1));

    cv::Mat close;

    // 闭操作
	cv::morphologyEx(binary, close, cv::MORPH_CLOSE, se);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 3, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    cv::cvtColor(binary, binary, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	binary.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
    cv::cvtColor(close, close, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
    close.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "binarised image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "image after closing", cv::Point(w * 2 + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("closing demo", result);

    cv::waitKey(0);

	cv::destroyWindow("closing demo");

    return cv::Error::StsOk;
}

/* end of file */
