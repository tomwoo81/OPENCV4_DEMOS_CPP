#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// HOG特征描述子—提取描述子
int OpencvDemo102() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat src = cv::imread("images/gaoyy_min.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    cv::imshow("input", src);

    cv::Mat gray;

    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    cv::HOGDescriptor hog;
    std::vector<float> features;

    hog.compute(src, features, cv::Size(8, 8), cv::Size(0, 0));

    CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("number of elements in the HOG descriptor: %zu", features.size()));

    for (size_t i = 0; i < features.size(); i++) {
        CV_LOG_DEBUG(CV_LOGTAG_GLOBAL, cv::format("[%zu]: %f", i, features[i]));
    }

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
