#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像梯度–Sobel算子
int OpencvDemo031() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	cv::Mat grad_x, grad_y;
	cv::Sobel(src, grad_x, CV_32F, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
	cv::Sobel(src, grad_y, CV_32F, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_x, grad_x);
	cv::convertScaleAbs(grad_y, grad_y);

	cv::imshow("gradient - sobel operator (x)", grad_x);
	cv::imshow("gradient - sobel operator (y)", grad_y);

	cv::Mat dst;
	cv::add(grad_x, grad_y, dst, cv::Mat(), CV_16S);
	cv::convertScaleAbs(dst, dst);

	cv::imshow("gradient - sobel operator", dst);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
