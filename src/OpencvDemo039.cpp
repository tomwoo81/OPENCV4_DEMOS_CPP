#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像模板匹配
int OpencvDemo039() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/llk.jpg");
	cv::Mat tpl = cv::imread("images/llk_tpl.png");
	if (src.empty() || tpl.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load image(s)!");
		return cv::Error::StsError;
	}
	cv::imshow("input", src);
	cv::imshow("template", tpl);

	int result_w = src.cols - tpl.cols + 1;
	int result_h = src.rows - tpl.rows + 1;
	cv::Mat result = cv::Mat::zeros(cv::Size(result_w, result_h), CV_32FC1);
	cv::matchTemplate(src, tpl, result, cv::TM_CCORR_NORMED);
	cv::imshow("comparison results", result);

	const float t = 0.98;

	for (int i = 0; i < result_h; i++) {
		for (int j = 0; j < result_w; j++) {
			float v = result.at<float>(i, j);
			if (v > t) {
				cv::rectangle(src, cv::Point(j, i), cv::Point(j + tpl.cols, i + tpl.rows), cv::Scalar(255, 0, 0), 1, cv::LINE_8, 0);
			}
		}
	}
	cv::imshow("template-matched results", src);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
