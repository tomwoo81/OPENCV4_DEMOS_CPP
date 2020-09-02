#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void add_gaussian_noise(cv::Mat& image);

// 图像去噪声
int OpencvDemo025() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/example.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	add_gaussian_noise(src);

	cv::Mat result1, result2, result3, result4;
	cv::blur(src, result1, cv::Size(5, 5));
	cv::imshow("blur", result1);

	cv::GaussianBlur(src, result2, cv::Size(5, 5), 0);
	cv::imshow("gaussian blur", result2);

	cv::medianBlur(src, result3, 5);
	cv::imshow("median blur", result3);

	cv::fastNlMeansDenoisingColored(src, result4, 15, 15, 10, 30);
	cv::imshow("non-local means denoising", result4);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static void add_gaussian_noise(cv::Mat& image) {
	cv::Mat noise = cv::Mat::zeros(image.size(), image.type());
	cv::Mat dst;

	cv::randn(noise, cv::Scalar(15, 15, 15), cv::Scalar(30, 30, 30));
	cv::add(image, noise, dst);

	cv::imshow("gaussian noise", dst);
	dst.copyTo(image);
}

/* end of file */
