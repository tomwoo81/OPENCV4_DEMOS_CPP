#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像锐化
int OpencvDemo034() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test1.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat sharpen_op = (cv::Mat_<char>(3, 3) <<  0, -1,  0,
												  -1,  5, -1,
												   0, -1,  0);

	cv::Mat dst;
	cv::filter2D(src, dst, CV_32F, sharpen_op);
	cv::convertScaleAbs(dst, dst);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::putText(result, "sharpen image", cv::Point(w + 10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::imshow("sharpen", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
