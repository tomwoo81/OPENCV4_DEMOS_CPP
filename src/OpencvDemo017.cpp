#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void drawHistogram(const cv::Mat& image);

// 图像直方图
int OpencvDemo017() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/flower.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	drawHistogram(src);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static void drawHistogram(const cv::Mat& image) {
	// 定义参数变量
	const int bins[1] = { 256 };
	float hranges[2] = { 0, 256 };
	const float* ranges[1] = { hranges };

	int dims = image.channels();
	if (dims == 3) {
		std::vector<cv::Mat> bgr_plane;
		cv::split(image, bgr_plane);

		cv::Mat b_hist;
		cv::Mat g_hist;
		cv::Mat r_hist;

		// 计算Blue, Green, Red通道的直方图
		cv::calcHist(&bgr_plane[0], 1, 0, cv::Mat(), b_hist, 1, bins, ranges);
		cv::calcHist(&bgr_plane[1], 1, 0, cv::Mat(), g_hist, 1, bins, ranges);
		cv::calcHist(&bgr_plane[2], 1, 0, cv::Mat(), r_hist, 1, bins, ranges);

		// 显示直方图
		int hist_w = 512;
		int hist_h = 400;
		int bin_w = cvRound((double)hist_w / bins[0]);
		cv::Mat histImage = cv::Mat::zeros(hist_h, hist_w, CV_8UC3);

		// 归一化直方图数据
		cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		// 绘制直方图曲线
		for (int i = 1; i < bins[0]; i++) {
			cv::line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
					cv::Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))), cv::Scalar(255, 0, 0), 2, 8, 0);
			cv::line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
					cv::Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))), cv::Scalar(0, 255, 0), 2, 8, 0);
			cv::line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
					cv::Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))), cv::Scalar(0, 0, 255), 2, 8, 0);
		}

		// 显示直方图
		cv::namedWindow("Histogram", cv::WINDOW_AUTOSIZE);
		cv::imshow("Histogram", histImage);
	}
	else {
		cv::Mat hist;

		// 计算Blue, Green, Red通道的直方图
		cv::calcHist(&image, 1, 0, cv::Mat(), hist, 1, bins, ranges);

		// 显示直方图
		int hist_w = 512;
		int hist_h = 400;
		int bin_w = cvRound((double)hist_w / bins[0]);
		cv::Mat histImage = cv::Mat::zeros(hist_h, hist_w, CV_8UC3);

		// 归一化直方图数据
		cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		// 绘制直方图曲线
		for (int i = 1; i < bins[0]; i++) {
			cv::line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
					cv::Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))), cv::Scalar(0, 255, 0), 2, 8, 0);
		}

		// 显示直方图
		cv::namedWindow("Histogram", cv::WINDOW_AUTOSIZE);
		cv::imshow("Histogram", histImage);
	}
}

/* end of file */
