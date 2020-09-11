#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void pyramidDown(const cv::Mat& image, std::vector<cv::Mat>& pyramid, const int level);
static void pyramidUp(const std::vector<cv::Mat>& pyramid);

// 图像金字塔
int OpencvDemo037() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/master.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("pyramid down - level 0", cv::WINDOW_AUTOSIZE);
	cv::imshow("pyramid down - level 0", src);

	std::vector<cv::Mat> pyramid;
	pyramidDown(src, pyramid, 3);
	pyramidUp(pyramid);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static void pyramidDown(const cv::Mat& image, std::vector<cv::Mat>& pyramid, const int level)
{
	cv::Mat temp = image.clone();
	cv::Mat dst;

	for (int i = 0; i < level; i++) {
		cv::pyrDown(temp, dst);
		cv::imshow(cv::format("pyramid down - level %d", i + 1), dst);

		temp = dst.clone();
		pyramid.push_back(temp);
	}
}

static void pyramidUp(const std::vector<cv::Mat>& pyramid)
{
	cv::Mat dst;

	for (int i = pyramid.size() - 1; i >= 0; i--) {
		cv::pyrUp(pyramid[i], dst);
		cv::imshow(cv::format("pyramid up - level %d", i), dst);
	}
}

/* end of file */
