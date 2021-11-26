#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string positive_dir = "images/train_data/elec_watch/positive/";
static std::string negative_dir = "images/train_data/elec_watch/negative/";
static std::string test_filename = "images/train_data/elec_watch/test/test_01.png";
static std::string model_filename = "models/svm_elec_watch.yml";

static void get_hog_descripor(const cv::Mat& image, std::vector<float>& desc);
static void generate_dataset(cv::Mat& trainData, cv::Mat& labels);
static void svm_train(const cv::Mat& trainData, const cv::Mat& labels);
static void svm_predict();

// SVM线性分类器
int OpencvDemo104() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat trainData, labels;

	generate_dataset(trainData, labels);

    svm_train(trainData, labels);

	svm_predict();

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

		std::vector<float> fv;
		get_hog_descripor(image, fv);

		for (size_t j = 0; j < fv.size(); j++) {
			trainData.at<float>(i, j) = fv[j];
		}
		labels.at<int>(i, 0) = 1;
	}

	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("number of images as negative samples: %zu", num_negative_images));
	for (size_t i = 0; i < num_negative_images; i++) {
		cv::Mat image = cv::imread(negative_image_filenames[i]);

		std::vector<float> fv;
		get_hog_descripor(image, fv);

		for (size_t j = 0; j < fv.size(); j++) {
			trainData.at<float>(num_positive_images + i, j) = fv[j];
		}
		labels.at<int>(num_positive_images + i, 0) = -1;
	}
}

static void svm_train(const cv::Mat& trainData, const cv::Mat& labels) {
    cv::Ptr<cv::ml::SVM> pSVM = cv::ml::SVM::create();

	// Default values to train SVM
    pSVM->setKernel(cv::ml::SVM::LINEAR);
    pSVM->setType(cv::ml::SVM::C_SVC);
    pSVM->setC(2.67);
    pSVM->setGamma(5.383);

    CV_LOG_INFO(CV_LOGTAG_GLOBAL, "SVM training starts...");
	pSVM->train(trainData, cv::ml::ROW_SAMPLE, labels);
    CV_LOG_INFO(CV_LOGTAG_GLOBAL, "SVM training done.");

	// Save SVM model
    pSVM->save(model_filename);
}

static void svm_predict() {
	cv::Ptr<cv::ml::SVM> pSVM = cv::ml::SVM::load(model_filename);

	cv::Mat src = cv::imread(test_filename);

	std::vector<float> fv;
	get_hog_descripor(src, fv);

	float result = pSVM->predict(fv);

	cv::Mat dst = src.clone();

	cv::putText(dst, cv::format("prediction result: %.3f", result), cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("SVM prediction", dst);

	cv::waitKey(0);
}

/* end of file */
