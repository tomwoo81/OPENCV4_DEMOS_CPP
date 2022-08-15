#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string model_bin = "models/yolov3/yolov3.weights";
static std::string model_txt = "models/yolov3/yolov3.cfg";
static std::string labels_txt = "models/yolov3/object_detection_classes_yolov3.txt";

static std::vector<std::string> readClassNames(const std::string& filename);
static void RunYolov3Model(cv::dnn::Net& net, const int targetId, const int backendId, 
						   const std::vector<std::string>& unconnectedOutLayersNames, 
						   const std::vector<std::string>& labels, const cv::Mat& src, cv::Mat& dst);

// OpenCV DNN 使用OpenVINO加速
int OpencvDemo138() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// load a DNN model
	cv::dnn::Net net = cv::dnn::readNetFromDarknet(model_txt, model_bin);
	if (net.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

	// get names of unconnected out layers
	std::vector<std::string> outNames = net.getUnconnectedOutLayersNames();

	// load labels
    std::vector<std::string> labels = readClassNames(labels_txt);
	if (labels.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load labels!");
		return cv::Error::StsError;
	}

	// read an image
	cv::Mat src = cv::imread("images/pedestrian.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::resize(src, src, cv::Size(src.cols * 0.8, src.rows * 0.8));

	cv::Mat dst1, dst2, dst3, dst4;

	RunYolov3Model(net, cv::dnn::DNN_TARGET_CPU, cv::dnn::DNN_BACKEND_OPENCV, outNames, labels, src, dst1);
	RunYolov3Model(net, cv::dnn::DNN_TARGET_CPU, cv::dnn::DNN_BACKEND_INFERENCE_ENGINE, outNames, labels, src, dst2);
	RunYolov3Model(net, cv::dnn::DNN_TARGET_CUDA, cv::dnn::DNN_BACKEND_CUDA, outNames, labels, src, dst3);
	RunYolov3Model(net, cv::dnn::DNN_TARGET_CUDA_FP16, cv::dnn::DNN_BACKEND_CUDA, outNames, labels, src, dst4);

	RunYolov3Model(net, cv::dnn::DNN_TARGET_CPU, cv::dnn::DNN_BACKEND_OPENCV, outNames, labels, src, dst1);
	RunYolov3Model(net, cv::dnn::DNN_TARGET_CPU, cv::dnn::DNN_BACKEND_INFERENCE_ENGINE, outNames, labels, src, dst2);
	RunYolov3Model(net, cv::dnn::DNN_TARGET_CUDA, cv::dnn::DNN_BACKEND_CUDA, outNames, labels, src, dst3);
	RunYolov3Model(net, cv::dnn::DNN_TARGET_CUDA_FP16, cv::dnn::DNN_BACKEND_CUDA, outNames, labels, src, dst4);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h * 2), src.type());
	dst1.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
    dst2.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	dst3.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
    dst4.copyTo(result(cv::Range(h, h * 2), cv::Range(w, w * 2)));
	cv::putText(result, "target: CPU, backend: OpenCV", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "target: CPU, backend: OpenVINO", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "target: GPU, backend: CUDA", cv::Point(10, h + 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
    cv::putText(result, "target: GPU (FP16), backend: CUDA", cv::Point(w + 10, h + 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("computational targets & backends of DNN module", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static std::vector<std::string> readClassNames(const std::string& filename)
{
	std::vector<std::string> classNames;

	std::ifstream f(filename);
	if (!f.is_open())
	{
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not open a file!");
		return std::move(classNames);
	}

	std::string name;

	while (!f.eof())
	{
		std::getline(f, name);
		if (name.length() > 0)
		{
			classNames.push_back(name);
		}
	}

	f.close();

	return std::move(classNames);
}

static void RunYolov3Model(cv::dnn::Net& net, const int targetId, const int backendId, 
						   const std::vector<std::string>& unconnectedOutLayersNames, 
						   const std::vector<std::string>& labels, const cv::Mat& src, cv::Mat& dst)
{
	int width = src.cols;
	int height = src.rows;

	dst = src.clone();

	cv::Mat input = cv::dnn::blobFromImage(src, 1 / 255.f, cv::Size(416, 416), cv::Scalar(), true, false);

	// run the model
    net.setPreferableTarget(targetId);
    net.setPreferableBackend(backendId);
    net.setInput(input);
	std::vector<cv::Mat> outputs;
    net.forward(outputs, unconnectedOutLayersNames);

	// get detection results
	std::vector<cv::Rect> boxes;
	std::vector<int> classIds;
	std::vector<float> confidences;
	float confidence_threshold = 0.5;

	for (size_t i = 0; i < outputs.size(); ++i)
	{
		// Network produces output blob with a shape NxC where N is a number of
		// detected objects and C is a number of classes + 4 where the first 4
		// numbers are [center_x, center_y, width, height]
		float* data = (float*)outputs[i].data;
		for (int j = 0; j < outputs[i].rows; ++j, data += outputs[i].cols)
		{
			cv::Mat scores = outputs[i].row(j).colRange(5, outputs[i].cols);
			cv::Point classIdPoint;
			double confidence;
			cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > confidence_threshold)
			{
				int centerX = (int)(data[0] * width);
				int centerY = (int)(data[1] * height);
				int boxWidth = (int)(data[2] * width);
				int boxHeight = (int)(data[3] * height);
				int left = centerX - boxWidth / 2;
				int top = centerY - boxHeight / 2;

				boxes.push_back(cv::Rect(left, top, boxWidth, boxHeight));
				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
			}
		}
	}

	// perform non maximum suppression
	std::vector<int> indices;
	cv::dnn::NMSBoxes(boxes, confidences, confidence_threshold, 0.2, indices);

	for (size_t i = 0; i < indices.size(); ++i)
	{
		int index = indices[i];
		cv::Rect box = boxes[index];
		std::string label = labels[classIds[index]];
		float confidence = confidences[index];
		cv::rectangle(dst, box, cv::Scalar(0, 0, 255), 1);
		cv::putText(dst, cv::format("%s (%.3f)", label.c_str(), confidence), 
									box.tl(), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 0, 0), 1);
	}

	std::vector<double> timings;
	double time = net.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
	std::string text = cv::format("time: %.0f ms", time);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, text);
	cv::putText(dst, text, cv::Point(20, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 1);
}

/* end of file */
