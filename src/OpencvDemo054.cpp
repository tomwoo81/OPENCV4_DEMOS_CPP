#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—对轮廓圆与椭圆拟合
int OpencvDemo054() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/stuff.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 去噪声与二值化
	cv::Mat binary;
	cv::Canny(src, binary, 80, 160, 3, false);

	// 形态学操作-膨胀
	cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));
	cv::dilate(binary, binary, k);

	// 轮廓发现
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());

	cv::Mat dst = src.clone();

	for (size_t t = 0; t < contours.size(); t++) {
		// 椭圆拟合
		cv::RotatedRect rotRect = cv::fitEllipse(contours[t]);
		float w = rotRect.size.width;
		float h = rotRect.size.height;

        // 绘制椭圆
		cv::ellipse(dst, rotRect, cv::Scalar(0, 0, 255), 2, cv::LINE_8);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of ellipse fitting", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("ellipse fitting", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
