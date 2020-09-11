#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像直方图比较
int OpencvDemo019() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src1 = cv::imread("images/m1.png");
	cv::Mat src2 = cv::imread("images/m2.png");
	cv::Mat src3 = cv::imread("images/flower.png");
	cv::Mat src4 = cv::imread("images/test.png");
	if (src1.empty() || src2.empty() || src3.empty() || src4.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load image(s)!");
		return cv::Error::StsError;
	}
	cv::imshow("input1", src1);
	cv::imshow("input2", src2);
	cv::imshow("input3", src3);
	cv::imshow("input4", src4);

	cv::Mat hsv1, hsv2, hsv3, hsv4;
	cv::cvtColor(src1, hsv1, cv::COLOR_BGR2HSV);
	cv::cvtColor(src2, hsv2, cv::COLOR_BGR2HSV);
	cv::cvtColor(src3, hsv3, cv::COLOR_BGR2HSV);
	cv::cvtColor(src4, hsv4, cv::COLOR_BGR2HSV);

	int h_bins = 60; int s_bins = 64;
	int histSize[] = { h_bins, s_bins };
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges };
	int channels[] = { 0, 1 };
	cv::Mat hist1, hist2, hist3, hist4;
	cv::calcHist(&hsv1, 1, channels, cv::Mat(), hist1, 2, histSize, ranges, true, false);
	cv::calcHist(&hsv2, 1, channels, cv::Mat(), hist2, 2, histSize, ranges, true, false);
	cv::calcHist(&hsv3, 1, channels, cv::Mat(), hist3, 2, histSize, ranges, true, false);
	cv::calcHist(&hsv4, 1, channels, cv::Mat(), hist4, 2, histSize, ranges, true, false);

	cv::normalize(hist1, hist1, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(hist2, hist2, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(hist3, hist3, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
	cv::normalize(hist4, hist4, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

	for (int i = 0; i < 4; i++)
	{
		int compare_method = i;
		double src1_src2 = cv::compareHist(hist1, hist2, compare_method);
		double src3_src4 = cv::compareHist(hist3, hist4, compare_method);
		CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("Method [%d]: src1_src2: %f, src3_src4: %f", i, src1_src2, src3_src4));
	}

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
