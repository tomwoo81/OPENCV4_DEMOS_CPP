#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string model_bin = "models/faster_rcnn_resnet50_coco/frozen_inference_graph.pb";
static std::string model_txt = "models/faster_rcnn_resnet50_coco/graph.pbtxt";
static std::string labels_txt = "models/faster_rcnn_resnet50_coco/mscoco_label_map.pbtxt";

static std::map<int, std::string> readLabelMap(const std::string& filename);

// OpenCV DNN 直接调用TensorFlow的导出模型
int OpencvDemo128() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// load a DNN model
	cv::dnn::Net net = cv::dnn::readNetFromTensorflow(model_bin, model_txt);
	if (net.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

	// load labels
    std::map<int, std::string> labelMap = readLabelMap(labels_txt);
	if (labelMap.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load labels!");
		return cv::Error::StsError;
	}

	// read an image
	cv::Mat src = cv::imread("images/objects.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	int width = src.cols;
	int height = src.rows;

	cv::Mat dst = src.clone();

	cv::Mat input = cv::dnn::blobFromImage(src, 1.0, cv::Size(300, 300), cv::Scalar(), true, false);

    // run the model
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    // net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setInput(input);
    cv::Mat output = net.forward();

	// get detection results
	cv::Mat detMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());
	float confidence_threshold = 0.4;
	for (int i = 0; i < detMat.rows; i++) {
		float confidence = detMat.at<float>(i, 2);
		if (confidence > confidence_threshold) {
			int objId = detMat.at<float>(i, 1) + 1;
			int left = detMat.at<float>(i, 3) * width;
			int top = detMat.at<float>(i, 4) * height;
			int right = detMat.at<float>(i, 5) * width;
			int bottom = detMat.at<float>(i, 6) * height;

			cv::rectangle(dst, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(23, 230, 210), 2);
            auto it = labelMap.find(objId);
            std::string label = it->second;
            CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("ID: %d, label: %s, confidence: %.3f", objId, label.c_str(), confidence));
			cv::putText(dst, cv::format("%s (%.3f)", label.c_str(), confidence), 
                                        cv::Point(left, top - 8), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 0, 255), 1);
		}
	}

	std::vector<double> timings;
	double time = net.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
	std::string text = cv::format("time: %.0f ms", time);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, text);
	cv::putText(dst, text, cv::Point(20, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 1);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("object detection - Faster-RCNN model via TensorFlow Object Detection API", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static std::map<int, std::string> readLabelMap(const std::string& filename)
{
	std::map<int, std::string> labelMap;

	std::ifstream f(filename);
	if (!f.is_open())
	{
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not open a file!");
		return std::move(labelMap);
	}

	std::string one_line;
	std::string display_name;

	while (!f.eof())
	{
		std::getline(f, one_line);
		std::size_t found = one_line.find("id:");

		if (found != std::string::npos) {
			int index = found + 4;
			std::string id = one_line.substr(index);

			std::getline(f, display_name);
			std::size_t found = display_name.find("display_name:");

			index = found + 15;
			std::string name = display_name.substr(index);
			name = name.replace(name.length() - 1, name.length(), "");
			labelMap[stoi(id)] = name;
		}
	}

	f.close();

	return std::move(labelMap);
}

/* end of file */
