#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

std::vector<std::string> labels;
std::vector<std::vector<float>> text_features;

int train_data();
std::vector<float> ExtractFeature(const cv::Mat& txtImage);
static float GetWeightBlackNumber(const cv::Mat& image, const int width, const int height, 
								  const float x, const float y, const float xstep, const float ystep);

// 案例：识别0～9印刷体数字—Part 1
int OpencvDemo139() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_DEBUG);

	// 训练
	if (cv::Error::StsOk != train_data()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "Fail to train data!");
		return cv::Error::StsError;
	}

	cv::waitKey(0);

	return cv::Error::StsOk;
}

int train_data() {
	cv::Mat src = cv::imread("images/td1.png");
	if (src.empty()) {
		CV_LOG_ERROR(CV_LOGTAG_GLOBAL, "could not load an image!");
		return cv::Error::StsError;
	}

    cv::Mat dst = src.clone();

    cv::Mat gray, binary;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, cv::Point());

    std::vector<cv::Rect> rects;
	for (size_t i = 0; i < contours.size(); i++) {
		cv::Rect rect = cv::boundingRect(contours[i]);
		rects.emplace_back(rect);
		cv::rectangle(dst, rect, cv::Scalar(0, 0, 255), 1);
	}

	// sort the ROIs in ascending order
	std::sort(rects.begin(), rects.end(), [](auto r1, auto r2){ return r1.x < r2.x; });

	for (size_t i = 0; i < rects.size(); i++) {
		std::vector<float> feature = ExtractFeature(binary(rects[i]));

		text_features.emplace_back(feature);

		if ((rects.size() - 1) == i) {
			labels.emplace_back("0");
		}
		else {
			labels.emplace_back(cv::format("%zu", (i + 1)));
		}
	}

	int h = src.rows;
	int w = src.cols;
	cv::Mat result = cv::Mat::zeros(cv::Size(w, h * 2), src.type());
	src.copyTo(result(cv::Range(0, h), cv::Range(0, w)));
	dst.copyTo(result(cv::Range(h, h * 2), cv::Range(0, w)));
	cv::putText(result, "original image", cv::Point(10, 15), cv::FONT_ITALIC, 0.5, cv::Scalar(0, 0, 255), 1);
	cv::putText(result, "image with digit ROIs", cv::Point(10, h + 15), cv::FONT_ITALIC, 0.5, cv::Scalar(0, 0, 255), 1);
	cv::imshow("digit recognition - train data", result);

	return cv::Error::StsOk;
}

std::vector<float> ExtractFeature(const cv::Mat& txtImage) {
	// total black pixels
	std::vector<float> feature(40);

	int width = txtImage.cols;
	int height = txtImage.rows;

	// feature
	float bins = 10.0f;
	float xstep = width / 4.0f;
	float ystep = height / 5.0f;

	int index = 0;
	for (float y = 0; y < height; y += ystep) {
		for (float x = 0; x < width; x += xstep) {
			feature[index] = GetWeightBlackNumber(txtImage, width, height, x, y, xstep, ystep);
			index++;
		}
	}

	// calculate Y Project
	xstep = width / bins;
	for (float x = 0; x < width; x += xstep) {
		if ((x + xstep) - width > 1) continue;
		feature[index] = GetWeightBlackNumber(txtImage, width, height, x, 0, xstep, height);
		index++;
	}

	// calculate X Project
	ystep = height / bins;
	for (float y = 0; y < height; y += ystep) {
		if ((y + ystep) - height > 1) continue;
		feature[index] = GetWeightBlackNumber(txtImage, width, height, 0, y, width, ystep);
		index++;
	}

	// normalization of feature vector

	// 4x5 cells = 20-element vector
	float sum = 0;
	for (int i = 0; i < 20; i++) {
		sum += feature[i];
	}
	for (int i = 0; i < 20; i++) {
		feature[i] /= sum;
	}

	// Y Projection 10-element vector
	sum = 0;
	for (int i = 20; i < 30; i++) {
		sum += feature[i];
	}
	for (int i = 20; i < 30; i++) {
		feature[i] /= sum;
	}

	// X Projection 10-element vector
	sum = 0;
	for (int i = 30; i < 40; i++) {
		sum += feature[i];
	}
	for (int i = 30; i < 40; i++) {
		feature[i] /= sum;
	}

	return std::move(feature);
}

static float GetWeightBlackNumber(const cv::Mat& image, const int width, const int height, 
								  const float x, const float y, const float xstep, const float ystep) {
	float weightNum = 0;

	// 取整
	int nx = (int)floor(x);
	int ny = (int)floor(y);

	// 浮点数
	float fx = x - nx;
	float fy = y - ny;

	// 计算位置
	float w = x + xstep;
	float h = y + ystep;
	if (w - width > 1e-3) {
		w = width - 1;
	}
	if (h - height > 1e-3) {
		h = height - 1;
	}

	// 权重取整
	int nw = (int)floor(w);
	int nh = (int)floor(h);

	// 浮点数
	float fw = w - nw;
	float fh = h - nh;

	// 计算
	int c, weight = 0;
	int row, col;
	for (row = ny; row < nh; row++) {
		for (col = nx; col < nw; col++) {
			c = image.at<uchar>(row, col);
			if (c == 0) {
				weight++;
			}
		}
	}

	float w1 = 0, w2 = 0, w3 = 0, w4 = 0;

	// calculate w1
	if (fx > 1e-3) {
		col = nx + 1;
		if (col > width - 1) {
			col = col - 1;
		}
		int count = 0;
		for (row = ny; row < nh; row++) {
			c = image.at<uchar>(row, col);
			if (c == 0) {
				count++;
			}
		}
		w1 = count * fx;
	}

	// calculate w2
	if (fy > 1e-3) {
		row = ny + 1;
		if (row > height - 1) {
			row = row - 1;
		}
		int count = 0;
		for (col = nx; col < nw; col++) {
			c = image.at<uchar>(row, col);
			if (c == 0) {
				count++;
			}
		}
		w2 = count * fy;
	}

	// calculate w3
	if (fw > 1e-3) {
		col = nw + 1;
		if (col > width - 1) {
			col = col - 1;
		}
		int count = 0;
		for (row = ny; row < nh; row++) {
			c = image.at<uchar>(row, col);
			if (c == 0) {
				count++;
			}
		}
		w3 = count * fw;
	}

	// calculate w4
	if (fh > 1e-3) {
		row = nh + 1;
		if (row > height - 1) {
			row = row - 1;
		}
		int count = 0;
		for (col = nx; col < nw; col++) {
			c = image.at<uchar>(row, col);
			if (c == 0) {
				count++;
			}
		}
		w4 = count * fh;
	}

	weightNum = weight - w1 - w2 + w3 + w4;

	if (weightNum < 0) {
		weightNum = 0;
	}

	return weightNum;
}

/* end of file */
