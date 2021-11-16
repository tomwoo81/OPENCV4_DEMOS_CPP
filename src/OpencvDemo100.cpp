#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// HOG特征与行人检测
int OpencvDemo100() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/pedestrian.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    cv::HOGDescriptor hog;

    hog.setSVMDetector(hog.getDefaultPeopleDetector());

    std::vector<cv::Rect> rects;

    // Detect pedestrians in the image
    hog.detectMultiScale(src, rects, 0.0, cv::Size(4, 4), cv::Size(8, 8), 1.25);

    cv::Mat dst = src.clone();

    for (const auto& rect: rects) {
        cv::rectangle(dst, rect, cv::Scalar(0, 255, 0), 2, cv::LINE_8, 0);
    }

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of HOG pedestrian detection", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("HOG pedestrian detection", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
