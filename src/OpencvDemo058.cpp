#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 二值图像分析—寻找最大内接圆
int OpencvDemo058() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

    // 绘制六边形
	const int r = 100;
	cv::Mat src = cv::Mat::zeros(cv::Size(4 * r, 4 * r), CV_8U);
	std::vector<cv::Point2f> vert(6);
	vert[0] = cv::Point(3 * r / 2, 1.34 * r);
	vert[1] = cv::Point(1 * r, 2 * r);
	vert[2] = cv::Point(3 * r / 2, 2.866 * r);
	vert[3] = cv::Point(5 * r / 2, 2.866 * r);
	vert[4] = cv::Point(3 * r, 2 * r);
	vert[5] = cv::Point(5 * r / 2, 1.34 * r);
	for (int i = 0; i < 6; i++)
	{
		cv::line(src, vert[i], vert[(i + 1) % 6], cv::Scalar(255), 3);
	}

	// 轮廓发现
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(src, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    // 计算每个像素到轮廓的距离
	cv::Mat raw_dist(src.size(), CV_32F);
	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
			raw_dist.at<float>(row, col) = (float)pointPolygonTest(contours[0], cv::Point(col, row), true);
		}
	}

	// 获取最大内接圆的半径和圆心
	double minVal, maxVal;
	cv::Point maxValPt; // center of inscribed circle
    cv::minMaxLoc(raw_dist, &minVal, &maxVal, NULL, &maxValPt);
	minVal = cv::abs(minVal);
	maxVal = cv::abs(maxVal);

	cv::Mat dst = cv::Mat::zeros(src.size(), CV_8UC3);
	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
			if (raw_dist.at<float>(row, col) < 0)
			{
				dst.at<cv::Vec3b>(row, col)[0] = (uchar)(255 - cv::abs(raw_dist.at<float>(row, col)) * 255 / minVal);
			}
			else if (raw_dist.at<float>(row, col) > 0)
			{
				dst.at<cv::Vec3b>(row, col)[2] = (uchar)(255 - raw_dist.at<float>(row, col) * 255 / maxVal);
			}
			else
			{
				dst.at<cv::Vec3b>(row, col)[0] = 255;
				dst.at<cv::Vec3b>(row, col)[1] = 255;
				dst.at<cv::Vec3b>(row, col)[2] = 255;
			}
		}
	}

	// 绘制最大内接圆
    cv::circle(dst, maxValPt, maxVal, cv::Scalar(255, 255, 255));

    cv::cvtColor(src, src, cv::COLOR_GRAY2BGR);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 255, 0), 1);
	cv::putText(result, "image with maximum inscribed circle", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 255, 0), 1);
	cv::imshow("maximum inscribed circle searching", result);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
