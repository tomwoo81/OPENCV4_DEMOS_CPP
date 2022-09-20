#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 前景背景图像合成
int main(int argc, const char *argv[])
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

    // Generate a composite image from foreground and background images

    cv::Mat foreground = cv::imread("images/greenback.png");
	if (foreground.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    // BGR space -> HSV space
    cv::Mat hsv;
    cv::cvtColor(foreground, hsv, cv::COLOR_BGR2HSV);
    // 提取前景与背景区域
    cv::Mat mask_inv, mask;
    cv::inRange(hsv, cv::Scalar(35, 43, 46), cv::Scalar(77, 255, 255), mask_inv);
    cv::bitwise_not(mask_inv, mask);
    // 删除干扰区域（开运算）
    cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, k);
    // 使前景与背景间边界光滑（高斯模糊）
    cv::GaussianBlur(mask, mask, cv::Size(9, 9), 0);

    cv::Mat background = cv::imread("images/river.jpg");
    if (background.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
    cv::resize(background, background, foreground.size());

    cv::Mat mask_float, mask_inv_float, foreground_float, background_float, 
            foreground_masked, background_masked, composite, composite_float;
    mask.convertTo(mask_float, CV_32F);
    cv::normalize(mask_float, mask_float, 1.0, 0, cv::NORM_INF);
    cv::subtract(1, mask_float, mask_inv_float);
    cv::cvtColor(mask_float, mask_float, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
    cv::cvtColor(mask_inv_float, mask_inv_float, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
    foreground.convertTo(foreground_float, CV_32F);
    cv::multiply(foreground_float, mask_float, foreground_masked);
    background.convertTo(background_float, CV_32F);
    cv::multiply(background_float, mask_inv_float, background_masked);
    cv::add(foreground_masked, background_masked, composite_float);
    composite_float.convertTo(composite, CV_8U);

	int h = foreground.rows;
	int w = foreground.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h * 2), foreground.type());
	foreground.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
    mask.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	background.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
    composite.copyTo(result(cv::Range(h, h * 2), cv::Range(w, w * 2)));
	cv::putText(result, "foreground", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
    cv::putText(result, "mask", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "background", cv::Point(10, h + 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
    cv::putText(result, "composite", cv::Point(w + 10, h + 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	std::string windowTitle = "image composition";
	cv::namedWindow(windowTitle, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowTitle, cv::Size(w, h));
	cv::imshow(windowTitle, result);

    cv::waitKey(0);

    return cv::Error::StsOk;
}

/* end of file */
