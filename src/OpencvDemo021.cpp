#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像卷积操作
int OpencvDemo021() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/lena.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	// 3x3 均值模糊，自定义版本实现
	int h = src.rows;
	int w = src.cols;
	cv::Mat dst = src.clone();
	for (int row = 1; row < h-1; row++) {
		for (int col = 1; col < w-1; col++) {
			cv::Vec3b p1 = src.at<cv::Vec3b>(row-1, col-1);
			cv::Vec3b p2 = src.at<cv::Vec3b>(row-1, col);
			cv::Vec3b p3 = src.at<cv::Vec3b>(row-1, col+1);
			cv::Vec3b p4 = src.at<cv::Vec3b>(row, col-1);
			cv::Vec3b p5 = src.at<cv::Vec3b>(row, col);
			cv::Vec3b p6 = src.at<cv::Vec3b>(row, col+1);
			cv::Vec3b p7 = src.at<cv::Vec3b>(row+1, col-1);
			cv::Vec3b p8 = src.at<cv::Vec3b>(row+1, col);
			cv::Vec3b p9 = src.at<cv::Vec3b>(row+1, col+1);

			int b = p1[0] + p2[0] + p3[0] + p4[0] + p5[0] + p6[0] + p7[0] + p8[0] + p9[0];
			int g = p1[1] + p2[1] + p3[1] + p4[1] + p5[1] + p6[1] + p7[1] + p8[1] + p9[1];
			int r = p1[2] + p2[2] + p3[2] + p4[2] + p5[2] + p6[2] + p7[2] + p8[2] + p9[2];

			dst.at<cv::Vec3b>(row, col)[0] = cv::saturate_cast<uchar>(b / 9);
			dst.at<cv::Vec3b>(row, col)[1] = cv::saturate_cast<uchar>(g / 9);
			dst.at<cv::Vec3b>(row, col)[2] = cv::saturate_cast<uchar>(r / 9);
		}
	}
	cv::imshow("blur", dst);
	cv::imwrite("output/blur.png", dst);

	// OpenCV 均值模糊
	cv::Mat result;
	cv::blur(src, result, cv::Size(15, 15), cv::Point(-1, -1), cv::BORDER_DEFAULT);
	cv::imshow("result", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
