#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// OpenCV自定义的滤波器
int OpencvDemo030() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	cv::Mat kernel1 = cv::Mat::ones(5, 5, CV_32F) / (float)(25);
	cv::Mat kernel2 = (cv::Mat_<char>(3, 3) <<  0, -1,  0,
											   -1,  5, -1,
											    0, -1,  0);
	cv::Mat kernel3 = (cv::Mat_<int>(2, 2) <<  1,  0,
											   0, -1);

	cv::Mat dst1, dst2, dst3;
	cv::filter2D(src, dst1, -1, kernel1);
	cv::filter2D(src, dst2, -1, kernel2);
	cv::filter2D(src, dst3, CV_32F, kernel3);
	cv::convertScaleAbs(dst3, dst3);

	cv::imshow("blur=5x5", dst1);
	cv::imshow("sharpen=3x3", dst2);
	cv::imshow("gradient=2x2", dst3);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
