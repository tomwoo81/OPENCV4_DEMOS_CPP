#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

#define RATIO 0.4

// 基于描述子匹配的已知对象定位
int OpencvDemo097() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	cv::Mat box = cv::imread("images/box.png");
	cv::Mat box_in_scene = cv::imread("images/box_in_scene.png");
	if (box.empty() || box_in_scene.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load image(s)!");
		return cv::Error::StsError;
	}
	cv::imshow("box", box);
	cv::imshow("box in scene", box_in_scene);

    // 检测ORB关键点和提取描述子
	auto orb = cv::ORB::create();

	std::vector<cv::KeyPoint> kps_box, kps_bis;
    cv::Mat descs_box, descs_bis;
	orb->detectAndCompute(box, cv::Mat(), kps_box, descs_box);
    orb->detectAndCompute(box_in_scene, cv::Mat(), kps_bis, descs_bis);

	// 描述子匹配 — FLANN匹配
    // auto flann = cv::FlannBasedMatcher::create(); // Default is bad, using Locality Sensitive Hashing (LSH)
    auto flann = cv::makePtr<cv::FlannBasedMatcher>(cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2));

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

	// Localize the object
	std::vector<cv::Point2f> pts_box;
	std::vector<cv::Point2f> pts_bis;

	for (size_t i = 0; i < goodMatches.size(); i++)
	{
		// Get the keypoints from the good matches
		pts_box.push_back(kps_box[goodMatches[i].queryIdx].pt);
		pts_bis.push_back(kps_bis[goodMatches[i].trainIdx].pt);
	}

	// cv::Mat H = cv::findHomography(pts_box, pts_bis, cv::RANSAC); // 有时无法配准
    cv::Mat H = cv::findHomography(pts_box, pts_bis, cv::RHO);

	// Get the corners from the image 1 (the object to be detected)
	std::vector<cv::Point2f> corners_box(4);
	corners_box[0] = cv::Point(0, 0); corners_box[1] = cv::Point(box.cols, 0);
	corners_box[2] = cv::Point(box.cols, box.rows); corners_box[3] = cv::Point(0, box.rows);

	std::vector<cv::Point2f> corners_bis(4);
    cv::perspectiveTransform(corners_box, corners_bis, H);

	// Draw lines between the corners (the mapped object in the scene - image 2)
    for (uint i = 0; i < 4; ++i)
    {
        cv::line(img_matches, corners_bis[i] + cv::Point2f(box.cols, 0), corners_bis[(i + 1) % 4] + cv::Point2f(box.cols, 0), cv::Scalar(0, 255, 0), 4);
    }

	// Show detected matches and detected object
    cv::imshow("good matches & object detection", img_matches);

    cv::waitKey(0);

	return cv::Error::StsOk;
}

/* end of file */
