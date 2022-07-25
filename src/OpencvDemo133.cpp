#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string model_bin = "models/colorization/colorization_release_v2.caffemodel";
static std::string model_txt = "models/colorization/colorization_deploy_v2.prototxt";

// 313 ab cluster centers from "pts_in_hull.npy" (already transposed)
static float pts_in_hull[] = {
	-90., -90., -90., -90., -90., -80., -80., -80., -80., -80., -80., -80., -80., -70., -70., -70., -70., -70., -70., -70., -70.,
	-70., -70., -60., -60., -60., -60., -60., -60., -60., -60., -60., -60., -60., -60., -50., -50., -50., -50., -50., -50., -50., -50.,
	-50., -50., -50., -50., -50., -50., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -40., -30.,
	-30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -30., -20., -20., -20., -20., -20., -20., -20.,
	-20., -20., -20., -20., -20., -20., -20., -20., -20., -10., -10., -10., -10., -10., -10., -10., -10., -10., -10., -10., -10., -10.,
	-10., -10., -10., -10., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 10., 10., 10., 10., 10., 10., 10.,
	10., 10., 10., 10., 10., 10., 10., 10., 10., 10., 10., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20., 20.,
	20., 20., 20., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 30., 40., 40., 40., 40.,
	40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 40., 50., 50., 50., 50., 50., 50., 50., 50., 50., 50.,
	50., 50., 50., 50., 50., 50., 50., 50., 50., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60., 60.,
	60., 60., 60., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 70., 80., 80., 80.,
	80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 80., 90., 90., 90., 90., 90., 90., 90., 90., 90., 90.,
	90., 90., 90., 90., 90., 90., 90., 90., 90., 100., 100., 100., 100., 100., 100., 100., 100., 100., 100., 50., 60., 70., 80., 90.,
	20., 30., 40., 50., 60., 70., 80., 90., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., -20., -10., 0., 10., 20., 30., 40., 50.,
	60., 70., 80., 90., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 100., -40., -30., -20., -10., 0., 10., 20.,
	30., 40., 50., 60., 70., 80., 90., 100., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 100., -50.,
	-40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., 100., -60., -50., -40., -30., -20., -10., 0., 10., 20.,
	30., 40., 50., 60., 70., 80., 90., 100., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90.,
	100., -80., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., -80., -70., -60., -50.,
	-40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., 90., -90., -80., -70., -60., -50., -40., -30., -20., -10.,
	0., 10., 20., 30., 40., 50., 60., 70., 80., 90., -100., -90., -80., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30.,
	40., 50., 60., 70., 80., 90., -100., -90., -80., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70.,
	80., -110., -100., -90., -80., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., -110., -100.,
	-90., -80., -70., -60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., 80., -110., -100., -90., -80., -70.,
	-60., -50., -40., -30., -20., -10., 0., 10., 20., 30., 40., 50., 60., 70., -110., -100., -90., -80., -70., -60., -50., -40., -30.,
	-20., -10., 0., 10., 20., 30., 40., 50., 60., 70., -90., -80., -70., -60., -50., -40., -30., -20., -10., 0.
};

// OpenCV DNN 图像彩色化模型使用
int OpencvDemo133() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// load a DNN model
	cv::dnn::Net net = cv::dnn::readNetFromCaffe(model_txt, model_bin);
	if (net.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

	// populate additional layers
	int size[] = { 2, 313, 1, 1 };
	cv::Ptr<cv::dnn::Layer> class8_ab = net.getLayer("class8_ab");
	class8_ab->blobs.push_back(cv::Mat(4, size, CV_32F, pts_in_hull));
	cv::Ptr<cv::dnn::Layer> conv8_313_rh = net.getLayer("conv8_313_rh");
	conv8_313_rh->blobs.push_back(cv::Mat(1, 313, CV_32F, cv::Scalar(2.606)));

	// read an image
	cv::Mat src = cv::imread("images/river.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	// extract L channel and subtract mean
	cv::Mat temp, srcLab, srcL, srcL2;
	src.convertTo(temp, CV_32F, 1.0 / 255);
	cv::cvtColor(temp, srcLab, cv::COLOR_BGR2Lab);
	cv::extractChannel(srcLab, srcL, 0);

	cv::Mat input = cv::dnn::blobFromImage(srcL, 1.0, cv::Size(224, 224), cv::Scalar(50));

	// run the model
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    // net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setInput(input);
    cv::Mat output = net.forward();

	// retrieve the calculated a, b channels from the network output
	cv::Size outputSize(output.size[2], output.size[3]);
	cv::Mat a = cv::Mat(outputSize, CV_32F, output.ptr(0, 0));
	cv::Mat b = cv::Mat(outputSize, CV_32F, output.ptr(0, 1));
	cv::resize(a, a, src.size());
	cv::resize(b, b, src.size());

	// merge L, a, b channels, and convert the image back to BGR space
    cv::Mat chn[] = { srcL, a, b }, dstLab, dst;
	cv::merge(chn, 3, dstLab);
	cv::cvtColor(dstLab, dst, cv::COLOR_Lab2BGR);
	dst.convertTo(dst, CV_8U, 255);

	std::vector<double> timings;
	double time = net.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
	std::string text = cv::format("time: %.0f ms", time);
	CV_LOG_INFO(CV_LOGTAG_GLOBAL, text);
	cv::putText(dst, text, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(255, 0, 0), 2);

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w, h * 3), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	cv::normalize(srcL, srcL, 0, 255, cv::NORM_MINMAX);
	cv::convertScaleAbs(srcL, srcL);
	cv::cvtColor(srcL, srcL, cv::COLOR_GRAY2BGR); // 1 channel -> 3 channels
	srcL.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(h * 2, h * 3), cv::Range(0, w)));
	cv::putText(result, "original image", cv::Point(10, 40), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "image with L component", cv::Point(10, h + 40), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "image after colourisation", cv::Point(10, h * 2 + 40), cv::FONT_ITALIC, 1.0, cv::Scalar(0, 0, 255), 2);
	std::string windowTitle = "image colourisation";
	cv::namedWindow(windowTitle, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowTitle, cv::Size(w / 2, h * 3 / 2));
	cv::imshow(windowTitle, result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
