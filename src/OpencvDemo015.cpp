#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 几何形状绘制
int OpencvDemo015() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat image = cv::Mat::zeros(cv::Size(512, 512), CV_8UC3);
	cv::Rect rect(100, 100, 200, 200);
	cv::rectangle(image, rect, cv::Scalar(255, 0, 0), 2, cv::LINE_8, 0);
	cv::circle(image, cv::Point(256, 256), 50, cv::Scalar(0, 0, 255), 2, cv::LINE_8, 0);
	cv::ellipse(image, cv::Point(256, 256), cv::Size(150, 50), 360, 0, 360, cv::Scalar(0, 255, 0), 2, cv::LINE_8, 0);
	cv::imshow("image", image);

	cv::waitKey(0);

	cv::RNG rng(0xFFFFFF);
	image.setTo(cv::Scalar(0, 0, 0));

	for (int i = 0; i < 100000; i++) {
//		image.setTo(cv::Scalar(0, 0, 0));

		int x1 = rng.uniform(0, 512);
		int y1 = rng.uniform(0, 512);
		int x2 = rng.uniform(0, 512);
		int y2 = rng.uniform(0, 512);
		int b = rng.uniform(0, 256);
		int g = rng.uniform(0, 256);
		int r = rng.uniform(0, 256);

		cv::line(image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(b, g, r), 1, cv::LINE_AA, 0);

//		rect.x = x1;
//		rect.y = y1;
//		rect.width = x2 - x1;
//		rect.height = y2 - y1;
//		cv::rectangle(image, rect, cv::Scalar(b, g, r), 1, cv::LINE_AA, 0);

		imshow("image", image);

		char c = cv::waitKey(20);
		if (c == 27) { // ESC
			break;
		}
	}

	return cv::Error::StsOk;
}

/* end of file */
