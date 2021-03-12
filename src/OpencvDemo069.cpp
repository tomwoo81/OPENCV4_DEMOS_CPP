#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像形态学—图像梯度
int OpencvDemo069() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/wm.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 定义结构元素
	cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));

    cv::Mat basic, external, internal, dilate, erode;

	// 基本梯度
	cv::morphologyEx(src, basic, cv::MORPH_GRADIENT, se);

	// 外梯度
	cv::morphologyEx(src, dilate, cv::MORPH_DILATE, se);
	cv::subtract(dilate, src, external);

	// 内梯度
	cv::morphologyEx(src, erode, cv::MORPH_ERODE, se);
	cv::subtract(src, erode, internal);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    basic.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	external.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
    internal.copyTo(result(cv::Range(h, h * 2), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "image with basic gradients", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with external gradients", cv::Point(10, h + 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "image with internal gradients", cv::Point(w + 10, h + 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("gradients", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
