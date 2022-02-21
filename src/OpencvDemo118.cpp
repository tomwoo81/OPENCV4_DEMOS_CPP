#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// GrabCut图像分割
int OpencvDemo118() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/master.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat mask;
	cv::Rect rect(180, 20, 180, 220);
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

    cv::Mat dst;
    cv::bitwise_and(src, src, dst, mask);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "GrabCut segmented image", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("GrabCut image segmentation", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
