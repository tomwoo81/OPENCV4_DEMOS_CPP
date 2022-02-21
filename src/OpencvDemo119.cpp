#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// GrabCut图像分割–背景替换
int OpencvDemo119() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/master.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat mask;
	cv::Rect rect(53, 12, src.cols - 100, src.rows - 12);
	cv::Mat bgdModel = cv::Mat::zeros(1, 65, CV_64F);
	cv::Mat fgdModel = cv::Mat::zeros(1, 65, CV_64F);
	cv::grabCut(src, mask, rect, bgdModel, fgdModel, 5, cv::GC_INIT_WITH_RECT);

	for (int row = 0; row < mask.rows; row++) {
		for (int col = 0; col < mask.cols; col++) {
			int pv = mask.at<uchar>(row, col);
			if (pv == cv::GC_FGD || pv == cv::GC_PR_FGD) {
				mask.at<uchar>(row, col) = 255;
			}
			else {
				mask.at<uchar>(row, col) = 0;
			}
		}
	}

	// 对掩膜进行高斯模糊
	cv::GaussianBlur(mask, mask, cv::Size(5, 5), 0);

	cv::Mat bgd = cv::imread("images/river.jpg");
	if (bgd.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::resize(bgd, bgd, src.size());

	// 虚化背景
	cv::Mat bgd2;
	cv::GaussianBlur(bgd, bgd2, cv::Size(0, 0), 15);

	// 融合前景与虚化背景
	cv::Mat dst = cv::Mat(src.size(), src.type());
	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
			float w1 = float(mask.at<uchar>(row, col)) / 255;
			cv::Vec3b bgr = src.at<cv::Vec3b>(row, col);
			cv::Vec3b bgr_bgd2 = bgd2.at<cv::Vec3b>(row, col);
			bgr[0] = bgr[0] * w1 + bgr_bgd2[0] * (1 - w1);
			bgr[1] = bgr[1] * w1 + bgr_bgd2[1] * (1 - w1);
			bgr[2] = bgr[2] * w1 + bgr_bgd2[2] * (1 - w1);
			dst.at<cv::Vec3b>(row, col) = bgr;
		}
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 4, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	mask.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	bgd.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
    dst.copyTo(result(cv::Range(0, h), cv::Range(w * 3, w * 4)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "foreground mask", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "background image", cv::Point(w * 2 + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "image after background replacement", cv::Point(w * 3 + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("GrabCut image segmentation - background replacement", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
