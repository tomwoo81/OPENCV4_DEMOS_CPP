#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

#define RATIO 0.22

// SIFT特征提取—描述子生成
int OpencvDemo099() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat box = cv::imread("images/box.png");
	cv::Mat box_in_scene = cv::imread("images/box_in_scene.png");
	if (box.empty() || box_in_scene.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load image(s)!");
		return cv::Error::StsError;
	}
	cv::imshow("box", box);
	cv::imshow("box in scene", box_in_scene);

    // 检测SIFT关键点和提取描述子
	auto sift = cv::SIFT::create();

	std::vector<cv::KeyPoint> kps_box, kps_bis;
    cv::Mat descs_box, descs_bis;
	sift->detectAndCompute(box, cv::Mat(), kps_box, descs_box);
    sift->detectAndCompute(box_in_scene, cv::Mat(), kps_bis, descs_bis);

	// 描述子匹配 — FLANN匹配
    auto flann = cv::FlannBasedMatcher::create();

    std::vector<cv::DMatch> matches;
	flann->match(descs_box, descs_bis, matches);

    // 筛选较佳匹配点对
	float distMax = 0;

	for (size_t i = 0; i < matches.size(); ++i) {
		distMax = cv::max(distMax, matches[i].distance);
	}

    std::vector<cv::DMatch> goodMatches;

    for (size_t i = 0; i < matches.size(); ++i) {
		if (matches[i].distance < distMax * RATIO)
			goodMatches.push_back(matches[i]);
	}

    CV_LOG_INFO(CV_LOGTAG_GLOBAL, cv::format("total number of matches: %zu, number of good matches: %zu", matches.size(), goodMatches.size()));

    // 绘制匹配点对
	cv::Mat img_matches;
	cv::drawMatches(box, kps_box, box_in_scene, kps_bis, goodMatches, img_matches);
	cv::imshow("SIFT descriptors matching", img_matches);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
