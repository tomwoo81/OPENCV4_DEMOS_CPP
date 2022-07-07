#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string bin_model = "models/googlenet/bvlc_googlenet.caffemodel";
static std::string protxt = "models/googlenet/bvlc_googlenet.prototxt";

// OpenCV DNN 获取导入模型各层信息
int OpencvDemo121() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// load a DNN model
	cv::dnn::Net net = cv::dnn::readNet(bin_model, protxt);
	if (net.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}
    CV_LOG_INFO(CV_LOGTAG_GLOBAL, "Successfully loaded a DNN model.");

	// get info of layers
	std::vector<std::string> layer_names = net.getLayerNames();
	for (const auto& name: layer_names) {
		int id = net.getLayerId(name);
		auto pLayer = net.getLayer(id);
		CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("layer id: %d, layer type: %s, layer name: %s", id, pLayer->type.c_str(), pLayer->name.c_str()));
	}

	return cv::Error::StsOk;
}

/* end of file */
