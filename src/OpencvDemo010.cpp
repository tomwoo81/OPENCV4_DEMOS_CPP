#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像像素值统计
int OpencvDemo010() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);
	char str[256];

	cv::Mat src = cv::imread("images/test.png", cv::IMREAD_GRAYSCALE);
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
	cv::minMaxLoc(src, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("min: %.2f, max: %.2f", minVal, maxVal));
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("min loc: (%d, %d), max loc: (%d, %d)", minLoc.x, minLoc.y, maxLoc.x, maxLoc.y));

	// 彩色图像 三通道的 均值与标准差
	src = cv::imread("images/test.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::Mat means, stddevs;
	cv::meanStdDev(src, means, stddevs);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("blue channel ->> mean: %.2f, stddev: %.2f", means.at<double>(0, 0), stddevs.at<double>(0, 0)));
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("green channel ->> mean: %.2f, stddev: %.2f", means.at<double>(1, 0), stddevs.at<double>(1, 0)));
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format(str, "red channel ->> mean: %.2f, stddev: %.2f", means.at<double>(2, 0), stddevs.at<double>(2, 0)));

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
