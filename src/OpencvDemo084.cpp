#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 视频分析—移动对象的KLT光流跟踪算法
int OpencvDemo084() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::VideoCapture capture("videos/color_object.mp4");
	if (!capture.isOpened()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a video file!");
		return cv::Error::StsError;
	}

    cv::Mat src;

    if (!capture.read(src)) {
        CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a frame!");
        return cv::Error::StsError;
    }

	// detector parameters
	int maxCorners = 100;
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;

    // detecting feature points
    cv::Mat old_gray, gray;
	cv::cvtColor(src, old_gray, cv::COLOR_BGR2GRAY);
	std::vector<cv::Point2f> old_pts, pts;
	cv::goodFeaturesToTrack(old_gray, old_pts, maxCorners, qualityLevel, minDistance, cv::Mat(), blockSize, false);

	cv::Mat dst, result;
	std::string winName = "Lucas-Kanade optical flow tracking";
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

	cv::RNG rng(12345);

	while (capture.read(src)) {
		char c = cv::waitKey(50);

        dst.release();
        dst = src.clone();

        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

        std::vector<uchar> status;
		std::vector<float> err;

		// tracker parameters
		cv::Size winSize = cv::Size(31, 31);
		int maxLevel = 3;
		cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 0.01);
		int flags = 0;

		// calculating an optical flow for a sparse feature set using the iterative Lucas-Kanade method with pyramids
		if (old_pts.size() > 0) {
			cv::calcOpticalFlowPyrLK(old_gray, gray, old_pts, pts, status, err, winSize, maxLevel, criteria, flags);
		}

		for (size_t i = 0; i < pts.size(); i++)
		{
			// drawing circles around feature points and lines on their optical flows
			if (status[i]) {
				int b = rng.uniform(0, 256);
				int g = rng.uniform(0, 256);
				int r = rng.uniform(0, 256);
				cv::circle(dst, pts[i], 3, cv::Scalar(b, g, r), cv::FILLED);
				cv::line(dst, old_pts[i], pts[i], cv::Scalar(b, g, r), 2);
			}
		}

		// detecting feature points
		cv::goodFeaturesToTrack(gray, old_pts, maxCorners, qualityLevel, minDistance, cv::Mat(), blockSize, false);

		pts.clear();

		// updating old gray image
		gray.copyTo(old_gray);

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
