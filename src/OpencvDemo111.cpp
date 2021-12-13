#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// KMeans图像分割
int OpencvDemo111() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/toux.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	int width = src.cols;
	int height = src.rows;
	int dims = src.channels();

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

	cv::Scalar colorTab[] = {
		cv::Scalar(0, 0, 255),
		cv::Scalar(0, 255, 0),
		cv::Scalar(255, 0, 0)
	};

	// 显示图像分割结果
	int index;
	cv::Mat dst = cv::Mat(src.size(), src.type());
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			index = row * width + col;
			int label = labels.at<int>(index, 0);
			dst.at<cv::Vec3b>(row, col)[0] = colorTab[label][0];
			dst.at<cv::Vec3b>(row, col)[1] = colorTab[label][1];
			dst.at<cv::Vec3b>(row, col)[2] = colorTab[label][2];
		}
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.5, cv::Scalar(0, 255, 0), 1);
	cv::putText(result, "image with results of K-means clustering", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.5, cv::Scalar(0, 255, 0), 1);
	cv::imshow("K-means clustering", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
