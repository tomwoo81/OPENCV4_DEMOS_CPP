#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像介绍
int OpencvDemo040() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/master.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat gray, dst;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	dst = cv::Mat::zeros(gray.size(), CV_8UC1);
	double t = 127;
	cv::Scalar m = cv::mean(gray);
	t = m[0];
	CV_LOG_DEBUG(CV_LOGTAG_GLOBAL, cv::format("threshold: %.3f", t));

	// 直接读取图像像素
	for (int i = 0; i < gray.rows; i++) {
		for (int j = 0; j < gray.cols; j++) {
			int pv = gray.at<uchar>(i, j);
			if (pv > t) {
				dst.at<uchar>(i, j) = 255;
			}
			else {
				dst.at<uchar>(i, j) = 0;
			}
		}
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "binary image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::imshow("binarize", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
