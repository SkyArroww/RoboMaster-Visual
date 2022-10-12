# 李东校

GitHub链接：[Homework 1]([RoboMaster-Visual/Homework 1 at main · SkyArroww/RoboMaster-Visual (github.com)](https://github.com/SkyArroww/RoboMaster-Visual/tree/main/Homework 1))

###  T1    Shell

本人shell小白，所以去网上搜了搜，结果如下：

time [time使用说明]([(4条消息) shell 中 time的使用_while_false_的博客-CSDN博客_shell time](https://blog.csdn.net/qq_34595352/article/details/86702411))

find [find使用说明]([shell编程：find命令 - 豆浆D - 博客园 (cnblogs.com)](https://www.cnblogs.com/soymilk2019/p/11758086.html))

根据这两篇博客简单入门了一下。所以对于该问题，仅需执行

time find Test/ -name "*.c" -o -name "*.h" -type f | xargs grep hello -n

其中Test为当前目录下文件夹名称，若想查找其他文件夹下文件，把它替换为其他文件夹位置即可



###  T2    识别苹果

本张图片主要有四种颜色：蓝色的天空，绿色的树叶，红色的苹果正面，棕色的苹果底面和树干。

前三者较为容易区分，而有一部分树干连在苹果左上方，比较难以处理。

本人分别使用了多通道相减和HSV颜色提取两种方法进行处理，结果如下：

多通道相减法

![多通道相减](C:\Users\Li Dongxiao\Desktop\RoboMaster Visual\Homework 1\T2 Apple\build\Apple_rgb.jpg)



HSV颜色提取

![HSV颜色提取](C:\Users\Li Dongxiao\Desktop\RoboMaster Visual\Homework 1\T2 Apple\build\Apple_hsv.jpg)



对于此图片，多通道相减法效果更好 (也可能是窝hsv调参木有调好/(ㄒoㄒ)/~~



###  T3    识别小弹丸















