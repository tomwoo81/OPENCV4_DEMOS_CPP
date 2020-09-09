#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// 图像梯度–更多梯度算子
int OpencvDemo032() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/test.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}
	cv::namedWindow("input", cv::WINDOW_AUTOSIZE);
	cv::imshow("input", src);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());

	cv::Mat robert_x = (cv::Mat_<int>(2, 2) <<  1,  0,
												0, -1);
	cv::Mat robert_y = (cv::Mat_<int>(2, 2) <<  0, -1,
												1,  0);

	cv::Mat robert_grad_x, robert_grad_y;
	cv::filter2D(src, robert_grad_x, CV_16S, robert_x);
	cv::filter2D(src, robert_grad_y, CV_16S, robert_y);
	cv::convertScaleAbs(robert_grad_x, robert_grad_x);
	cv::convertScaleAbs(robert_grad_y, robert_grad_y);

	robert_grad_x.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	robert_grad_y.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "robert operator (x)", cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::putText(result, "robert operator (y)", cv::Point(w + 10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::imshow("gradient - robert operator", result);

	cv::Mat prewitt_x = (cv::Mat_<char>(3, 3) << -1,  0,  1,
												 -1,  0,  1,
												 -1,  0,  1);
	cv::Mat prewitt_y = (cv::Mat_<char>(3, 3) << -1, -1, -1,
												  0,  0,  0,
												  1,  1,  1);

	cv::Mat prewitt_grad_x, prewitt_grad_y;
	cv::filter2D(src, prewitt_grad_x, CV_32F, prewitt_x);
	cv::filter2D(src, prewitt_grad_y, CV_32F, prewitt_y);
	cv::convertScaleAbs(prewitt_grad_x, prewitt_grad_x);
	cv::convertScaleAbs(prewitt_grad_y, prewitt_grad_y);

	prewitt_grad_x.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	prewitt_grad_y.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "prewitt operator (x)", cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::putText(result, "prewitt operator (y)", cv::Point(w + 10, 30), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0, 255, 255), 1);
	cv::imshow("gradient - prewitt operator", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
