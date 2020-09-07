#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void add_salt_pepper_noise(const cv::Mat& src, cv::Mat& dst);
static void add_gaussian_noise(const cv::Mat& src, cv::Mat& dst);

// 图像噪声
int OpencvDemo024() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/cos.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat dst;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());

	add_salt_pepper_noise(src, dst);

	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::putText(result, "salt-pepper noise image", cv::Point(w + 10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::imshow("salt-pepper noise", result);

	add_gaussian_noise(src, dst);

	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::putText(result, "gaussian noise image", cv::Point(w + 10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::imshow("gaussian noise", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static void add_salt_pepper_noise(const cv::Mat& src, cv::Mat& dst) {
	cv::RNG rng(12345);
	int h = src.rows;
	int w = src.cols;
	int nums = 10000;
	dst = src.clone();

	for (int i = 0; i < nums; i++) {
		int x = rng.uniform(0, w);
		int y = rng.uniform(0, h);
		if (i % 2 == 1) {
			dst.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
		}
		else {
			dst.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
		}
	}
}

static void add_gaussian_noise(const cv::Mat& src, cv::Mat& dst) {
	cv::Mat noise = cv::Mat::zeros(src.size(), src.type());

	cv::randn(noise, cv::Scalar(15, 15, 15), cv::Scalar(30, 30, 30));
	cv::add(src, noise, dst);
}

/* end of file */
