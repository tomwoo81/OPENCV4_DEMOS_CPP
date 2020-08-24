#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像对象的创建与赋值
int OpencvDemo003() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	// 创建方法 - 克隆
	cv::Mat m1 = src.clone();

	// 复制
	cv::Mat m2;
	src.copyTo(m2);

	// 赋值法
	cv::Mat m3 = src;

	// 创建空白图像
	cv::Mat m4 = cv::Mat::zeros(src.size(), src.type());
	cv::Mat m5 = cv::Mat::zeros(cv::Size(512, 512), CV_8UC3);
	cv::Mat m6 = cv::Mat::ones(cv::Size(512, 512), CV_8UC3);

	cv::Mat kernel = (cv::Mat_<char>(3, 3) << 0, -1, 0,
		-1, 5, -1,
		0, -1, 0);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
