#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

void add_salt_pepper_noise(cv::Mat& image);
void gaussian_noise(cv::Mat& image);

// 图像噪声
int OpencvDemo024() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/cos.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	add_salt_pepper_noise(src);
	gaussian_noise(src);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

void add_salt_pepper_noise(cv::Mat& image) {
	cv::RNG rng(12345);
	int h = image.rows;
	int w = image.cols;
	int nums = 10000;
	cv::Mat dst = image.clone();

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

	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), image.type());
	image.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::putText(result, "salt-pepper noise image", cv::Point(w + 10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::imshow("salt-pepper noise", result);
}

void gaussian_noise(cv::Mat& image) {
	cv::Mat noise = cv::Mat::zeros(image.size(), image.type());
	cv::Mat dst;

	cv::randn(noise, cv::Scalar(15, 15, 15), cv::Scalar(30, 30, 30));
	cv::add(image, noise, dst);

	int h = image.rows;
	int w = image.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), image.type());
	image.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::putText(result, "gaussian noise image", cv::Point(w + 10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::imshow("gaussian noise", result);
}

/* end of file */
