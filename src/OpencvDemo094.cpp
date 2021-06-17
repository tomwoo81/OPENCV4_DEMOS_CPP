#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// ORB FAST特征关键点检测
int OpencvDemo094() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test2.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	auto orb = cv::ORB::create(1000);

	std::vector<cv::KeyPoint> kps;
	orb->detect(src, kps);

    cv::Mat dst;

	cv::drawKeypoints(src, kps, dst, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of ORB detection", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("ORB keypoint detection and descriptor extraction", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
