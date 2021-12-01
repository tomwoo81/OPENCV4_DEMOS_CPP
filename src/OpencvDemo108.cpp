#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 特征提取之关键点检测—GFTTDetector
int OpencvDemo108() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test1.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    // 通过GFTTDetector检测关键点
    auto gfft = cv::GFTTDetector::create(1000, 0.01, 1.0, 3, false, 0.04);

	std::vector<cv::KeyPoint> kps;
	gfft->detect(src, kps);

    cv::Mat dst;

    cv::drawKeypoints(src, kps, dst, cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of GFTT keypoint detection", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("GFTT keypoint detection", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
