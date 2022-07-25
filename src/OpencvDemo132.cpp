#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string model_bin = "models/googlenet/bvlc_googlenet.caffemodel";
static std::string model_txt = "models/googlenet/bvlc_googlenet.prototxt";
static std::string labels_txt = "models/googlenet/classification_classes_ILSVRC2012.txt";

static std::vector<std::string> readClassNames(const std::string& filename);

// OpenCV DNN 单张与多张图像的推断
int OpencvDemo132() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// load a DNN model
	cv::dnn::Net net = cv::dnn::readNetFromCaffe(model_txt, model_bin);
	if (net.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

	// load labels
    std::vector<std::string> labels = readClassNames(labels_txt);
	if (labels.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load labels!");
		return cv::Error::StsError;
	}

	// read image(s)
	cv::Mat src1 = cv::imread("images/cat.jpg");
    cv::Mat src2 = cv::imread("images/aeroplane.jpg");
	if (src1.empty() || src2.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load image(s)!");
		return cv::Error::StsError;
	}
	std::vector<cv::Mat> srcs;
	srcs.push_back(src1);
	srcs.push_back(src2);

	cv::Mat dst1 = src1.clone();
    cv::Mat dst2 = src2.clone();
	std::vector<cv::Mat> dsts;
	dsts.push_back(dst1);
	dsts.push_back(dst2);

	cv::Mat input = cv::dnn::blobFromImages(srcs, 1.0, cv::Size(224, 224), cv::Scalar(104, 117, 123), false, false);

	// run the model
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    // net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setInput(input);
    cv::Mat output;
    output = net.forward();

    for (int i = 0; i < output.rows; i++) {
        // get the class with the highest score for each image
        cv::Mat probMat = output(cv::Rect(0, i, 1000, 1)).clone();
        probMat = probMat.reshape(1, 1);
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
        cv::putText(dsts[i], text, cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(0, 0, 255), 1);
        cv::imshow(cv::format("image classification - GoogLeNet model - %d", i + 1), dsts[i]);
	}

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
