#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二维码检测与识别
int OpencvDemo120() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/qrcode_06.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat dst = src.clone();

	cv::Mat gray;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

	cv::QRCodeDetector detector;
	std::vector<cv::Point> points;
	cv::Mat straight_qrcode;
	std::string info;
	bool ret = detector.detect(gray, points);
	if (ret) {
		info = detector.decode(gray, points, straight_qrcode);
	}

	std::vector<std::vector<cv::Point>> contours;
	contours.emplace_back(points);
	cv::drawContours(dst, contours, 0, cv::Scalar(0, 0, 255), 2);
	cv::circle(dst, points[0], 2, cv::Scalar(0, 255, 0), cv::FILLED);
	cv::circle(dst, points[1], 2, cv::Scalar(255, 0, 0), cv::FILLED);
	cv::circle(dst, points[3], 2, cv::Scalar(0, 255, 255), cv::FILLED);

	cv::imshow("rectified and binarised QR code", straight_qrcode);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.5, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of QR code detection and decoding", cv::Point(10, h + 30), cv::FONT_ITALIC, 0.5, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, info, cv::Point(10, h + 350), cv::FONT_ITALIC, 0.4, cv::Scalar(255, 0, 0), 1);
	cv::imshow("QR code detection and decoding", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
