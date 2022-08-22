#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string base_dir = "models/fast_style/";
static std::string models_bin[9] = {
"composition_vii.t7", 
"starry_night.t7", 
"la_muse.t7", 
"the_wave.t7", 
"mosaic.t7", 
"the_scream.t7", 
"feathers.t7", 
"candy.t7", 
"udnie.t7"
};

static int RunStyleTransferModel(const std::string& modelBin, const cv::Mat& src, cv::Mat& dst);

// OpenCV DNN 实时快速的图像风格迁移
int OpencvDemo135() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// read an image
	cv::Mat src = cv::imread("images/lena.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	std::vector<cv::Mat> dsts;

	for (int i = 0; i < 9; i++) {
		cv::Mat dst;

		// run style transfer model
		if (cv::Error::StsOk != RunStyleTransferModel(base_dir + models_bin[i], src, dst)) {
			CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "Fail to run style transfer model!");
			return cv::Error::StsError;
		}

		dsts.emplace_back(dst);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 5, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dsts[0].copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	dsts[1].copyTo(result(cv::Range(0, h), cv::Range(w * 2, w * 3)));
	dsts[2].copyTo(result(cv::Range(0, h), cv::Range(w * 3, w * 4)));
	dsts[3].copyTo(result(cv::Range(0, h), cv::Range(w * 4, w * 5)));
	dsts[4].copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	dsts[5].copyTo(result(cv::Range(h, h * 2), cv::Range(w, w * 2)));
	dsts[6].copyTo(result(cv::Range(h, h * 2), cv::Range(w * 2, w * 3)));
	dsts[7].copyTo(result(cv::Range(h, h * 2), cv::Range(w * 3, w * 4)));
	dsts[8].copyTo(result(cv::Range(h, h * 2), cv::Range(w * 4, w * 5)));
	cv::putText(result, "original", cv::Point(10, 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "composition vii", cv::Point(w + 10, 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "starry night", cv::Point(w * 2 + 10, 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "la muse", cv::Point(w * 3 + 10, 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "the wave", cv::Point(w * 4 + 10, 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "mosaic", cv::Point(10, h + 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "the scream", cv::Point(w + 10, h + 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "feathers", cv::Point(w * 2 + 10, h + 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "candy", cv::Point(w * 3 + 10, h + 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "udnie", cv::Point(w * 4 + 10, h + 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	std::string windowTitle = "fast style transfer - DCGAN models";
	cv::namedWindow(windowTitle, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowTitle, cv::Size(w * 5 / 2, h));
	cv::imshow(windowTitle, result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static int RunStyleTransferModel(const std::string& modelBin, const cv::Mat& src, cv::Mat& dst) {
	// load a DNN model
	cv::dnn::Net net = cv::dnn::readNetFromTorch(modelBin);
	if (net.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

	cv::Mat input = cv::dnn::blobFromImage(src, 1.0, cv::Size(256, 256), cv::Scalar(103.939, 116.779, 123.68), false, false);

	// run the model
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    // net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setInput(input);
    cv::Mat output = net.forward();

    // get style transfer results
	int chns = output.size[1];
	int rows = output.size[2];
	int cols = output.size[3];

    const float *data = output.ptr<float>();
	dst = cv::Mat(rows, cols, CV_32FC3);
	for (int h = 0; h < chns; h++) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
                dst.at<cv::Vec3f>(i, j)[h] = *data++;
			}
		}
	}

    cv::add(dst, cv::Scalar(103.939, 116.779, 123.68), dst);
	cv::convertScaleAbs(dst, dst);

    cv::medianBlur(dst, dst, 5);
    cv::resize(dst, dst, src.size());

    std::vector<double> timings;
	double time = net.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
	std::string text = cv::format("time: %.0f ms", time);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("model: %s, ", modelBin.c_str()) + text);
	cv::putText(dst, text, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.4, cv::Scalar(0, 255, 0), 2);

	return cv::Error::StsOk;
}

/* end of file */
