#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

std::string positive_dir = "images/train_data/elec_watch/positive/";
std::string negative_dir = "images/train_data/elec_watch/negative/";

static void get_hog_descripor(const cv::Mat& image, std::vector<float>& desc);
static void generate_dataset(cv::Mat& trainData, cv::Mat& labels);

// HOG特征描述子—使用描述子特征生成样本数据
int OpencvDemo103() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat trainData, labels;

	generate_dataset(trainData, labels);

	return cv::Error::StsOk;
}

static void get_hog_descripor(const cv::Mat& image, std::vector<float>& desc) {
	int h = image.rows;
	int w = image.cols;

	float rate = 64.0 / w;

    cv::Mat img, gray;

	cv::resize(image, img, cv::Size(64, h * rate));
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

	cv::Mat result(cv::Size(64, 128), CV_8U, cv::Scalar(127));
	cv::Rect roi(0, (128 - gray.rows) / 2, 64, gray.rows);

	gray.copyTo(result(roi));

	cv::HOGDescriptor hog;

	hog.compute(result, desc, cv::Size(8, 8), cv::Size(0, 0));
}

static void generate_dataset(cv::Mat& trainData, cv::Mat& labels) {
	std::vector<std::string> positive_image_filenames, negative_image_filenames;

	cv::glob(positive_dir, positive_image_filenames);
	cv::glob(negative_dir, negative_image_filenames);

	size_t num_positive_images = positive_image_filenames.size(), 
		   num_negative_images = negative_image_filenames.size(), 
		   num_images = num_positive_images + num_negative_images;
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("number of images: %zu", num_images));

	trainData.create(cv::Size(3780, num_images), CV_32F);
	labels.create(cv::Size(1, num_images), CV_32S);

	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("number of images as positive samples: %zu", num_positive_images));
	for (size_t i = 0; i < num_positive_images; i++) {
		cv::Mat image = cv::imread(positive_image_filenames[i]);
		cv::imshow("image as positive sample", image);
		cv::waitKey(0);

		std::vector<float> fv;
		get_hog_descripor(image, fv);
		CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("image path: %s, feature data length: %zu", positive_image_filenames[i].c_str(), fv.size()));

		for (size_t j = 0; j < fv.size(); j++) {
			trainData.at<float>(i, j) = fv[j];
		}
		labels.at<int>(i, 0) = 1;
	}

	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("number of images as negative samples: %zu", num_negative_images));
	for (size_t i = 0; i < num_negative_images; i++) {
		cv::Mat image = cv::imread(negative_image_filenames[i]);
		cv::imshow("image as negative sample", image);
		cv::waitKey(0);

		std::vector<float> fv;
		get_hog_descripor(image, fv);
		CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("image path: %s, feature data length: %zu", negative_image_filenames[i].c_str(), fv.size()));

		for (size_t j = 0; j < fv.size(); j++) {
			trainData.at<float>(num_positive_images + i, j) = fv[j];
		}
		labels.at<int>(num_positive_images + i, 0) = -1;
	}
}

/* end of file */
