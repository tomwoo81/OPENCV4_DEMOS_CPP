#include <stdlib.h>
#include <iostream>

int OpencvDemo001();
int OpencvDemo002();
int OpencvDemo003();
int OpencvDemo004();
int OpencvDemo005();
int OpencvDemo006();
int OpencvDemo007();
int OpencvDemo008();
int OpencvDemo009();
int OpencvDemo010();
int OpencvDemo011();
int OpencvDemo012();
int OpencvDemo013();
int OpencvDemo014();
int OpencvDemo015();
int OpencvDemo016();
int OpencvDemo017();
int OpencvDemo018();
int OpencvDemo019();
int OpencvDemo020();
int OpencvDemo021();
int OpencvDemo022();
int OpencvDemo023();
int OpencvDemo024();
int OpencvDemo025();
int OpencvDemo026();
int OpencvDemo027();
int OpencvDemo028();
int OpencvDemo029();
int OpencvDemo030();
int OpencvDemo031();
int OpencvDemo032();
int OpencvDemo033();
int OpencvDemo034();
int OpencvDemo035();
int OpencvDemo036();
int OpencvDemo037();
int OpencvDemo038();
int OpencvDemo039();
int OpencvDemo040();
int OpencvDemo041();
int OpencvDemo042();
int OpencvDemo043();
int OpencvDemo044();
//int OpencvDemo045();
//int OpencvDemo046();
//int OpencvDemo047();
//int OpencvDemo048();
//int OpencvDemo049();
//int OpencvDemo050();

int main(int argc, char** argv) {
	if (argc < 2)
	{
		std::cerr << "No arguments!" << std::endl;
		return -1;
	}

	unsigned int demoId = atoi(argv[1]);
	int ret;

	switch (demoId)
	{
	case 1:
		// 图像读取与显示
		ret = OpencvDemo001();
		break;

	case 2:
		// 图像色彩空间转换
		ret = OpencvDemo002();
		break;

	case 3:
		// 图像对象的创建与赋值
		ret = OpencvDemo003();
		break;

	case 4:
		// 图像像素的读写操作
		ret = OpencvDemo004();
		break;

	case 5:
		// 图像像素的算术操作
		ret = OpencvDemo005();
		break;

	case 6:
		// LUT的作用与用法
		ret = OpencvDemo006();
		break;

	case 7:
		// 图像像素的逻辑操作
		ret = OpencvDemo007();
		break;

	case 8:
		// 通道分离与合并
		ret = OpencvDemo008();
		break;

	case 9:
		// 图像色彩空间转换
		ret = OpencvDemo009();
		break;

	case 10:
		// 图像像素值统计
		ret = OpencvDemo010();
		break;

	case 11:
		// 像素归一化
		ret = OpencvDemo011();
		break;

	case 12:
		// 视频文件的读写
		ret = OpencvDemo012();
		break;

	case 13:
		// 图像翻转
		ret = OpencvDemo013();
		break;

	case 14:
		// 图像插值
		ret = OpencvDemo014();
		break;

	case 15:
		// 几何形状绘制
		ret = OpencvDemo015();
		break;

	case 16:
		// 图像ROI与ROI操作
		ret = OpencvDemo016();
		break;

	case 17:
		// 图像直方图
		ret = OpencvDemo017();
		break;

	case 18:
		// 图像直方图均衡化
		ret = OpencvDemo018();
		break;

	case 19:
		// 图像直方图比较
		ret = OpencvDemo019();
		break;

	case 20:
		// 图像直方图反向投影
		ret = OpencvDemo020();
		break;

	case 21:
		// 图像卷积操作
		ret = OpencvDemo021();
		break;

	case 22:
		// 图像均值与高斯模糊
		ret = OpencvDemo022();
		break;

	case 23:
		// 中值模糊
		ret = OpencvDemo023();
		break;

	case 24:
		// 图像噪声
		ret = OpencvDemo024();
		break;

	case 25:
		// 图像去噪声
		ret = OpencvDemo025();
		break;

	case 26:
		// 高斯双边模糊
		ret = OpencvDemo026();
		break;

	case 27:
		// 均值迁移模糊
		ret = OpencvDemo027();
		break;

	case 28:
		// 图像积分图算法
		ret = OpencvDemo028();
		break;

	case 29:
		// 快速的图像边缘滤波算法
		ret = OpencvDemo029();
		break;

	case 30:
		// OpenCV自定义的滤波器
		ret = OpencvDemo030();
		break;

	case 31:
		// 图像梯度–Sobel算子
		ret = OpencvDemo031();
		break;

	case 32:
		// 图像梯度–更多梯度算子
		ret = OpencvDemo032();
		break;

	case 33:
		// 图像梯度–拉普拉斯算子
		ret = OpencvDemo033();
		break;

	case 34:
		// 图像锐化
		ret = OpencvDemo034();
		break;

	case 35:
		// USM锐化增强算法
		ret = OpencvDemo035();
		break;

	case 36:
		// Canny边缘检测器
		ret = OpencvDemo036();
		break;

	case 37:
		// 图像金字塔
		ret = OpencvDemo037();
		break;

	case 38:
		// 拉普拉斯金字塔
		ret = OpencvDemo038();
		break;

	case 39:
		// 图像模板匹配
		ret = OpencvDemo039();
		break;

	case 40:
		// 二值图像介绍
		ret = OpencvDemo040();
		break;

	case 41:
		// OpenCV中的基本阈值操作
		ret = OpencvDemo041();
		break;

	case 42:
		// OTSU二值寻找算法
		ret = OpencvDemo042();
		break;

	case 43:
		// TRIANGLE二值寻找算法
		ret = OpencvDemo043();
		break;

	case 44:
		// 自适应阈值算法
		ret = OpencvDemo044();
		break;

//	case 45:
//		// xxxx
//		ret = OpencvDemo045();
//		break;
//
//	case 46:
//		// xxxx
//		ret = OpencvDemo046();
//		break;
//
//	case 47:
//		// xxxx
//		ret = OpencvDemo047();
//		break;
//
//	case 48:
//		// xxxx
//		ret = OpencvDemo048();
//		break;
//
//	case 49:
//		// xxxx
//		ret = OpencvDemo049();
//		break;
//
//	case 50:
//		// xxxx
//		ret = OpencvDemo050();
//		break;

	default:
		std::cerr << "The argument is invalid!" << std::endl;
		return -1;
	}

	return ret;
}

/* end of file */
