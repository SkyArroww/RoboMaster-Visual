# 李东校

GitHub链接：[Homework 1](https://github.com/SkyArroww/RoboMaster-Visual/tree/main/Homework 1)

###  T1    Shell

本人shell小白，所以去网上搜了搜，结果如下：

time [time使用说明](https://blog.csdn.net/qq_34595352/article/details/86702411)

find [find使用说明](https://www.cnblogs.com/soymilk2019/p/11758086.html)

根据这两篇博客简单入门了一下。所以对于该问题，仅需执行

time find Test/ -name "*.c" -o -name "*.h" -type f | xargs grep hello -n

其中Test为当前目录下文件夹名称，若想查找其他文件夹下文件，把它替换为其他文件夹位置即可



###  T2    识别苹果

步骤：

 	1. 通过多通道相减或HSV颜色提取对图像二值化
 	2. 通过形态学运算或滤波对二值化图像进行处理
 	3. 找出轮廓并筛选
 	4. 画图



本张图片主要有四种颜色：蓝色的天空，绿色的树叶，红色的苹果正面，棕色的苹果底面和树干。

前三者较为容易区分，而有一部分树干连在苹果左上方，比较难以处理。

对于此图片，多通道相减法效果更好 (也可能是窝hsv调参木有调好/(ㄒoㄒ)/~~

顺便贴个还不错的教程 [形态学处理](https://zhuanlan.zhihu.com/p/511628175)

(卷积核选择MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE 效果有挺大差异的，可以适当选择



###  T3    识别小弹丸

步骤：

	1. 二值化筛选出弹道
	2. 找出当前帧最大的矩形标定为当前帧弹道
	3. 用矩形四个顶点坐标算出中心位置并标定为当前子弹位置
	4. 比较前后两帧子弹距离确定是否为同一颗子弹

看了看lcy大佬的想法，考虑的因素比窝的充分很多，向大佬学习o(*￣▽￣*)ブ













