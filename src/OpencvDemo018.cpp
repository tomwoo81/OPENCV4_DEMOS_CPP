#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像直方图均衡化
int OpencvDemo018() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/flower.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat gray, dst;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::namedWindow("input_gray", cv::WINDOW_AUTOSIZE);
	cv::imshow("input_gray", gray);
	cv::equalizeHist(gray, dst);
	cv::imshow("eq_gray", dst);

	cv::imshow("input_bgr", src);

	cv::Mat ycrcb;
	cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);
	std::vector<cv::Mat> channels;
	cv::split(ycrcb, channels);
	cv::equalizeHist(channels[0], channels[0]);
	cv::Mat result;
	cv::merge(channels, ycrcb);
	cv::cvtColor(ycrcb, result, cv::COLOR_YCrCb2BGR);
	cv::imshow("eq_ycrcb", result);

	cv::Mat yuv;
	cv::cvtColor(src, yuv, cv::COLOR_BGR2YUV);
	channels.clear();
	cv::split(yuv, channels);
	cv::equalizeHist(channels[0], channels[0]);
	cv::merge(channels, yuv);
	cv::cvtColor(yuv, result, cv::COLOR_YUV2BGR);
	cv::imshow("eq_yuv", result);

	cv::Mat hsv;
	cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
	channels.clear();
	cv::split(hsv, channels);
	cv::equalizeHist(channels[2], channels[2]);
	cv::merge(channels, hsv);
	cv::cvtColor(hsv, result, cv::COLOR_HSV2BGR);
	cv::imshow("eq_hsv", result);

	channels.clear();
	cv::split(src, channels);
	for (auto& channel: channels)
	{
		cv::equalizeHist(channel, channel);
	}
	cv::merge(channels, result);
	cv::imshow("eq_bgr", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
