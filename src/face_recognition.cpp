#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string model_1_bin = "models/resnet/res10_300x300_ssd_iter_140000.caffemodel";
static std::string model_1_txt = "models/resnet/deploy.prototxt";
static std::string model_2_bin = "models/face_detector/openface.nn4.small2.v1.t7";

static void register_face(cv::dnn::Net& detection_net, cv::dnn::Net& extraction_net, const cv::Mat& src, cv::Mat& dst, 
                          const std::string& label, std::vector<std::vector<float>>& face_features, std::vector<std::string>& face_labels);
static void recognize_face(cv::dnn::Net& detection_net, cv::dnn::Net& extraction_net, const cv::Mat& src, cv::Mat& dst, 
                           const std::vector<std::vector<float>>& face_features, const std::vector<std::string>& face_labels);
static void extract_face_feature(cv::dnn::Net& net, const cv::Mat& face, std::vector<float>& fv, double& time);
static float compare_by_cosine_similarity(const std::vector<float>& fv1, const std::vector<float>& fv2);

// OpenCV DNN: 人脸识别
int main(int argc, const char *argv[])
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// load DNN models
	cv::dnn::Net net1 = cv::dnn::readNetFromCaffe(model_1_txt, model_1_bin);
	if (net1.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

	cv::dnn::Net net2 = cv::dnn::readNetFromTorch(model_2_bin);
	if (net2.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

    net1.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // net1.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    net1.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    // net1.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);

    net2.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // net2.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    net2.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    // net2.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);

	// read an image
	cv::Mat src1 = cv::imread("images/greenback.png");
	if (src1.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    cv::Mat dst1;
    std::vector<std::vector<float>> face_features;
    std::vector<std::string> face_labels;

    register_face(net1, net2, src1, dst1, "anchor", face_features, face_labels);

    // read a video file
	cv::VideoCapture capture("videos/01.mp4");
	if (!capture.isOpened()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a video file!");
		return cv::Error::StsError;
	}

    cv::Mat src2;
    if (!capture.read(src2)) {
        CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not read a frame!");
        return cv::Error::StsError;
    }

    cv::Mat result;
	std::string windowTitle = "OpenCV DNN - face recognition";
    cv::namedWindow(windowTitle, cv::WINDOW_NORMAL);

    while (capture.read(src2)) {
        cv::Mat dst2;
        recognize_face(net1, net2, src2, dst2, face_features, face_labels);

        int h = src2.rows;
		int w = src2.cols;
		if (result.empty()) {
			result = cv::Mat::zeros(cv::Size(w, h * 2), src2.type());
		}
        cv::resize(dst1, dst1, src2.size());
		dst1.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
        dst2.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
        cv::putText(result, "face registration", cv::Point(10, 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
        cv::putText(result, "face recognition", cv::Point(10, h + 40), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
        cv::resizeWindow(windowTitle, cv::Size(w / 2, h));
        cv::imshow(windowTitle, result);

        char c = cv::waitKey(20);

        if (c == 27) { // ESC
			break;
		}
    }

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static void register_face(cv::dnn::Net& detection_net, cv::dnn::Net& extraction_net, const cv::Mat& src, cv::Mat& dst, 
                          const std::string& label, std::vector<std::vector<float>>& face_features, std::vector<std::string>& face_labels) {
	int width = src.cols;
	int height = src.rows;

    dst = src.clone();

    cv::Mat input = cv::dnn::blobFromImage(src, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0), false, false);

    // run face detection model
    detection_net.setInput(input, "data");
    cv::Mat output = detection_net.forward("detection_out");

    // get face detection results
    cv::Mat detMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());
    float confidence_threshold = 0.5;
    for (int i = 0; i < detMat.rows; i++) {
        float confidence = detMat.at<float>(i, 2);
        if (confidence > confidence_threshold) {
            float tl_x = detMat.at<float>(i, 3) * width;
            float tl_y = detMat.at<float>(i, 4) * height;
            float br_x = detMat.at<float>(i, 5) * width;
            float br_y = detMat.at<float>(i, 6) * height;

            cv::Rect rect(cv::Point((int)tl_x, (int)tl_y), cv::Point((int)br_x, (int)br_y));

            if (rect.width < 5 || rect.height < 5) {
                continue;
            }

            cv::rectangle(dst, rect, cv::Scalar(0, 0, 255), 2);

            cv::Mat face = src(rect);
            std::vector<float> feature;
            double time;

            // extract face feature
            extract_face_feature(extraction_net, face, feature, time);

            face_features.emplace_back(feature);
            face_labels.emplace_back(label);

            cv::putText(dst, label, cv::Point(tl_x, tl_y - 5), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 0, 0), 2);
        }
    }
}

static void recognize_face(cv::dnn::Net& detection_net, cv::dnn::Net& extraction_net, const cv::Mat& src, cv::Mat& dst, 
                           const std::vector<std::vector<float>>& face_features, const std::vector<std::string>& face_labels) {
	int width = src.cols;
	int height = src.rows;

    dst = src.clone();

    cv::Mat input = cv::dnn::blobFromImage(src, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0), false, false);

    // run face detection model
    detection_net.setInput(input, "data");
    cv::Mat output = detection_net.forward("detection_out");

    // get face detection results
    cv::Mat detMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());
    float confidence_threshold = 0.5;
    for (int i = 0; i < detMat.rows; i++) {
        float confidence = detMat.at<float>(i, 2);
        if (confidence > confidence_threshold) {
            float tl_x = detMat.at<float>(i, 3) * width;
            float tl_y = detMat.at<float>(i, 4) * height;
            float br_x = detMat.at<float>(i, 5) * width;
            float br_y = detMat.at<float>(i, 6) * height;

            cv::Rect rect(cv::Point((int)tl_x, (int)tl_y), cv::Point((int)br_x, (int)br_y));

            if (rect.width < 5 || rect.height < 5) {
                continue;
            }

            cv::rectangle(dst, rect, cv::Scalar(0, 0, 255), 2);

            cv::Mat face = src(rect);
            std::vector<float> feature;
            double time;

            // extract face feature
            extract_face_feature(extraction_net, face, feature, time);

            float min_dist = FLT_MAX;
            int index = -1;
            for (size_t i = 0; i < face_features.size(); i++) {
                float dist = compare_by_cosine_similarity(feature, face_features[i]);
                if (dist < min_dist) {
                    min_dist = dist;
                    index = i;
                }
            }

            float min_dist_threshold = 0.3;
            std::string text;
            if (index >= 0 && min_dist < min_dist_threshold) {
                text = cv::format("%s, %.3f, %.0f ms", face_labels[index].c_str(), min_dist, time);
            }
            else
            {
                text = cv::format("%.0f ms", time);
            }

            cv::putText(dst, text, cv::Point(tl_x, tl_y - 5), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 0, 0), 2);
        }
    }

    std::vector<double> timings;
    double time = detection_net.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
    std::string text = cv::format("face detection time: %.0f ms", time);
    cv::putText(dst, text, cv::Point(20, 90), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 0, 0), 2);
}

static void extract_face_feature(cv::dnn::Net& net, const cv::Mat& face, std::vector<float>& fv, double& time) {
	cv::Mat input = cv::dnn::blobFromImage(face, 1.0 / 255, cv::Size(96, 96), cv::Scalar(0, 0, 0), true, false);

	net.setInput(input);
	cv::Mat output = net.forward();

	cv::Mat fm = output.reshape(1, 1);
	for (int i = 0; i < fm.cols; i++) {
		fv.emplace_back(fm.at<float>(0, i));
	}

    std::vector<double> timings;
    time = net.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
}

static float compare_by_cosine_similarity(const std::vector<float>& fv1, const std::vector<float>& fv2) {
	// 计算余弦相似，0 ~ 1距离，距离越小越相似
	// 0表示夹角为0°，0.5表示夹角为90°，1表示夹角为180°
	float dot = 0;
	float sum1 = 0;
	float sum2 = 0;
	for (size_t i = 0; i < fv1.size(); i++) {
		dot += fv1[i] * fv2[i];
		sum1 += pow(fv1[i], 2);
		sum2 += pow(fv2[i], 2);
	}
	float norm = sqrt(sum1) * sqrt(sum2);
	float similarity = dot / norm;
	float dist = acos(similarity) / CV_PI;
	return dist;
}

/* end of file */
