#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// KNN算法介绍
int OpencvDemo114() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// 读取数据
	cv::Mat data = cv::imread("images/digits.png");
	if (data.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat gray;
	cv::cvtColor(data, gray, cv::COLOR_BGR2GRAY);

	cv::Mat images = cv::Mat(5000, 400, CV_8U);
	cv::Mat labels = cv::Mat(5000, 1, CV_8U);
	cv::Rect rect;
    rect.width = 20;
	rect.height = 20;
	int index = 0;
	cv::Rect roi;
	roi.x = 0;
    roi.width = 400;
	roi.height = 1;
	for (int row = 0; row < 50; row++) {
		int label = row / 5;
		for (int col = 0; col < 100; col++) {
			cv::Mat digit = cv::Mat(20, 20, CV_8U);
			index = row * 100 + col;
			rect.x = col * 20;
			rect.y = row * 20;
			gray(rect).copyTo(digit);
			cv::Mat one_row = digit.reshape(1, 1);
			roi.y = index;
			one_row.copyTo(images(roi));
			labels.at<uchar>(index, 0) = label;
		}
	}

	images.convertTo(images, CV_32F);
	labels.convertTo(labels, CV_32S);

	// 创建训练数据
	cv::Mat trainData = cv::Mat(2500, 400, CV_32F);
	cv::Mat trainLabels = cv::Mat(2500, 1, CV_32S);
	for (int i = 0, j = 0; i < images.rows; i++) {
		if (i % 100 < 50) {
			images(cv::Range(i, i + 1), cv::Range::all()).copyTo(trainData(cv::Range(j, j + 1), cv::Range::all()));
			j++;
		}
	}
	for (int i = 0, j = 0; i < labels.rows; i++) {
		if (i % 100 < 50) {
			labels(cv::Range(i, i + 1), cv::Range::all()).copyTo(trainLabels(cv::Range(j, j + 1), cv::Range::all()));
			j++;
		}
	}

	// 训练KNN模型
	cv::Ptr<cv::ml::KNearest> pKNN = cv::ml::KNearest::create();
	pKNN->setDefaultK(5);
	pKNN->setIsClassifier(true);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, "KNN training starts...");
	pKNN->train(trainData, cv::ml::ROW_SAMPLE, trainLabels);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, "KNN training done.");
	pKNN->save("models/knn_digits.yml");

	return cv::Error::StsOk;
}

/* end of file */
