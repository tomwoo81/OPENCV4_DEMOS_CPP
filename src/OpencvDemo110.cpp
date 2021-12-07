#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// KMeans数据分类
int OpencvDemo110() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

    cv::RNG rng(12345);

    cv::Mat img(500, 500, CV_8UC3);

	int numCluster = 2;
	int sampleCount = rng.uniform(5, 500);
	cv::Mat points(sampleCount, 1, CV_32FC2);

	// 生成随机数
	for (int k = 0; k < numCluster; k++) {
		cv::Point center;
		center.x = rng.uniform(0, img.cols);
		center.y = rng.uniform(0, img.rows);
		cv::Mat pointChunk = points.rowRange(k * sampleCount / numCluster,
			k == numCluster - 1 ? sampleCount : (k + 1) * sampleCount / numCluster);
		rng.fill(pointChunk, cv::RNG::NORMAL, cv::Scalar(center.x, center.y), cv::Scalar(img.cols * 0.05, img.rows * 0.05));
	}
	cv::randShuffle(points, 1, &rng);

	// 使用KMeans进行数据分类
	cv::Mat labels;
	cv::Mat centers;
	cv::TermCriteria term_crit = cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 10, 0.1);
	cv::kmeans(points, numCluster, labels, term_crit, 3, cv::KMEANS_PP_CENTERS, centers);

	cv::Scalar colorTab[] = {
		cv::Scalar(0, 0, 255),
		cv::Scalar(255, 0, 0)
	};

	img = cv::Scalar::all(255);

	// 用不同颜色显示分类
	for (int i = 0; i < sampleCount; i++) {
		int index = labels.at<int>(i);
		cv::Point pt = points.at<cv::Point2f>(i);
		cv::circle(img, pt, 2, colorTab[index], cv::FILLED, cv::LINE_8);
	}

	// 为每个聚类的中心绘制圆
	for (int i = 0; i < centers.rows; i++) {
		int x = centers.at<float>(i, 0);
		int y = centers.at<float>(i, 1);
		CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("index: %d, center: (%d, %d)", i, x, y));
		cv::circle(img, cv::Point(x, y), 40, colorTab[i], 1, cv::LINE_AA);
	}

	cv::imshow("K-means clustering", img);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
