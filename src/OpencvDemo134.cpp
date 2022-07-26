#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string model_bin = "models/enet/model-best.net";

// OpenCV DNN ENet实现图像分割
int OpencvDemo134() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// load a DNN model
	cv::dnn::Net net = cv::dnn::readNetFromTorch(model_bin);
	if (net.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

	// read an image
	cv::Mat src = cv::imread("images/cityscapes_test.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat input = cv::dnn::blobFromImage(src, 0.00392, cv::Size(1024, 512), cv::Scalar(0, 0, 0), true, false);

	// run the model
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    // net.setPreferableBackend(cv::dnn::DNN_BACKEND_INFERENCE_ENGINE);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    // net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setInput(input);
    cv::Mat output = net.forward();

	// get segmentation results
	int chns = output.size[1];
	int rows = output.size[2];
	int cols = output.size[3];

	cv::Mat maxMat = cv::Mat::zeros(rows, cols, CV_32F);
	cv::Mat argMaxMat = cv::Mat::zeros(rows, cols, CV_8U);
	for (int h = 1; h < chns; h++) {
		for (int i = 0; i < rows; i++) {
			const float *pScore = output.ptr<float>(0, h, i);
			float *pMax = maxMat.ptr<float>(i);
			uchar *pArgMax = argMaxMat.ptr<uchar>(i);
			for (int j = 0; j < cols; j++) {
				if (pScore[j] > pMax[j]) {
					pMax[j] = pScore[j];
					pArgMax[j] = (uchar)h;
				}
			}
		}
	}

	cv::Mat normalized, colormapped, dst;
	cv::normalize(argMaxMat, normalized, 0, 255, cv::NORM_MINMAX);
	cv::applyColorMap(normalized, colormapped, cv::COLORMAP_JET);
	cv::resize(colormapped, colormapped, src.size());
	cv::addWeighted(src, 0.7, colormapped, 0.3, 0, dst);

	std::vector<double> timings;
	double time = net.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
	std::string text = cv::format("time: %.0f ms", time);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, text);
	cv::putText(dst, text, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0), 2);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	cv::putText(result, "original image", cv::Point(10, 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "image with results", cv::Point(10, h + 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	std::string windowTitle = "image segmentation - ENet model";
	cv::namedWindow(windowTitle, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowTitle, cv::Size(w / 3, h * 2 / 3));
	cv::imshow(windowTitle, result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
