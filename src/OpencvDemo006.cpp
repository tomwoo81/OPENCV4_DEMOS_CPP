#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void customColorMap(cv::Mat& image);

// LUT的作用与用法
int OpencvDemo006() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/canjian.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat gray, dst;
	// 使用LUT
	cv::applyColorMap(src, dst, cv::COLORMAP_SUMMER);
	// 显示结果
	cv::imshow("colorMap", dst);

	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	imshow("gray", gray);
	customColorMap(gray);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static void customColorMap(cv::Mat& image) {
	int lut[256];
	for (int i = 0; i < 256; i++) {
		if (i < 127)
			lut[i] = 0;
		else
			lut[i] = 255;
	}

	int h = image.rows;
	int w = image.cols;
	for (int row = 0; row < h; row++) {
		for (int col = 0; col < w; col++) {
			int pv = image.at<uchar>(row, col);
			image.at<uchar>(row, col) = lut[pv];
		}
	}
	cv::imshow("lut demo", image);
}

/* end of file */
