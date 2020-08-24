#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像翻转
int OpencvDemo013() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	cv::Mat dst;
	// X Flip 倒影
	cv::flip(src, dst, 0);
	cv::imshow("x-flip", dst);

	// Y Flip 镜像
	cv::flip(src, dst, 1);
	cv::imshow("y-flip", dst);

	// XY Flip 对角
	cv::flip(src, dst, -1);
	cv::imshow("xy-flip", dst);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
