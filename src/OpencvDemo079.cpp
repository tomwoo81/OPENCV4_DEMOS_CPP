#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 视频分析—背景/前景提取
int OpencvDemo079() {
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

    cv::Ptr<cv::BackgroundSubtractor> pMOG2 = cv::createBackgroundSubtractorMOG2(500, 1000, false);
	cv::Mat src, mask, background, result;
	std::string winName = "foreground/background segmentation";
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

	while (capture.read(src)) {
		char c = cv::waitKey(50);

        pMOG2->apply(src, mask);
        pMOG2->getBackgroundImage(background);

		int h = src.rows;
		int w = src.cols;
		if (result.empty()) {
			result = cv::Mat::zeros(cv::Size(w * 3, h), src.type());
		}
		src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
        cv::cvtColor(mask, mask, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
		mask.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
        background.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
		cv::putText(result, "original frame", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
		cv::putText(result, "foreground mask", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
        cv::putText(result, "background image", cv::Point(w * 2 + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
		cv::imshow(winName, result);

		if (c == 27) { // ESC
			break;
		}
	}

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
