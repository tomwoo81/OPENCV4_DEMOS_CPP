int OpencvDemo123();
int OpencvDemo124();
int OpencvDemo130();
int OpencvDemo134();
int OpencvDemo135();

// OpenCV DNN 解析网络输出结果
int OpencvDemo136() {
    int ret1, ret2, ret3, ret4, ret5;

    // image classification network
    ret1 = OpencvDemo123();

    // object detection network (SSD/RCNN/Faster-RCNN)
    ret2 = OpencvDemo124();

    // object detection network (YOLO)
    ret3 = OpencvDemo130();

    // image segmentation
    ret4 = OpencvDemo134();

    // image generation
    ret5 = OpencvDemo135();

    return (ret1 || ret2 || ret3 || ret4 || ret5);
}

/* end of file */
