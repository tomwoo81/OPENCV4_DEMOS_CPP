#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void process_frame(const cv::Ptr<cv::BackgroundSubtractor>& pBgSub, const cv::Mat& src, cv::Mat& dst);

// 视频分析—背景消除与前景ROI提取
int OpencvDemo080() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::VideoCapture capture("videos/vtest.avi");
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

    cv::Ptr<cv::BackgroundSubtractor> pMOG2 = cv::createBackgroundSubtractorMOG2(500, 100, false);
	cv::Mat src, dst, result;
	std::string winName = "background elimination & foreground extraction";
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

	while (capture.read(src)) {
		char c = cv::waitKey(50);

        process_frame(pMOG2, src, dst);

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

static void process_frame(const cv::Ptr<cv::BackgroundSubtractor>& pBgSub, const cv::Mat& src, cv::Mat& dst) {
	dst = src.clone();

    // 提取前景，生成mask
	cv::Mat mask;
    pBgSub->apply(src, mask);

	// 定义结构元素
	cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 5), cv::Point(-1, -1));

	// 开操作
	cv::morphologyEx(mask, mask, cv::MORPH_OPEN, se);

	// 发现轮廓
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (size_t t = 0; t < contours.size(); t++) {
        // 过滤面积较小的轮廓
		double area = cv::contourArea(contours[t]);
		if (area < 100) {
			continue;
		}

        // 寻找轮廓的最小外接矩形
		cv::RotatedRect rotRect = cv::minAreaRect(contours[t]);

		cv::ellipse(dst, rotRect, cv::Scalar(0, 255, 0), 2, cv::LINE_8);
		cv::drawMarker(dst, rotRect.center, cv::Scalar(255, 0, 0), cv::MARKER_CROSS, 20, 2, cv::LINE_8);
	}
}

/* end of file */
