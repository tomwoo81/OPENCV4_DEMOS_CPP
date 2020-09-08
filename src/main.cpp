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

	default:
		std::cerr << "The argument is invalid!" << std::endl;
		return -1;
	}

	return ret;
}

/* end of file */
