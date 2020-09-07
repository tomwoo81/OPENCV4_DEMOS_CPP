#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

void blur_demo(const cv::Mat& src, const cv::Mat& sum, cv::Mat& dst);
int get_block_sum(const cv::Mat& sum, const int x1, const int y1, const int x2, const int y2, const int i);

// 图像积分图算法
int OpencvDemo028() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test1.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 计算积分图
	cv::Mat sum, sqrsum;
	cv::integral(src, sum, sqrsum, CV_32S, CV_32F);

	cv::Mat dst;
	blur_demo(src, sum, dst);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::putText(result, "blur image", cv::Point(w + 10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::imshow("blur using integral image", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

void blur_demo(const cv::Mat& src, const cv::Mat& sum, cv::Mat& dst) {
	int w = src.cols;
	int h = src.rows;
	int ch = src.channels();
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 0;
	int ksize = 15;
	int radius = ksize / 2;
	dst = cv::Mat::zeros(src.size(), src.type());

	for (int yc = 0; yc < h; yc++) {
		y1 = (yc - radius - 1) < 0 ? 0 : (yc - radius - 1);
		y2 = (yc + radius) >= h ? (h - 1) : (yc + radius);
		for (int xc = 0; xc < w; xc++) {
			x1 = (xc - radius - 1) < 0 ? 0 : (xc - radius - 1);
			x2 = (xc + radius) >= w ? (w - 1) : (xc + radius);
			int num = (x2 - x1) * (y2 - y1);
			for (int i = 0; i < ch; i++) {
				// 积分图查找和表，计算卷积
				int s = get_block_sum(sum, x1, y1, x2, y2, i);
				dst.at<cv::Vec3b>(yc, xc)[i] = cv::saturate_cast<uchar>(s / num);
			}
		}
	}
}

int get_block_sum(const cv::Mat& sum, const int x1, const int y1, const int x2, const int y2, const int i) {
	int tl = sum.at<cv::Vec3i>(y1 + 1, x1 + 1)[i];
	int tr = sum.at<cv::Vec3i>(y1 + 1, x2 + 1)[i];
	int bl = sum.at<cv::Vec3i>(y2 + 1, x1 + 1)[i];
	int br = sum.at<cv::Vec3i>(y2 + 1, x2 + 1)[i];

	int s = (br - bl - tr + tl);

	return s;
}

/* end of file */
