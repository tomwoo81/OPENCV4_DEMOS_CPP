#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像像素的算术操作
int OpencvDemo005() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src1 = cv::imread("images/LinuxLogo.jpg");
	cv::Mat src2 = cv::imread("images/WindowsLogo.jpg");
	if (src1.empty() || src2.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load image(s)!");
		return cv::Error::StsError;
	}
	cv::imshow("input1", src1);
	cv::imshow("input2", src2);

	int height = src1.rows;
	int width = src1.cols;
	int b1 = 0, g1 = 0, r1 = 0;
	int b2 = 0, g2 = 0, r2 = 0;
	cv::Mat result = cv::Mat::zeros(src1.size(), src1.type());
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
				b1 = src1.at<cv::Vec3b>(row, col)[0];
				g1 = src1.at<cv::Vec3b>(row, col)[1];
				r1 = src1.at<cv::Vec3b>(row, col)[2];

				b2 = src2.at<cv::Vec3b>(row, col)[0];
				g2 = src2.at<cv::Vec3b>(row, col)[1];
				r2 = src2.at<cv::Vec3b>(row, col)[2];

				result.at<cv::Vec3b>(row, col)[0] = cv::saturate_cast<uchar>(b1 + b2);
				result.at<cv::Vec3b>(row, col)[1] = cv::saturate_cast<uchar>(g1 + g2);
				result.at<cv::Vec3b>(row, col)[2] = cv::saturate_cast<uchar>(r1 + r2);
		}
	}
	cv::imshow("output", result);

	cv::Mat add_result = cv::Mat::zeros(src1.size(), src1.type());
	cv::add(src1, src2, add_result);
	cv::imshow("add_result", add_result);

	cv::Mat sub_result = cv::Mat::zeros(src1.size(), src1.type());
	cv::subtract(src1, src2, sub_result);
	cv::imshow("sub_result", sub_result);

	cv::Mat mul_result = cv::Mat::zeros(src1.size(), src1.type());
	cv::multiply(src1, src2, mul_result);
	cv::imshow("mul_result", mul_result);

	cv::Mat div_result = cv::Mat::zeros(src1.size(), src1.type());
	cv::divide(src1, src2, div_result);
	cv::imshow("div_result", div_result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
