#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void connected_components_with_stats_demo(const cv::Mat& src, cv::Mat& dst);

// 二值图像连通组件状态统计
int OpencvDemo047() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/rice.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat dst;
	connected_components_with_stats_demo(src, dst);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "labeled image", cv::Point(10, h + 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::imshow("connected components with statistics", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static void connected_components_with_stats_demo(const cv::Mat& src, cv::Mat& dst) {
	cv::Mat blurred, gray, binary;
	cv::GaussianBlur(src, blurred, cv::Size(3, 3), 0);
	cv::cvtColor(blurred, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// extract labels
	cv::Mat labels = cv::Mat::zeros(src.size(), CV_32S);
	cv::Mat stats, centroids;
	int num_labels = cv::connectedComponentsWithStats(binary, labels, stats, centroids, 8, CV_32S);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("number of foreground labels: %d", num_labels - 1));

	std::vector<cv::Vec3b> colors(num_labels);

	// background color
	colors[0] = cv::Vec3b(0, 0, 0);

	// foreground colors
	cv::RNG rng(12345);
	for (int i = 1; i < num_labels; i++) {
		colors[i] = cv::Vec3b(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
	}

	// extract statistics info
	dst = src.clone();
	for (int i = 1; i < num_labels; i++) {
		// 外接矩形
		int x = stats.at<int>(i, cv::CC_STAT_LEFT);
		int y = stats.at<int>(i, cv::CC_STAT_TOP);
		int w = stats.at<int>(i, cv::CC_STAT_WIDTH);
		int h = stats.at<int>(i, cv::CC_STAT_HEIGHT);
		int area = stats.at<int>(i, cv::CC_STAT_AREA);

		cv::Rect rect(x, y, w, h);
		cv::rectangle(dst, rect, colors[i], 1, cv::LINE_8, 0);
		cv::putText(dst, cv::format("index: %d", i), cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, .5, cv::Scalar(0, 0, 255), 1);
		CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("index: %d, area: %d", i, area));

		// 中心位置
		int cx = centroids.at<double>(i, 0);
		int cy = centroids.at<double>(i, 1);

		cv::circle(dst, cv::Point(cx, cy), 2, cv::Scalar(0, 255, 0), 2, cv::LINE_8, 0);
	}
}

/* end of file */
