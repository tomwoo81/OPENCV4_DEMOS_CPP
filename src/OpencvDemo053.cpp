#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void contours_info(const cv::Mat& image, std::vector<std::vector<cv::Point>>& contours);

// 二值图像分析—Hu矩实现轮廓匹配
int OpencvDemo053() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/abc.png");
	cv::Mat tpl = cv::imread("images/a5.png");
	if (src.empty() || tpl.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load image(s)!");
		return cv::Error::StsError;
	}

	// 轮廓提取
	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> contours_tpl;
	contours_info(src, contours);
	contours_info(tpl, contours_tpl);

	cv::Mat dst = src.clone();
	cv::Mat dst_tpl = tpl.clone();

	for (uint i = 0; i < contours_tpl.size(); i++) {
		// 绘制模板轮廓
		if (i == 0) {
			cv::drawContours(dst_tpl, contours_tpl, i, cv::Scalar(0, 0, 255), 2, cv::LINE_8);
		}
		else {
			cv::drawContours(dst_tpl, contours_tpl, i, cv::Scalar(0, 255, 0), 2, cv::LINE_8);
		}
	}

	// 模板轮廓Hu矩计算
	cv::Moments moments_tpl = cv::moments(contours_tpl[0]);
	cv::Mat hu_tpl;
	cv::HuMoments(moments_tpl, hu_tpl);

	for (uint i = 0; i < contours.size(); i++) {
		// 轮廓Hu矩计算
		cv::Moments moments = cv::moments(contours[i]);
		cv::Mat hu;
		cv::HuMoments(moments, hu);

		// 轮廓匹配
		double dist = cv::matchShapes(hu, hu_tpl, cv::CONTOURS_MATCH_I1, 0);
		CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("index: %d, distance between 2 shapes: %.3f", i, dist));

		if (dist < 1) {
			CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("index: %d, MATCHED", i));
			cv::drawContours(dst, contours, i, cv::Scalar(0, 0, 255), 2, cv::LINE_8);
		}
		else {
			cv::drawContours(dst, contours, i, cv::Scalar(0, 255, 0), 2, cv::LINE_8);
		}
	}

	int h_tpl = tpl.rows;
	int w_tpl = tpl.cols;
	cv::Mat result_tpl = cv::Mat::zeros(cv::Size(w_tpl, h_tpl * 2), tpl.type());
	tpl.copyTo(result_tpl(cv::Range(0, h_tpl), cv::Range(0, w_tpl)));
	dst_tpl.copyTo(result_tpl(cv::Range(h_tpl, h_tpl * 2), cv::Range(0, w_tpl)));
	cv::putText(result_tpl, "template image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result_tpl, "template image with external contours", cv::Point(10, h_tpl + 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("contours matching - template", result_tpl);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with external contours", cv::Point(10, h + 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("contours matching - results", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static void contours_info(const cv::Mat& image, std::vector<std::vector<cv::Point>>& contours) {
	// 二值化
	cv::Mat gray, binary;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// 轮廓发现
	std::vector<cv::Vec4i> hierarchy;

	cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
}

/* end of file */
