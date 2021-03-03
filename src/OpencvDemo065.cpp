#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像形态学–闭操作
int OpencvDemo065() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/cells.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 二值化图像
	cv::Mat gray, binary;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// 定义结构元素
    cv::Mat se1 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(25, 16), cv::Point(-1, -1));
    cv::Mat se2 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(16, 25), cv::Point(-1, -1));

    cv::Mat close = binary.clone();

    // 闭操作
    for (size_t i = 0; i < 3; i++) {
        cv::morphologyEx(close, close, cv::MORPH_CLOSE, se1);
        cv::morphologyEx(close, close, cv::MORPH_CLOSE, se2);
    }

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
	cv::imshow("closing", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
