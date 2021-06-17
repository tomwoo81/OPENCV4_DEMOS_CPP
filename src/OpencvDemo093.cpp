#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 对象检测—LBP特征介绍
int OpencvDemo093() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// 打开摄像头
	cv::VideoCapture capture(0);
	if (!capture.isOpened()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not open a camera!");
		return cv::Error::StsError;
	}

    cv::CascadeClassifier face_detector;
    face_detector.load("models/lbpcascade_frontalface_improved.xml");

    std::vector<cv::Rect> faces;

    cv::Mat src, gray, dst, result;
	std::string winName = "LBP cascade classifiers";
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

	while (capture.read(src)) {
		char c = cv::waitKey(5);

        dst = src.clone();

		cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
		cv::equalizeHist(gray, gray);

        face_detector.detectMultiScale(gray, faces, 1.2, 1, 0, cv::Size(30, 30), cv::Size(400, 400));

		for (size_t t = 0; t < faces.size(); t++) {
			cv::rectangle(dst, faces[t], cv::Scalar(0, 0, 255), 2, cv::LINE_8, 0);
		}

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
