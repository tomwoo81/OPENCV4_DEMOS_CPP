#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void process_frame(const cv::Mat& src, cv::Mat& dst);

// 识别与跟踪视频中的特定颜色对象
int OpencvDemo078() {
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
	std::string winName = "object recognition & tracking in video";
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

	cv::Mat hsv, mask;
	cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

	// 定义结构元素
	cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15), cv::Point(-1, -1));

	// 颜色提取 - 红色
	cv::inRange(hsv, cv::Scalar(0, 43, 46), cv::Scalar(10, 255, 255), mask);

	// 开操作
	cv::morphologyEx(mask, mask, cv::MORPH_OPEN, se);

	// 发现轮廓
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// 寻找最大轮廓
	int index = -1;
	int max = 0;
	for (size_t t = 0; t < contours.size(); t++) {
		double area = cv::contourArea(contours[t]);
		if (area > max) {
			max = area;
			index = t;
		}
	}

	// 寻找最大轮廓的最小外接矩形
	if (index >= 0) {
		cv::RotatedRect rotRect = cv::minAreaRect(contours[index]);

		cv::ellipse(dst, rotRect, cv::Scalar(0, 255, 0), 2, cv::LINE_8);
		cv::drawMarker(dst, rotRect.center, cv::Scalar(255, 0, 0), cv::MARKER_CROSS, 20, 2, cv::LINE_8);
	}
}

/* end of file */
