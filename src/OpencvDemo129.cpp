#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static std::string model_coco_bin = "models/openpose/coco/pose_iter_440000.caffemodel";
static std::string model_coco_txt = "models/openpose/coco/openpose_pose_coco.prototxt";

static std::string model_mpi_bin = "models/openpose/mpi/pose_iter_160000.caffemodel";
static std::string model_mpi_txt = "models/openpose/mpi/openpose_pose_mpi_faster_4_stages.prototxt";

static std::string model_hand_bin = "models/openpose/hand/pose_iter_102000.caffemodel";
static std::string model_hand_txt = "models/openpose/hand/pose_deploy.prototxt";

static const int POSE_PAIRS[3][20][2] = {
	{   // COCO body model
		{ 1,2 },{ 1,5 },{ 2,3 },
		{ 3,4 },{ 5,6 },{ 6,7 },
		{ 1,8 },{ 8,9 },{ 9,10 },
		{ 1,11 },{ 11,12 },{ 12,13 },
		{ 1,0 },{ 0,14 },
		{ 14,16 },{ 0,15 },{ 15,17 }
	},
	{   // MPI body model
		{ 0,1 },{ 1,2 },{ 2,3 },
		{ 3,4 },{ 1,5 },{ 5,6 },
		{ 6,7 },{ 1,14 },{ 14,8 },{ 8,9 },
		{ 9,10 },{ 14,11 },{ 11,12 },{ 12,13 }
	},
	{   // hand model
		{ 0,1 },{ 1,2 },{ 2,3 },{ 3,4 },         // thumb
		{ 0,5 },{ 5,6 },{ 6,7 },{ 7,8 },         // index
		{ 0,9 },{ 9,10 },{ 10,11 },{ 11,12 },    // middle
		{ 0,13 },{ 13,14 },{ 14,15 },{ 15,16 },  // ring
		{ 0,17 },{ 17,18 },{ 18,19 },{ 19,20 }   // pinkie
	} };

static int RunOpenPoseModel(const std::string& modelTxt, const std::string& modelBin, 
                            const std::string& imageFilename, cv::Mat& dst);

// OpenCV DNN 调用OpenPose模型实现姿态评估
int OpencvDemo129() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

    cv::Mat dst_coco, dst_mpi, dst_hand;

    // run COCO body OpenPose model
    if (cv::Error::StsOk != RunOpenPoseModel(model_coco_txt, model_coco_bin, "images/green.jpg", dst_coco)) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "Fail to run COCO body OpenPose model!");
		return cv::Error::StsError;
    }

    // run MPI body OpenPose model
    if (cv::Error::StsOk != RunOpenPoseModel(model_mpi_txt, model_mpi_bin, "images/green.jpg", dst_mpi)) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "Fail to run MPI body OpenPose model!");
		return cv::Error::StsError;
    }

    // run hand OpenPose model
    if (cv::Error::StsOk != RunOpenPoseModel(model_hand_txt, model_hand_bin, "images/hand.jpg", dst_hand)) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "Fail to run hand OpenPose model!");
		return cv::Error::StsError;
    }

    if (dst_coco.cols != dst_mpi.cols || dst_coco.rows != dst_mpi.rows) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "The size of 2 images with results is NOT equal!");
		return cv::Error::StsError;
    }

	int h = dst_coco.rows;
	int w = dst_coco.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), dst_coco.type());
	dst_coco.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst_mpi.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "image with results (COCO body model)", cv::Point(10, 30), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	cv::putText(result, "image with results (MPI body model)", cv::Point(w + 10, 30), cv::FONT_ITALIC, 1.2, cv::Scalar(0, 0, 255), 2);
	std::string windowTitle = "body pose estimation - OpenPose models";
	cv::namedWindow(windowTitle, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowTitle, cv::Size(w / 2, h / 4));
	cv::imshow(windowTitle, result);

    cv::putText(dst_hand, "image with results (hand model)", cv::Point(10, 30), cv::FONT_ITALIC, 0.6, cv::Scalar(0, 0, 255), 1);
    cv::imshow("hand pose estimation - OpenPose model", dst_hand);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

static int RunOpenPoseModel(const std::string& modelTxt, const std::string& modelBin, 
                            const std::string& imageFilename, cv::Mat& dst) {
	// load a DNN model
	cv::dnn::Net net = cv::dnn::readNetFromCaffe(modelTxt, modelBin);
	if (net.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load a DNN model!");
		return cv::Error::StsError;
	}

	// read an image
	cv::Mat src = cv::imread(imageFilename);
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

	int width = src.cols;
	int height = src.rows;

	dst = src.clone();

	cv::Mat input = cv::dnn::blobFromImage(src, 1.0 / 255, cv::Size(368, 368), cv::Scalar(0, 0, 0), false, false);

    // run the model
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    // net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA_FP16);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    // net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setInput(input);
    cv::Mat output = net.forward();

	int midx, npairs;
	int nparts = output.size[1];
	int h_output = output.size[2];
	int w_output = output.size[3];

	// find out which model is used
	if (nparts == 19)
	{   // COCO body model
		midx = 0;
		npairs = 17;
		nparts = 18; // skip background
	}
	else if (nparts == 16)
	{   // MPI body model
		midx = 1;
		npairs = 14;
	}
	else if (nparts == 22)
	{   // hand model
		midx = 2;
		npairs = 20;
	}
	else
	{
        CV_LOG_ERROR(CV_LOGTAG_GLOBAL, cv::format("There should be 19 parts for the COCO model, 16 for MPI, or 22 for the hand one, but this model has %d parts!", nparts));
        return cv::Error::StsError;
	}

	// find the position of the body/hand parts
	std::vector<cv::Point> points(22);
    float thresh = 0.1;
    for (int i = 0; i < nparts; i++)
	{
		// slice heatmap of corresponding body's/hand's part
        cv::Mat heatMap(h_output, w_output, CV_32F, output.ptr(0, i));

		// 1 maximum point per heatmap
		cv::Point p(-1, -1), pm;
		double conf;

		cv::minMaxLoc(heatMap, 0, &conf, 0, &pm);
		if (conf > thresh)
			p = pm;
		points[i] = p;
	}

	// draw lines to connect body/hand parts
	float sx = float(width) / w_output;
	float sy = float(height) / h_output;
	for (int i = 0; i < npairs; i++)
	{
		// lookup 2 connected body/hand parts
		cv::Point2f a = points[POSE_PAIRS[midx][i][0]];
		cv::Point2f b = points[POSE_PAIRS[midx][i][1]];

		// we did not find enough confidence before
		if (a.x <= 0 || a.y <= 0 || b.x <= 0 || b.y <= 0)
			continue;

		// scale to image size
		a.x *= sx; a.y *= sy;
		b.x *= sx; b.y *= sy;

		cv::circle(dst, a, 3, cv::Scalar(0, 0, 200), cv::FILLED);
		cv::circle(dst, b, 3, cv::Scalar(0, 0, 200), cv::FILLED);
		cv::line(dst, a, b, cv::Scalar(0, 200, 0), 2);
	}

	std::vector<double> timings;
	double time = net.getPerfProfile(timings) * 1000.0 / cv::getTickFrequency();
	std::string text = cv::format("time: %.0f ms", time);
	cv::putText(dst, text, cv::Point(20, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(200, 0, 0), 2);

    return cv::Error::StsOk;
}

/* end of file */
