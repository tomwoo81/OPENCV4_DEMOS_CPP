#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 均值迁移模糊
int OpencvDemo027() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/example.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat dst;
	cv::pyrMeanShiftFiltering(src, dst, 15, 30, 1, cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 5, 1));

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 255, 255), 1);
	cv::putText(result, "mean shift filtering image", cv::Point(w + 10, 30), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 255, 255), 1);
	cv::imshow("mean shift filtering", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
