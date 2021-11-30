#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

// BRISK特征提取与描述子匹配
int OpencvDemo107() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat box = cv::imread("images/box.png");
	cv::Mat box_in_scene = cv::imread("images/box_in_scene.png");
	if (box.empty() || box_in_scene.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load image(s)!");
		return cv::Error::StsError;
	}
	cv::imshow("box", box);
	cv::imshow("box in scene", box_in_scene);

    // 检测BRISK关键点和提取描述子
	auto brisk = cv::BRISK::create();

	std::vector<cv::KeyPoint> kps_box, kps_bis;
    cv::Mat descs_box, descs_bis;
	brisk->detectAndCompute(box, cv::Mat(), kps_box, descs_box);
    brisk->detectAndCompute(box_in_scene, cv::Mat(), kps_bis, descs_bis);

	// 描述子匹配 — 暴力匹配
    auto bf = cv::BFMatcher::create();

	std::vector<cv::DMatch> matches;
	bf->match(descs_box, descs_bis, matches);

    // 绘制匹配点对
	cv::Mat img_matches;
	cv::drawMatches(box, kps_box, box_in_scene, kps_bis, matches, img_matches);
	cv::imshow("BRISK descriptors matching", img_matches);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
