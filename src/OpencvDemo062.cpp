#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像形态学—膨胀与腐蚀
int OpencvDemo062() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/wm.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 定义结构元素 3x3大小矩形
	cv::Mat se = cv::Mat::ones(cv::Size(3, 3), CV_8U);

    cv::Mat dilate, erode;

	// 膨胀
	cv::dilate(src, dilate, se, cv::Point(-1, -1), 1);

	// 腐蚀
	cv::erode(src, erode, se, cv::Point(-1, -1), 1);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 3, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dilate.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
    erode.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image after dilation", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "image after erosion", cv::Point(w * 2 + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("dilation & erosion", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
