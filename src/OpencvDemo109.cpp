#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// BLOB特征分析—SimpleBlobDetector使用
int OpencvDemo109() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/sunflowers.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    cv::Mat gray;

    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

	// 初始化参数设置
	cv::SimpleBlobDetector::Params params;

	params.minThreshold = 10;
	params.maxThreshold = 200;
    params.filterByArea = true;
    params.minArea = 100;
    params.filterByCircularity = true;
    params.minCircularity = 0.1;
	params.filterByConvexity = true;
	params.minConvexity = 0.87;
	params.filterByInertia = true;
	params.minInertiaRatio = 0.01;

    // 通过SimpleBlobDetector检测关键点
    auto blob = cv::SimpleBlobDetector::create(params);

	std::vector<cv::KeyPoint> kps;
	blob->detect(gray, kps);

    cv::Mat dst;

    cv::drawKeypoints(src, kps, dst, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of blob keypoint detection", cv::Point(10, h + 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("blob keypoint detection", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
