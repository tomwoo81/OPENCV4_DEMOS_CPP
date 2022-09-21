#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像处理: 分水岭分割案例
int main(int argc, const char *argv[])
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

    cv::Mat src = cv::imread("images/pill_002.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// 去噪声（这步很重要）
	cv::Mat shifted;
	cv::pyrMeanShiftFiltering(src, shifted, 21, 51);

    // binarisation 1
    cv::Mat gray, binary;
	cv::cvtColor(shifted, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	// distance transform
	cv::Mat dist;
	cv::distanceTransform(binary, dist, cv::DistanceTypes::DIST_L2, 3, CV_32F);
	cv::normalize(dist, dist, 0, 1, cv::NORM_MINMAX);

	// binarisation 2
    cv::Mat dist_binary;
	cv::threshold(dist, dist_binary, 0.4, 1, cv::THRESH_BINARY);

	// contours
	dist_binary.convertTo(dist_binary, CV_8U);
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(dist_binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// create markers
	cv::Mat markers = cv::Mat::zeros(src.size(), CV_32SC1), 
            contours_img = cv::Mat::zeros(src.size(), CV_8U);
	size_t i;
	for (i = 0; i < contours.size(); i++) {
		cv::drawContours(markers, contours, i, cv::Scalar(i + 1), cv::FILLED);
        cv::drawContours(contours_img, contours, i, cv::Scalar(255), 1);
	}
	cv::circle(markers, cv::Point(5, 5), 3, cv::Scalar(i + 1), cv::FILLED);
	cv::circle(contours_img, cv::Point(5, 5), 3, cv::Scalar(255), 1);

	// 形态学操作 - 彩色图像（目的是去掉干扰，让结果更好）
    cv::Mat eroded;
	cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::morphologyEx(src, eroded, cv::MORPH_ERODE, k);

	// 完成分水岭变换
	cv::watershed(eroded, markers);
	cv::Mat markers_8u;
	cv::normalize(markers, markers_8u, 0, 255, cv::NORM_MINMAX);
	cv::convertScaleAbs(markers_8u, markers_8u);

	// generate random colours
    cv::RNG rng(12345);
	std::vector<cv::Vec3b> colors;
	for (size_t i = 0; i < contours.size() + 1; i++) {
		int b = rng.uniform(0, 256);
		int g = rng.uniform(0, 256);
		int r = rng.uniform(0, 256);
		colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
	}

	// 颜色填充与最终显示
	cv::Mat dst = cv::Mat::zeros(markers.size(), CV_8UC3);
	for (int row = 0; row < markers.rows; row++) {
		for (int col = 0; col < markers.cols; col++) {
			int index = markers.at<int>(row, col);
			if (index > 0 && index <= contours.size() + 1) { // marked regions
				dst.at<cv::Vec3b>(row, col) = colors[index - 1];
			} else if (index == -1) { // region boundaries
				dst.at<cv::Vec3b>(row, col) = cv::Vec3b(255, 255, 255);
			}
            else { // unmarked regions
                dst.at<cv::Vec3b>(row, col) = cv::Vec3b(0, 0, 0);
            }
		}
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 4, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	shifted.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::cvtColor(binary, binary, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	binary.copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
	cv::normalize(dist, dist, 0, 255, cv::NORM_MINMAX);
	cv::convertScaleAbs(dist, dist);
	cv::cvtColor(dist, dist, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	dist.copyTo(result(cv::Range(0, h), cv::Range(w * 3, w * 4)));
	cv::normalize(dist_binary, dist_binary, 0, 255, cv::NORM_MINMAX);
	cv::convertScaleAbs(dist_binary, dist_binary);
	cv::cvtColor(dist_binary, dist_binary, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	dist_binary.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	cv::cvtColor(contours_img, contours_img, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	contours_img.copyTo(result(cv::Range(h, h * 2), cv::Range(w, w * 2)));
	cv::cvtColor(markers_8u, markers_8u, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	markers_8u.copyTo(result(cv::Range(h, h * 2), cv::Range(w * 2, w * 3)));
	dst.copyTo(result(cv::Range(h, h * 2), cv::Range(w * 3, w * 4)));
	cv::putText(result, "original", cv::Point(10, 20), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "meanshift filtering", cv::Point(w + 10, 20), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "binarised 1", cv::Point(w * 2 + 10, 20), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "distance transform", cv::Point(w * 3 + 10, 20), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "binarised 2", cv::Point(10, h + 20), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "contours", cv::Point(w + 10, h + 20), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "markers after watershed", cv::Point(w * 2 + 10, h + 20), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "watershed results", cv::Point(w * 3 + 10, h + 20), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("image segmentation - watershed", result);

    cv::waitKey(0);

    return cv::Error::StsOk;
}

/* end of file */
