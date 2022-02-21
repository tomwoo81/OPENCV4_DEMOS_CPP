#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// KMeans图像分割—主色彩提取
int OpencvDemo113() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/master.jpg");
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
    int clusterCount = 4;
	cv::Mat labels;
	cv::Mat centers;
    cv::TermCriteria term_crit = cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 10, 0.1);
	cv::kmeans(data, clusterCount, labels, term_crit, clusterCount, cv::KMEANS_PP_CENTERS, centers);

	// 计算各聚类的比例
	std::vector<float> clusters(clusterCount);
	for (int i = 0; i < labels.rows; i++) {
		clusters[labels.at<int>(i, 0)]++;
	}
	for (size_t i = 0; i < clusters.size(); i++) {
		clusters[i] = clusters[i] / sampleCount;
	}

	// 建立色卡
	cv::Mat card = cv::Mat(cv::Size(width, 50), CV_8UC3);
	int x_offset = 0;
	for (int i = 0; i < clusterCount; i++) {
		cv::Rect rect;
		rect.x = x_offset;
		rect.y = 0;
		rect.width = round(clusters[i] * width);
		rect.height = 50;

		int b = centers.at<float>(i, 0);
		int g = centers.at<float>(i, 1);
		int r = centers.at<float>(i, 2);
		cv::rectangle(card, rect, cv::Scalar(b, g, r), cv::FILLED, cv::LINE_8, 0);

		x_offset += rect.width;
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w, 50 + h), src.type());
	card.copyTo(result(cv::Range(0, 50), cv::Range(0, w)));
	src.copyTo(result(cv::Range(50, 50 + h), cv::Range(0, w)));
	cv::putText(result, "colour card", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "original image", cv::Point(10, 50 + 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("K-means clustering - primary colour extraction", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
