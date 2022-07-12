#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string model_bin = "models/face_detector/opencv_face_detector_uint8.pb";
static std::string model_txt = "models/face_detector/opencv_face_detector.pbtxt";

// OpenCV DNN 基于残差网络的人脸检测
int OpencvDemo126() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// load a DNN model
	cv::dnn::Net net = cv::dnn::readNetFromTensorflow(model_bin, model_txt);
	if (net.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

	// read an image
	cv::Mat src = cv::imread("images/persons.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	int width = src.cols;
	int height = src.rows;

	cv::Mat dst = src.clone();

	cv::Mat input = cv::dnn::blobFromImage(src, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0), false, false);

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

			cv::rectangle(dst, cv::Point((int)tl_x, (int)tl_y), cv::Point((int)br_x, (int)br_y), cv::Scalar(0, 0, 255), 1, cv::LINE_8);
			cv::putText(dst, cv::format("%.3f", confidence), 
                                        cv::Point(tl_x, tl_y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 1);
		}
	}

	std::vector<double> timings;
	double time = net.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
	std::string text = cv::format("time: %.0f ms", time);
	cv::putText(dst, text, cv::Point(20, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 1);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("face detection - residual SSD model", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
