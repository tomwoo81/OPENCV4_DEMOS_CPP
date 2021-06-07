#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 视频分析–稠密光流分析
int OpencvDemo086() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::VideoCapture capture("videos/vtest.avi");
	if (!capture.isOpened()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a video file!");
		return cv::Error::StsError;
	}

    cv::Mat src;

    if (!capture.read(src)) {
        CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a frame!");
        return cv::Error::StsError;
    }

    cv::Mat prev_gray, next_gray;
	cv::cvtColor(src, prev_gray, cv::COLOR_BGR2GRAY);

    cv::Mat_<cv::Point2f> flow;

	cv::Mat pts_x = cv::Mat::zeros(src.size(), CV_32FC1);
	cv::Mat pts_y = cv::Mat::zeros(src.size(), CV_32FC1);
	cv::Mat mag;
	cv::Mat ang;

	cv::Mat hsv = cv::Mat::zeros(src.size(), src.type());
	std::vector<cv::Mat> mv;
	cv::split(hsv, mv);

	cv::Mat dst, result;
	std::string winName = "Gunnar Farneback optical flow tracking";
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

	while (capture.read(src)) {
		char c = cv::waitKey(20);

        cv::cvtColor(src, next_gray, cv::COLOR_BGR2GRAY);

        cv::calcOpticalFlowFarneback(prev_gray, next_gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

		for (int row = 0; row < flow.rows; row++) {
			for (int col = 0; col < flow.cols; col++) {
				const cv::Point2f& flow_xy = flow.at<cv::Point2f>(row, col);

				pts_x.at<float>(row, col) = flow_xy.x;
				pts_y.at<float>(row, col) = flow_xy.y;
			}
		}

        cv::cartToPolar(pts_x, pts_y, mag, ang, true);

        ang = ang / 2;
        cv::convertScaleAbs(ang, ang);
		cv::normalize(mag, mag, 0, 255, cv::NORM_MINMAX);
		cv::convertScaleAbs(mag, mag);

		mv[0] = ang;
		mv[1] = cv::Scalar(255);
		mv[2] = mag;
		cv::merge(mv, hsv);

        cv::cvtColor(hsv, dst, cv::COLOR_HSV2BGR);

        next_gray.copyTo(prev_gray);

        int h = src.rows;
		int w = src.cols;
		if (result.empty()) {
			result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
		}
		src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
        dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
		cv::putText(result, "original frame", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
        cv::putText(result, "processed frame", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
		cv::imshow(winName, result);

		if (c == 27) { // ESC
			break;
		}
	}

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
