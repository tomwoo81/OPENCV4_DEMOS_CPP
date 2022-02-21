#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 决策树算法 介绍与使用
int OpencvDemo116() {
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

	// 训练RTrees模型
	cv::Ptr<cv::ml::RTrees> pRTrees = cv::ml::RTrees::create();
	// pRTrees->setMaxDepth(10);
	// pRTrees->setMinSampleCount(10);
	// pRTrees->setRegressionAccuracy(0);
	// pRTrees->setUseSurrogates(false);
	// pRTrees->setMaxCategories(15);
	// pRTrees->setPriors(cv::Mat());
	// pRTrees->setCalculateVarImportance(true);
	// pRTrees->setActiveVarCount(4);
	cv::TermCriteria term_crit = cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 100, 0.01);
	pRTrees->setTermCriteria(term_crit);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, "RTrees training starts...");
	pRTrees->train(trainData, cv::ml::ROW_SAMPLE, trainLabels);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, "RTrees training done.");

	// 创建测试数据
	cv::Mat testData = cv::Mat(2500, 400, CV_32F);
	cv::Mat testLabels = cv::Mat(2500, 1, CV_32S);
	for (int i = 0, j = 0; i < images.rows; i++) {
		if (i % 100 >= 50) {
			images(cv::Range(i, i + 1), cv::Range::all()).copyTo(testData(cv::Range(j, j + 1), cv::Range::all()));
			j++;
		}
	}
	for (int i = 0, j = 0; i < labels.rows; i++) {
		if (i % 100 >= 50) {
			labels(cv::Range(i, i + 1), cv::Range::all()).copyTo(testLabels(cv::Range(j, j + 1), cv::Range::all()));
			j++;
		}
	}

    // 使用RTrees分类器进行预测
	cv::Mat results;
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, "RTrees prediction starts...");
	pRTrees->predict(testData, results);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, "RTrees prediction done.");
	int correct_count = 0;
	for (int i = 0; i < results.rows; i++) {
		if ((int)results.at<float>(i, 0) == testLabels.at<int>(i, 0)) {
			correct_count++;
		}
	}
	float accuracy = (float)correct_count / results.rows;
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("accuracy: %.3f", accuracy));

	// 测试2张图片
	cv::Mat t1 = cv::imread("images/knn_01.png", cv::IMREAD_GRAYSCALE);
	if (t1.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat t2 = cv::imread("images/knn_02.png", cv::IMREAD_GRAYSCALE);
	if (t2.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat m1, m2;
	cv::resize(t1, m1, cv::Size(20, 20));
	cv::resize(t2, m2, cv::Size(20, 20));

	testData = cv::Mat(2, 400, CV_8U);
	testLabels = cv::Mat(2, 1, CV_8U);
	rect.x = 0;
	rect.width = 400;
	rect.height = 1;
	cv::Mat one_row_1 = m1.reshape(1, 1);
	cv::Mat one_row_2 = m2.reshape(1, 1);
	rect.y = 0;
	one_row_1.copyTo(testData(rect));
	rect.y = 1;
	one_row_2.copyTo(testData(rect));
	testLabels.at<uchar>(0, 0) = 1;
	testLabels.at<uchar>(1, 0) = 2;

	testData.convertTo(testData, CV_32F);
	testLabels.convertTo(testLabels, CV_32S);

	pRTrees->predict(testData, results);

	CV_LOG_INFO(CV_LOGTAG_GLOBAL, 
				cv::format("RTrees prediction - image 1: label: %d, prediction: %d", testLabels.at<int>(0, 0), (int)results.at<float>(0, 0)));
	cv::imshow("RTrees prediction - image 1", t1);

	CV_LOG_INFO(CV_LOGTAG_GLOBAL, 
				cv::format("RTrees prediction - image 2: label: %d, prediction: %d", testLabels.at<int>(1, 0), (int)results.at<float>(1, 0)));
	cv::imshow("RTrees prediction - image 2", t2);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
