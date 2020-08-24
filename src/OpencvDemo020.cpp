#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

void backProjection_demo(const cv::Mat& image, const cv::Mat& model);

// 图像直方图反向投影
int OpencvDemo020() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat target = cv::imread("images/target.png");
	cv::Mat model = cv::imread("images/sample.png");
	if (target.empty() || model.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load image(s)!");
		return cv::Error::StsError;
	}
	cv::namedWindow("target", cv::WINDOW_AUTOSIZE);
	cv::imshow("target", target);
	cv::imshow("model", model);

	backProjection_demo(target, model);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

void backProjection_demo(const cv::Mat& image, const cv::Mat& model) {
	cv::Mat image_hsv, model_hsv;
	cv::cvtColor(image, image_hsv, cv::COLOR_BGR2HSV);
	cv::cvtColor(model, model_hsv, cv::COLOR_BGR2HSV);

	// 定义直方图参数与属性
	int h_bins = 32; int s_bins = 32;
	int histSize[] = { h_bins, s_bins };
	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };
	int channels[] = { 0, 1 };

	cv::Mat roiHist;
	cv::calcHist(&model_hsv, 1, channels, cv::Mat(), roiHist, 2, histSize, ranges, true, false);
	cv::normalize(roiHist, roiHist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());

	cv::Mat backProj;
	cv::calcBackProject(&image_hsv, 1, channels, roiHist, backProj, ranges, 1.0);
	cv::imshow("BackProjection", backProj);
}

/* end of file */
