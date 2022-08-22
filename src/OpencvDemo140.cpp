#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

extern std::vector<std::string> labels;
extern std::vector<std::vector<float>> text_features;

int train_data();
static int test_data();
std::vector<float> ExtractFeature(const cv::Mat& txtImage);
static std::string PredictDigit(const std::vector<float>& feature);

// 案例：识别0～9印刷体数字—Part 2
int OpencvDemo140() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// 训练
	if (cv::Error::StsOk != train_data()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "Fail to train data!");
		return cv::Error::StsError;
	}

	// 测试
	if (cv::Error::StsOk != test_data()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "Fail to test data!");
		return cv::Error::StsError;
	}

    cv::waitKey(0);

	return cv::Error::StsOk;
}

static int test_data() {
	cv::Mat src = cv::imread("images/digit-02.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    cv::Mat dst = src.clone();

    cv::Mat gray, binary;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());

	for (size_t i = 0; i < contours.size(); i++) {
		cv::Rect rect = cv::boundingRect(contours[i]);

        std::vector<float> feature = ExtractFeature(binary(rect));

		std::string result = PredictDigit(feature);
		cv::putText(dst, result, cv::Point(rect.tl().x + 2, rect.br().y + 15), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 0, 255), 1);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 15), cv::FONT_ITALIC, 0.5, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results", cv::Point(w + 10, 15), cv::FONT_ITALIC, 0.5, cv::Scalar(0, 0, 255), 1);
	cv::imshow("digit recognition - test data", result);

    return cv::Error::StsOk;
}

static std::string PredictDigit(const std::vector<float>& feature) {
	float minDist = FLT_MAX;
	int index = -1;

	for (size_t i = 0; i < text_features.size(); i++) {
		float dist = 0;
		std::vector<float> temp = text_features[i];

		for (size_t j = 0; j < feature.size(); j++) {
			float d = temp[j] - feature[j];
			dist += d * d;
		}

		if (dist < minDist) {
			minDist = dist;
			index = i;
		}
	}

	std::string result;

	if (index >= 0) {
		result = labels[index];
	}
	else {
		result = "-";
	}

	return std::move(result);
}

/* end of file */
