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
int OpencvDemo045();
int OpencvDemo046();
int OpencvDemo047();
int OpencvDemo048();
int OpencvDemo049();
int OpencvDemo050();
int OpencvDemo051();
int OpencvDemo052();
int OpencvDemo053();
int OpencvDemo054();
int OpencvDemo055();
int OpencvDemo056();
int OpencvDemo057();
int OpencvDemo058();
int OpencvDemo059();
int OpencvDemo060();
int OpencvDemo061();
int OpencvDemo062();
int OpencvDemo063();
int OpencvDemo064();
int OpencvDemo065();
int OpencvDemo066();
int OpencvDemo067();
int OpencvDemo068();
int OpencvDemo069();
int OpencvDemo070();
int OpencvDemo071();
int OpencvDemo072();
int OpencvDemo073();
int OpencvDemo074();
int OpencvDemo075();
int OpencvDemo076();
int OpencvDemo077();
int OpencvDemo078();
int OpencvDemo079();
int OpencvDemo080();
int OpencvDemo081();
int OpencvDemo082();
int OpencvDemo083();
int OpencvDemo084();
int OpencvDemo085();
int OpencvDemo086();
// int OpencvDemo087();
// int OpencvDemo088();
// int OpencvDemo089();
// int OpencvDemo090();

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

	case 45:
		// 图像二值化与去噪
		ret = OpencvDemo045();
		break;

	case 46:
		// 二值图像连通组件寻找
		ret = OpencvDemo046();
		break;

	case 47:
		// 二值图像连通组件状态统计
		ret = OpencvDemo047();
		break;

	case 48:
		// 二值图像分析—轮廓发现
		ret = OpencvDemo048();
		break;

	case 49:
		// 二值图像分析—轮廓外接矩形
		ret = OpencvDemo049();
		break;

	case 50:
		// 二值图像分析—轮廓面积与弧长
		ret = OpencvDemo050();
		break;

	case 51:
		// 二值图像分析—使用轮廓逼近
		ret = OpencvDemo051();
		break;

	case 52:
		// 二值图像分析—用几何矩计算轮廓中心与横纵比过滤
		ret = OpencvDemo052();
		break;

	case 53:
		// 二值图像分析—Hu矩实现轮廓匹配
		ret = OpencvDemo053();
		break;

	case 54:
		// 二值图像分析—对轮廓圆与椭圆拟合
		ret = OpencvDemo054();
		break;

	case 55:
		// 二值图像分析—凸包检测
		ret = OpencvDemo055();
		break;

	case 56:
		// 二值图像分析—直线拟合与极值点寻找
		ret = OpencvDemo056();
		break;

	case 57:
		// 二值图像分析—点多边形测试
		ret = OpencvDemo057();
		break;

	case 58:
		// 二值图像分析—寻找最大内接圆
		ret = OpencvDemo058();
		break;

	case 59:
		// 二值图像分析—霍夫直线检测
		ret = OpencvDemo059();
		break;

	case 60:
		// 二值图像分析—霍夫直线检测二
		ret = OpencvDemo060();
		break;

	case 61:
		// 二值图像分析—霍夫圆检测
		ret = OpencvDemo061();
		break;

	case 62:
		// 图像形态学—膨胀与腐蚀
		ret = OpencvDemo062();
		break;

	case 63:
		// 图像形态学—膨胀与腐蚀二
		ret = OpencvDemo063();
		break;

	case 64:
		// 图像形态学–开操作
		ret = OpencvDemo064();
		break;

	case 65:
		// 图像形态学–闭操作
		ret = OpencvDemo065();
		break;

	case 66:
		// 图像形态学—开闭操作时候结构元素应用演示
		ret = OpencvDemo066();
		break;

	case 67:
		// 图像形态学—顶帽操作
		ret = OpencvDemo067();
		break;

	case 68:
		// 图像形态学—黑帽操作
		ret = OpencvDemo068();
		break;

	case 69:
		// 图像形态学—图像梯度
		ret = OpencvDemo069();
		break;

	case 70:
		// 形态学应用—用基本梯度实现轮廓分析
		ret = OpencvDemo070();
		break;

	case 71:
		// 形态学操作—击中击不中
		ret = OpencvDemo071();
		break;

	case 72:
		// 二值图像分析—缺陷检测一
		ret = OpencvDemo072();
		break;

	case 73:
		// 二值图像分析—缺陷检测二
		ret = OpencvDemo073();
		break;

	case 74:
		// 二值图像分析—提取最大轮廓与编码关键点
		ret = OpencvDemo074();
		break;

	case 75:
		// 图像去水印/修复
		ret = OpencvDemo075();
		break;

	case 76:
		// 图像透视变换应用
		ret = OpencvDemo076();
		break;

	case 77:
		// 视频读写与处理
		ret = OpencvDemo077();
		break;

	case 78:
		// 识别与跟踪视频中的特定颜色对象
		ret = OpencvDemo078();
		break;

	case 79:
		// 视频分析—背景/前景提取
		ret = OpencvDemo079();
		break;

	case 80:
		// 视频分析—背景消除与前景ROI提取
		ret = OpencvDemo080();
		break;

	case 81:
		// 角点检测—Harris角点检测
		ret = OpencvDemo081();
		break;

	case 82:
		// 角点检测—Shi-Tomasi角点检测
		ret = OpencvDemo082();
		break;

	case 83:
		// 角点检测–亚像素级别角点检测
		ret = OpencvDemo083();
		break;

	case 84:
		// 视频分析—移动对象的KLT光流跟踪算法之一
		ret = OpencvDemo084();
		break;

	case 85:
		// 视频分析-移动对象的KLT光流跟踪算法之二
		ret = OpencvDemo085();
		break;

	case 86:
		// 视频分析–稠密光流分析
		ret = OpencvDemo086();
		break;

	// case 87:
	// 	// 
	// 	ret = OpencvDemo087();
	// 	break;

	// case 88:
	// 	// 
	// 	ret = OpencvDemo088();
	// 	break;

	// case 89:
	// 	// 
	// 	ret = OpencvDemo089();
	// 	break;

	// case 90:
	// 	// 
	// 	ret = OpencvDemo090();
	// 	break;

	default:
		std::cerr << "The argument is invalid!" << std::endl;
		return -1;
	}

	return ret;
}

/* end of file */
