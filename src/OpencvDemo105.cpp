#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string test_dir = "images/train_data/elec_watch/test/";
static std::string model_filename = "models/svm_elec_watch.yml";

static void get_hog_descripor(const cv::Mat& image, std::vector<float>& desc);
static void svm_predict();

// HOG特征描述子—使用HOG进行对象检测
int OpencvDemo105() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

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

static void svm_predict() {
	cv::Ptr<cv::ml::SVM> pSVM = cv::ml::SVM::load(model_filename);

	std::vector<std::string> test_image_filenames;

	cv::glob(test_dir, test_image_filenames);

	size_t num_test_images = test_image_filenames.size();
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("number of test images: %zu", num_test_images));

    for (size_t i = 0; i < num_test_images; i++) {
        cv::Mat src = cv::imread(test_image_filenames[i]);

        cv::Mat img;

        if ((src.rows >= 1080) || (src.cols >= 1920)) {
            cv::resize(src, img, cv::Size(0, 0), 0.20, 0.20);
        }
        else {
            img = src.clone();
        }

        cv::Mat dst = img.clone();

        cv::Rect window;

        window.width = 64;
        window.height = 128;

        int sum_x = 0, sum_y = 0, count = 0;

        for (int row = 128; row <= img.rows; row += 4) {
            for (int col = 64; col <= img.cols; col += 4) {
                window.x = col - 64;
                window.y = row - 128;

                std::vector<float> fv;
                get_hog_descripor(img(window), fv);

                float result = pSVM->predict(fv);

                if (result > 0) {
                    // cv::rectangle(dst, window, cv::Scalar(255, 0, 0), 1, cv::LINE_8, 0);
                    sum_x += window.x;
                    sum_y += window.y;
                    count++;
                }
            }
        }

        if (count > 0) {
            window.x = sum_x / count;
            window.y = sum_y / count;
            cv::rectangle(dst, window, cv::Scalar(0, 0, 255), 1, cv::LINE_8, 0);
        }

        cv::putText(dst, cv::format("object detected: %s", count ? "yes" : "no"), cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
        cv::imshow("SVM prediction", dst);

        cv::waitKey(0);
    }
}

/* end of file */
