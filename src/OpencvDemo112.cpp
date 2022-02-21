#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// KMeans图像分割—背景替换
int OpencvDemo112() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/toux.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	int width = src.cols;
	int height = src.rows;

	// 初始化定义
	int sampleCount = width * height;

	// 将RGB数据转换为样本数据
	cv::Mat sample_data = src.reshape(3, sampleCount);
	cv::Mat data;
	sample_data.convertTo(data, CV_32F);

	// 使用KMeans进行图像分割
    int clusterCount = 3;
	cv::Mat labels;
	cv::Mat centers;
    cv::TermCriteria term_crit = cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 10, 0.1);
	cv::kmeans(data, clusterCount, labels, term_crit, clusterCount, cv::KMEANS_PP_CENTERS, centers);

	// 生成掩膜
	cv::Mat mask = cv::Mat::zeros(src.size(), CV_8U);
	int index = labels.at<int>(0, 0);
	labels = labels.reshape(1, height);
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			int c = labels.at<int>(row, col);
			if (c == index) {
				mask.at<uchar>(row, col) = 255;
			}
		}
	}

	// 对掩膜进行高斯模糊
	cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));
	cv::dilate(mask, mask, se);
	cv::GaussianBlur(mask, mask, cv::Size(5, 5), 0);

	// 融合前景与新背景
	cv::Mat dst = cv::Mat(src.size(), src.type());
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			float w1 = float(mask.at<uchar>(row, col)) / 255;
			cv::Vec3b bgr = src.at<cv::Vec3b>(row, col);
			bgr[0] = 255 * w1 + bgr[0] * (1 - w1);
			bgr[1] = 0 * w1 + bgr[1] * (1 - w1);
			bgr[2] = 255 * w1 + bgr[2] * (1 - w1);
			dst.at<cv::Vec3b>(row, col) = bgr;
		}
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 3, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	mask.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
    dst.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 255, 0), 1);
	cv::putText(result, "background mask", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 255, 0), 1);
    cv::putText(result, "image after background replacement", cv::Point(w * 2 + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 255, 0), 1);
	cv::imshow("K-means clustering - background replacement", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
