#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

static void sort_boxes(std::vector<cv::Rect>& boxes);
static cv::Mat get_template(cv::Mat& binary, std::vector<cv::Rect>& rects);
static void detect_defects(cv::Mat& binary, std::vector<cv::Rect>& rects, cv::Mat& tpl, std::vector<cv::Rect>& defects);

// 二值图像分析—缺陷检测二
int OpencvDemo073() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

    // 输入图像
	cv::Mat src = cv::imread("images/ce_02.jpg");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    // 二值化图像
	cv::Mat gray, binary;
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

	// 定义结构元素
	cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(-1, -1));

    cv::Mat open;

    // 开操作
	cv::morphologyEx(binary, open, cv::MORPH_OPEN, se);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

    // 轮廓发现/轮廓分析
	cv::findContours(open, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat dst = src.clone();

    int height = src.rows;

    std::vector<cv::Rect> rects;

	for (size_t t = 0; t < contours.size(); t++) {
		cv::Rect rect = cv::boundingRect(contours[t]);
		double area = cv::contourArea(contours[t]);

		if (rect.height > (height / 2)) {
			continue;
		}

		if (area < 150) {
            continue;
        }

        // 轮廓填充/扩大
        cv::drawContours(binary, contours, t, cv::Scalar(0), 2, cv::LINE_8);

        rects.push_back(rect);
	}

    // 轮廓排序
    sort_boxes(rects);

	for (size_t i = 0; i < rects.size(); i++) {
		cv::putText(dst, cv::format("num: %zu", (i+1)), 
                    cv::Point(rects[i].x-60, rects[i].y+15), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 0, 0), 1);
	}

    cv::Mat tpl = get_template(binary, rects);

    std::vector<cv::Rect> defects;

    // 模板比对
    detect_defects(binary, rects, tpl, defects);

    // 输出结果
	for (size_t i = 0; i < defects.size(); i++) {
		cv::rectangle(dst, defects[i], cv::Scalar(0, 0, 255), 1, cv::LINE_8, 0);
		cv::putText(dst, "bad", cv::Point(defects[i].x, defects[i].y), 
                    cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 255, 0), 2);
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w * 2, h), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(0, h), cv::Range(w, w * 2)));
	cv::putText(result, "original image", cv::Point(10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with results of defect detection", cv::Point(w + 10, 30), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 1);
	cv::imshow("defect detection - 2", result);

	cv::waitKey(0);

	return cv::Error::StsOk;
}

static void sort_boxes(std::vector<cv::Rect>& boxes) {
	size_t size = boxes.size();

	for (size_t i = 0; i < size - 1; i++) {
		for (size_t j = i; j < size; j++) {
			if (boxes[j].y < boxes[i].y) {
				cv::Rect temp = boxes[i];
				boxes[i] = boxes[j];
				boxes[j] = temp;
			}
		}
	}
}

static cv::Mat get_template(cv::Mat& binary, std::vector<cv::Rect>& rects) {
	return binary(rects[0]);
}

static void detect_defects(cv::Mat& binary, std::vector<cv::Rect>& rects, cv::Mat& tpl, std::vector<cv::Rect>& defects) {
	int height = tpl.rows;
	int width = tpl.cols;
	size_t size = rects.size();

    defects.clear();

	for (size_t i = 0; i < size; i++) {
		cv::Mat roi = binary(rects[i]);
		cv::resize(roi, roi, tpl.size());

		cv::Mat mask;
		cv::subtract(tpl, roi, mask);

		cv::Mat se = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1));

		cv::morphologyEx(mask, mask, cv::MORPH_OPEN, se);
		cv::threshold(mask, mask, 0, 255, cv::THRESH_BINARY);

		int count = 0;

		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {
				uchar pv = mask.at<uchar>(row, col);
				if (pv == 255) {
					count++;
				}
			}
		}

		if (count > 0) {
			defects.push_back(rects[i]);
		}
	}
}

/* end of file */
