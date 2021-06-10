#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 视频分析—基于连续自适应均值迁移的对象移动分析
int OpencvDemo089() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::VideoCapture capture("videos/balltest.mp4");
	if (!capture.isOpened()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a video file!");
		return cv::Error::StsError;
	}

    cv::Mat src;

    // 读取第一帧
    if (!capture.read(src)) {
        CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a frame!");
        return cv::Error::StsError;
    }

    cv::Rect tracking_window = cv::selectROI("select a region of interest", src, false, false);

    // 获取ROI直方图
    cv::Mat src_roi, hsv_roi, mask, roi_hist;
    src_roi = src(tracking_window);
    cv::cvtColor(src_roi, hsv_roi, cv::COLOR_BGR2HSV);
    cv::inRange(hsv_roi, cv::Scalar(26, 43, 46), cv::Scalar(34, 255, 255), mask);
    const int channels[] = { 0 };
	int hue_size = 180;
	int hist_size[] = { hue_size };
	float hue_range[] = { 0, 180 };
    const float* ranges[] = { hue_range };
    cv::calcHist(&hsv_roi, 1, channels, mask, roi_hist, 1, hist_size, ranges);
    cv::normalize(roi_hist, roi_hist, 0, 255, cv::NORM_MINMAX);

    cv::Mat hsv, back_proj;
    cv::RotatedRect tracking_box;

    cv::Mat dst, result;
	std::string winName = "object tracking by continously adaptive mean shift method";
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

	while (capture.read(src)) {
		char c = cv::waitKey(50);

        dst = src.clone();

        // 图像直方图反向投影
        cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);
        cv::calcBackProject(&hsv, 1, channels, roi_hist, back_proj, ranges, 1);

        // 通过均值迁移方法搜索更新ROI区域
        cv::TermCriteria term_crit = cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 10, 1);
        tracking_box = cv::CamShift(back_proj, tracking_window, term_crit);

        // 绘制窗口
        cv::rectangle(dst, tracking_window, cv::Scalar(0, 0, 255), 2);

        // 绘制ROI区域
        cv::ellipse(dst, tracking_box, cv::Scalar(0, 255, 0), 2);

		int h = src.rows;
		int w = src.cols;
		if (result.empty()) {
			result = cv::Mat::zeros(cv::Size(w * 3, h), src.type());
		}
		src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
        cv::cvtColor(back_proj, back_proj, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
		back_proj.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
        dst.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
		cv::putText(result, "original frame", cv::Point(10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
		cv::putText(result, "back projection", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
        cv::putText(result, "processed frame", cv::Point(w * 2 + 10, 30), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 1);
		cv::imshow(winName, result);

        if (c == 27) { // ESC
			break;
		}
	}

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
