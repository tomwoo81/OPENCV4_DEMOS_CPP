#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像均值与高斯模糊
int OpencvDemo022() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/snow.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	cv::Mat dst1, dst2;
	cv::blur(src, dst1, cv::Size(5, 5), cv::Point(-1, -1), cv::BORDER_DEFAULT);
	cv::GaussianBlur(src, dst2, cv::Size(5, 5), 15, 0, cv::BORDER_DEFAULT);
	cv::imshow("blur", dst1);
	cv::imshow("gaussian blur", dst2);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
