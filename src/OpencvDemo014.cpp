#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像插值
int OpencvDemo014() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	int h = src.rows;
	int w = src.cols;
	float fx = 0.0, fy = 0.0;

	cv::Mat dst = cv::Mat::zeros(src.size(), src.type());
	cv::resize(src, dst, cv::Size(w * 2, h * 2), fx = 0, fy = 0, cv::INTER_NEAREST);
	cv::imshow("INTER_NEAREST", dst);

	cv::resize(src, dst, cv::Size(w * 2, h * 2), fx = 0, fy = 0, cv::INTER_LINEAR);
	cv::imshow("INTER_LINEAR", dst);

	cv::resize(src, dst, cv::Size(w * 2, h * 2), fx = 0, fy = 0, cv::INTER_CUBIC);
	cv::imshow("INTER_CUBIC", dst);

	cv::resize(src, dst, cv::Size(w * 2, h * 2), fx = 0, fy = 0, cv::INTER_LANCZOS4);
	cv::imshow("INTER_LANCZOS4", dst);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
