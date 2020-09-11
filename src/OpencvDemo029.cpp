#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 快速的图像边缘滤波算法
int OpencvDemo029() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/example.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	double freq = cv::getTickFrequency();
	int64 start = cv::getTickCount();

	cv::Mat dst;
	cv::edgePreservingFilter(src, dst, cv::RECURS_FILTER, 60, 0.44);

	int64 end = cv::getTickCount();
	double time = (end - start) / (freq / 1000);
	CV_LOG_DEBUG(CV_LOGTAG_GLOBAL, cv::format("time consumed: %.3f ms", time));

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::putText(result, "filtered image", cv::Point(w + 10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::imshow("edge-preserving filtering", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
