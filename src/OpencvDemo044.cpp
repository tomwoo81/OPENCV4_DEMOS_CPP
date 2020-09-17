#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 自适应阈值算法
int OpencvDemo044() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/text1.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat gray, dst1, dst2;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

	// OTSU二值寻找算法
	double t = cv::threshold(gray, dst1, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	CV_LOG_DEBUG(CV_LOGTAG_GLOBAL, cv::format("threshold - otsu: %.0f", t));

	// 自适应阈值算法(mean)
	cv::adaptiveThreshold(gray, dst2, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 25, 10);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 3, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	cv::cvtColor(dst1, dst1, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	dst1.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::cvtColor(dst2, dst2, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	dst2.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "binary image (otsu)", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "binary image (adaptive)", cv::Point(w * 2 + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 2);
	cv::imshow("binarize - adaptive threshold (mean)", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
