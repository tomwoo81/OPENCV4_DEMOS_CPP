#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void process_frame(const cv::Mat& src, cv::Mat& dst, int opt);

// 视频读写与处理
int OpencvDemo077() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// 打开摄像头
	// cv::VideoCapture capture(0);

	// 打开文件
	cv::VideoCapture capture("videos/roadcars.avi");
	if (!capture.isOpened()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a video file!");
		return cv::Error::StsError;
	}

	int width = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH);
	int height = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = capture.get(cv::CAP_PROP_FPS);
	int num_of_frames = (int)capture.get(cv::CAP_PROP_FRAME_COUNT);
    CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("frame width: %d, frame height: %d, FPS: %.0f, number of frames: %d", 
                                                width, height, fps, num_of_frames));

	cv::Mat src, dst, result;
	std::string winName = "video processing";
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
    int index = 0;

	while (capture.read(src)) {
		char c = cv::waitKey(50);
		if (c >= 49) { // '1'
			index = c - 49;
		}

        process_frame(src, dst, index);

		int h = src.rows;
		int w = src.cols;
		if (result.empty()) {
			result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
		}
		src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
		if (index == 2) {
			cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
		}
		dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
		cv::putText(result, "original frame", cv::Point(10, 20), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
		cv::putText(result, "processed frame", cv::Point(w + 10, 20), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
		cv::imshow(winName, result);

		if (c == 27) { // ESC
			break;
		}
	}

    cv::waitKey(0);

	return cv::Error::StsOk;
}

static void process_frame(const cv::Mat& src, cv::Mat& dst, int opt) {
	if (opt == 0) {
		cv::bitwise_not(src, dst);
	}
	else if (opt == 1) {
		cv::GaussianBlur(src, dst, cv::Size(0, 0), 5);
	}
	else if (opt == 2) {
		cv::Canny(src, dst, 100, 200);
	}
	else {
		dst = src.clone();
	}
}

/* end of file */
