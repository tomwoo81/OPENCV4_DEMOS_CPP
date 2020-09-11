#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 视频文件的读写
int OpencvDemo012() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// 打开摄像头
	// cv::VideoCapture capture(0);

	// 打开文件
	cv::VideoCapture capture;
	capture.open("videos/bike.avi");
	if (!capture.isOpened()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a video file!");
		return cv::Error::StsError;
	}

	cv::Size S = cv::Size((int)capture.get(cv::CAP_PROP_FRAME_WIDTH),
		(int)capture.get(cv::CAP_PROP_FRAME_HEIGHT));
	double fps = capture.get(cv::CAP_PROP_FPS);
	CV_LOG_DEBUG(CV_LOGTAG_GLOBAL, cv::format("current fps: %.0f", fps));
	cv::VideoWriter writer("output/bike2.avi", cv::VideoWriter::fourcc('D', 'I', 'V', 'X'), fps, S, true);

	cv::Mat frame;
	cv::namedWindow("video-input", cv::WINDOW_AUTOSIZE);
	while (capture.read(frame)) {
		cv::imshow("video-input", frame);
		writer.write(frame);

		char c = cv::waitKey(50);
		if (c == 27) { // ESC
			break;
		}
	}

	capture.release();
	writer.release();

	return cv::Error::StsOk;
}

/* end of file */
