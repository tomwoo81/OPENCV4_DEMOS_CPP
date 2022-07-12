#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string model_bin = "models/ssd/MobileNetSSD_deploy.caffemodel";
static std::string model_txt = "models/ssd/MobileNetSSD_deploy.prototxt";

static std::string objNames[] = { "background",
"aeroplane", "bicycle", "bird", "boat",
"bottle", "bus", "car", "cat", "chair",
"cow", "diningtable", "dog", "horse",
"motorbike", "person", "pottedplant",
"sheep", "sofa", "train", "tvmonitor" };

// OpenCV DNN 基于SSD实现对象检测
int OpencvDemo124() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// load a DNN model
	cv::dnn::Net net = cv::dnn::readNetFromCaffe(model_txt, model_bin);
	if (net.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

	// read an image
	cv::Mat src = cv::imread("images/dog.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	int width = src.cols;
	int height = src.rows;

	cv::Mat dst = src.clone();

	cv::Mat input = cv::dnn::blobFromImage(src, 0.007843, cv::Size(300, 300), cv::Scalar(127.5, 127.5, 127.5), true, false);
    CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("blob - width: %d, height: %d", input.size[3], input.size[2]));

    // run the model
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    // net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setInput(input, "data");
    cv::Mat output = net.forward("detection_out");

	// get detection results
	cv::Mat detMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());
	float confidence_threshold = 0.5;
	for (int i = 0; i < detMat.rows; i++) {
		float confidence = detMat.at<float>(i, 2);
		if (confidence > confidence_threshold) {
			size_t objIndex = (size_t)(detMat.at<float>(i, 1));
			float tl_x = detMat.at<float>(i, 3) * width;
			float tl_y = detMat.at<float>(i, 4) * height;
			float br_x = detMat.at<float>(i, 5) * width;
			float br_y = detMat.at<float>(i, 6) * height;

			cv::rectangle(dst, cv::Point((int)tl_x, (int)tl_y), cv::Point((int)br_x, (int)br_y), cv::Scalar(0, 0, 255), 2, cv::LINE_8);
			cv::putText(dst, cv::format("%s (confidence: %.3f)", objNames[objIndex].c_str(), confidence), 
                                        cv::Point(tl_x, tl_y - 8), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
		}
	}

	std::vector<double> timings;
	double time = net.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
	std::string text = cv::format("time: %.0f ms", time);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, text);
	cv::putText(dst, text, cv::Point(20, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("object detection - SSD model", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
