#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—直线拟合与极值点寻找
int OpencvDemo056() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/twolines.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 去噪声与二值化
	cv::Mat binary;
	cv::Canny(src, binary, 80, 160, 3, false);

	// 形态学操作-膨胀
	cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));
	cv::dilate(binary, binary, k);

	// 轮廓发现
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());

    cv::Mat dst = src.clone();

    for (size_t t = 0; t < contours.size(); t++) {
		// 最大外接矩形
		cv::Rect rect = cv::boundingRect(contours[t]);

		int rect_max = cv::max(rect.width, rect.height);
		if ((rect_max < 30) || (rect_max > 300)) continue;

		// 直线拟合
		cv::Vec4f line;
		cv::fitLine(contours[t], line, cv::DIST_L1, 0, 0.01, 0.01);
		float vx = line[0];
		float vy = line[1];
		float x0 = line[2];
		float y0 = line[3];

		// 直线参数斜率k与截矩b
		float k = vy / vx;
		float b = y0 - k * x0;

		// 寻找轮廓极值点
		int x_min, y_min = 10000;
		int x_max, y_max = 0;
		for (size_t i = 0; i < contours[t].size(); i++) {
			cv::Point pt = contours[t][i];
			if (y_min > pt.y) {
				y_min = pt.y;
			}
			if (y_max < pt.y) {
				y_max = pt.y;
			}
		}
        x_min = (y_min - b) / k;
		x_max = (y_max - b) / k;

        cv::line(dst, cv::Point(x_min, y_min), cv::Point(x_max, y_max), cv::Scalar(0, 0, 255), 2, cv::LINE_8, 0);
    }

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.4, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of line fitting", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.4, cv::Scalar(0, 0, 255), 1);
	cv::imshow("line fitting", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
