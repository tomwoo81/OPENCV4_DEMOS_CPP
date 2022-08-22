#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string face_model_bin = "models/face_detector/opencv_face_detector_uint8.pb";
static std::string face_model_txt = "models/face_detector/opencv_face_detector.pbtxt";

static std::string gender_model_bin = "models/cnn_age_gender_models/gender_net.caffemodel";
static std::string gender_model_txt = "models/cnn_age_gender_models/gender_deploy.prototxt";

static std::string age_model_bin = "models/cnn_age_gender_models/age_net.caffemodel";
static std::string age_model_txt = "models/cnn_age_gender_models/age_deploy.prototxt";

static std::string genderList[] = { "Male", "Female" };
static std::string ageList[] = { "(0-2)", "(4-6)", "(8-12)", "(15-20)", "(25-32)", "(38-43)", "(48-53)", "(60-100)" };

// OpenCV DNN 实现性别与年龄预测
int OpencvDemo137() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// load DNN models
	cv::dnn::Net faceNet = cv::dnn::readNet(face_model_bin, face_model_txt);
	if (faceNet.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model for face detection!");
		return cv::Error::StsError;
	}

	cv::dnn::Net genderNet = cv::dnn::readNet(gender_model_bin, gender_model_txt);
	if (genderNet.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model for gender prediction!");
		return cv::Error::StsError;
	}

	cv::dnn::Net ageNet = cv::dnn::readNet(age_model_bin, age_model_txt);
	if (ageNet.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model for age prediction!");
		return cv::Error::StsError;
	}

    // configure the models
    faceNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // faceNet.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    faceNet.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    // faceNet.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);

    genderNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // genderNet.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    genderNet.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    // genderNet.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);

    ageNet.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // ageNet.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    ageNet.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    // ageNet.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);

	// read an image
	cv::Mat src = cv::imread("images/persons.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	int width = src.cols;
	int height = src.rows;

	cv::Mat dst = src.clone();

	cv::Mat faceInput = cv::dnn::blobFromImage(src, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0), false, false);

    // run the model for face detection
    faceNet.setInput(faceInput, "data");
    cv::Mat faceOutput = faceNet.forward("detection_out");

	std::vector<double> timings;
	double time = faceNet.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
	std::string text = cv::format("time of face detection: %.0f ms", time);
    CV_LOG_INFO(CV_LOGTAG_GLOBAL, text);

	// get detection results
	cv::Mat detMat(faceOutput.size[2], faceOutput.size[3], CV_32F, faceOutput.ptr<float>());
	float confidence_threshold = 0.5;
    int padding = 10;
	for (int i = 0; i < detMat.rows; i++) {
		float confidence = detMat.at<float>(i, 2);
		if (confidence > confidence_threshold) {
			float tl_x = detMat.at<float>(i, 3) * width;
			float tl_y = detMat.at<float>(i, 4) * height;
			float br_x = detMat.at<float>(i, 5) * width;
			float br_y = detMat.at<float>(i, 6) * height;

			cv::rectangle(dst, cv::Point((int)tl_x, (int)tl_y), cv::Point((int)br_x, (int)br_y), cv::Scalar(0, 0, 255), 1);
			cv::putText(dst, cv::format("%.3f", confidence), 
                                        cv::Point(tl_x, tl_y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 1);

            cv::Rect roi(cv::Point(cv::max(0, (int)tl_x - padding), cv::max(0, (int)tl_y - padding)), 
                         cv::Point(cv::min((int)br_x + padding, width), cv::min((int)br_y + padding, height)));

            cv::Mat genderAgeInput = cv::dnn::blobFromImage(src(roi), 1.0, cv::Size(227, 227), cv::Scalar(78.4263377603, 87.7689143744, 114.895847746), false, false);

            // run the model for gender prediction
            genderNet.setInput(genderAgeInput);
            cv::Mat genderOutput = genderNet.forward();

            time = genderNet.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
            text = cv::format("time of gender prediction: %.0f ms", time);
            CV_LOG_INFO(CV_LOGTAG_GLOBAL, text);

            // get the class with the highest score
            cv::Mat probMat = genderOutput.reshape(1, 1);
            double confidence;
            cv::Point maxLoc;
            cv::minMaxLoc(probMat, NULL, &confidence, NULL, &maxLoc);
            int classId = maxLoc.x;
            std::string gender = genderList[classId];

            cv::putText(dst, gender, cv::Point(tl_x, br_y + 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);

            // run the model for age prediction
            ageNet.setInput(genderAgeInput);
            cv::Mat ageOutput = ageNet.forward();

            time = ageNet.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
            text = cv::format("time of age prediction: %.0f ms", time);
            CV_LOG_INFO(CV_LOGTAG_GLOBAL, text);

            // get the class with the highest score
            probMat = ageOutput.reshape(1, 1);
            cv::minMaxLoc(probMat, NULL, &confidence, NULL, &maxLoc);
            classId = maxLoc.x;
            std::string age = ageList[classId];

            cv::putText(dst, age, cv::Point(tl_x, br_y + 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
		}
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
	cv::imshow("gender & age prediction", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
