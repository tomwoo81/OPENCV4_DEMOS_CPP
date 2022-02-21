#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像均值漂移分割
int OpencvDemo117() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/yuan_test.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat dst;
    cv::TermCriteria term_crit = cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 10, 0.1);
	cv::pyrMeanShiftFiltering(src, dst, 20, 40, 2, term_crit);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "mean shift segmented image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("mean shift segmentation", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
