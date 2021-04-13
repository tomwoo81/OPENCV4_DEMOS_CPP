#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像去水印/修复
int OpencvDemo075() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

    // inpainting - 1
	cv::Mat src = cv::imread("images/wm.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    cv::Mat hsv, mask;

	// inpainting mask
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
	cv::inRange(hsv, cv::Scalar(100, 43, 46), cv::Scalar(124, 255, 255), mask);
	cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
	cv::dilate(mask, mask, se);

    cv::Mat dst;

    // inpainting (image restoration)
    cv::inpaint(src, mask, dst, 3, cv::INPAINT_TELEA);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 3, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
    mask.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "inpainting mask", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "inpainted image", cv::Point(w * 2 + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("inpainting - 1", result);

    // inpainting - 2
	src = cv::imread("images/master2.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    // inpainting mask
    cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
	cv::inRange(hsv, cv::Scalar(100, 43, 46), cv::Scalar(124, 255, 255), mask);
	se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));
	cv::dilate(mask, mask, se);

    // inpainting (image restoration)
    cv::inpaint(src, mask, dst, 3, cv::INPAINT_TELEA);

	h = src.rows;
	w = src.cols;
	result = cv::Mat::zeros(cv::Size(w * 3, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
    mask.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "inpainting mask", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "inpainted image", cv::Point(w * 2 + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("inpainting - 2", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
