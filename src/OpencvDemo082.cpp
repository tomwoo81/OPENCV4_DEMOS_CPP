#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void process_frame(const cv::Mat& src, cv::Mat& dst);

// 角点检测—Shi-Tomasi角点检测
int OpencvDemo082() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::VideoCapture capture("videos/color_object.mp4");
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
	std::string winName = "Shi-Tomasi corner detection";
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

	while (capture.read(src)) {
		char c = cv::waitKey(50);

        process_frame(src, dst);

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

static void process_frame(const cv::Mat& src, cv::Mat& dst) {
	dst.release();
	dst = src.clone();

	// detector parameters
	int maxCorners = 100;
	double qualityLevel = 0.01;
	double minDistance = 10;

    // detecting corners
	cv::Mat gray;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	std::vector<cv::Point2f> corners;
	cv::goodFeaturesToTrack(gray, corners, maxCorners, qualityLevel, minDistance, cv::Mat(), 3, false);

    static cv::RNG rng(12345);

	// drawing circles around corners
	for (size_t i = 0; i < corners.size(); i++) {
        int b = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int r = rng.uniform(0, 256);
        cv::circle(dst, corners[i], 5, cv::Scalar(b, g, r), 2);
	}
}

/* end of file */
