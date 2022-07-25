#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string tf_pb_file = "models/inception5h/tensorflow_inception_graph.pb";
static std::string labels_txt_file = "models/inception5h/imagenet_comp_graph_label_strings.txt";

static std::vector<std::string> readClassNames(const std::string& filename);

// OpenCV DNN 实现图像分类
int OpencvDemo122() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// load a DNN model
	cv::dnn::Net net = cv::dnn::readNetFromTensorflow(tf_pb_file);
	if (net.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

	// load labels
    std::vector<std::string> labels = readClassNames(labels_txt_file);
	if (labels.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load labels!");
		return cv::Error::StsError;
	}

	// read an image
	cv::Mat src = cv::imread("images/space_shuttle.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	cv::Mat dst = src.clone();

	cv::Mat input = cv::dnn::blobFromImage(src, 1.0f, cv::Size(224, 224), cv::Scalar(), true, false);
	input -= 117.0; // mean value

    // run the model
    net.setInput(input, "input");
    cv::Mat output = net.forward("softmax2");

	// get the class with the highest score
	cv::Mat probMat = output.reshape(1, 1);
	double confidence;
	cv::Point maxLoc;
	cv::minMaxLoc(probMat, NULL, &confidence, NULL, &maxLoc);
	int classId = maxLoc.x;
	std::vector<double> timings;
	double time = net.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
	std::string text = cv::format("class: %s, confidence: %.3f, time: %.0f ms", 
					labels.at(classId).c_str(), confidence, time);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, text);

	// display the result
	cv::putText(dst, text, cv::Point(20, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::imshow("image classification - inception model", dst);

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

/* end of file */
