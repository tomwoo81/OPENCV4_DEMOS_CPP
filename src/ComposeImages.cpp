#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

using namespace cv;

// 前景背景图像合成
int main(int argc, const char *argv[])
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

    // Generate a composite image from foreground and background images
    Mat foreground = imread("images/greenback.png");
	if (foreground.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return Error::StsError;
	}
    imshow("foreground", foreground);

    Mat hsv;
    cvtColor(foreground, hsv, COLOR_BGR2HSV);
    Mat mask, mask_inv;
    inRange(hsv, Scalar(35, 43, 46), Scalar(77, 255, 255), mask);
    imshow("mask", mask);
    bitwise_not(mask, mask_inv);
    imshow("mask_inv", mask_inv);

    Mat foreground_masked = Mat::zeros(foreground.size(), CV_8UC3);
    bitwise_and(foreground, foreground, foreground_masked, mask_inv);

    Mat background = imread("images/river.jpg");
    if (background.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return Error::StsError;
	}
    resize(background, background, foreground.size());
    imshow("background", background);

    Mat background_masked = Mat::zeros(background.size(), CV_8UC3);
    bitwise_and(background, background, background_masked, mask);

    Mat composite;
    bitwise_or(foreground_masked, background_masked, composite);
    imshow("composite", composite);

    waitKey(0);

    return Error::StsOk;
}

/* end of file */
