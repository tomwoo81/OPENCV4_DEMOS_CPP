#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 视频分析—基于帧差法实现移动对象分析
int OpencvDemo087() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::VideoCapture capture("videos/bike.avi");
	if (!capture.isOpened()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a video file!");
		return cv::Error::StsError;
	}

    cv::Mat src;

    if (!capture.read(src)) {
        CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a frame!");
        return cv::Error::StsError;
    }

    cv::Mat prev_gray, next_gray;
	cv::cvtColor(src, prev_gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(prev_gray, prev_gray, cv::Size(0, 0), 15);

    cv::Mat diff, binary;

    cv::Mat dst, result;
	std::string winName = "object tracking by frame-difference method";
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

	while (capture.read(src)) {
		char c = cv::waitKey(50);

        dst = src.clone();

        cv::cvtColor(src, next_gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(next_gray, next_gray, cv::Size(0, 0), 15);

        cv::subtract(next_gray, prev_gray, diff, cv::Mat(), CV_16S);
        cv::convertScaleAbs(diff, diff);

        cv::threshold(diff, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

        cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7), cv::Point(-1, -1));
        cv::morphologyEx(binary, binary, cv::MORPH_OPEN, k);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());
        cv::drawContours(dst, contours, -1, cv::Scalar(0, 0, 255), 1, cv::LINE_8); // draw all contours found

        next_gray.copyTo(prev_gray);

		int h = src.rows;
		int w = src.cols;
		if (result.empty()) {
			result = cv::Mat::zeros(cv::Size(w * 3, h), src.type());
		}
		src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
        cv::cvtColor(binary, binary, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
		binary.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
        dst.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
		cv::putText(result, "original frame", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
		cv::putText(result, "moving objects", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
        cv::putText(result, "processed frame", cv::Point(w * 2 + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
		cv::imshow(winName, result);

        if (c == 27) { // ESC
			break;
		}
	}

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
