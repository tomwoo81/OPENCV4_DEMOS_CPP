#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// SIFT特征提取—关键点提取
int OpencvDemo098() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/flower.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	auto sift = cv::SIFT::create();

	std::vector<cv::KeyPoint> kps;
	sift->detect(src, kps);

    cv::Mat dst;

	cv::drawKeypoints(src, kps, dst, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.4, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of SIFT keypoint detection", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.4, cv::Scalar(0, 0, 255), 1);
	cv::imshow("SIFT keypoint detection", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
