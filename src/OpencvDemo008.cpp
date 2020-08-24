#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 通道分离与合并
int OpencvDemo008() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/flower.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	std::vector<cv::Mat> mv;
	cv::Mat dst1, dst2, dst3;

	// 蓝色通道为零
	cv::split(src, mv);
	mv[0] = cv::Scalar(0);
	cv::merge(mv, dst1);
	cv::imshow("output1", dst1);

	// 绿色通道为零
	cv::split(src, mv);
	mv[1] = cv::Scalar(0);
	cv::merge(mv, dst2);
	cv::imshow("output2", dst2);

	// 红色通道为零
	cv::split(src, mv);
	mv[2] = cv::Scalar(0);
	cv::merge(mv, dst3);
	cv::imshow("output3", dst3);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
