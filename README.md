# Fast-tracking-car-based-on-photoelectric-sensor

记录一次课程设计：硬件组成+软件

基于stm32f103c8t6最小系统板的光电管循迹小车

使用到stm32f103c8t6最小系统板，主频为72Mhz，性能一般，使用vofa+进行速度环和转向环pid调试，只是够基础使用。在定时中断中通信失败，无法进行规律的数据传输，有能力的可以用更高性能芯片，将数据定时输出再使用matlab系统辨识，最后使用simulink进行pid参数镇定，效果超级好。

电源使用降压芯片3.3V供给传感器和单片机，升压芯片6v进行电机（额定6V）供电,但是电机运行两端电压减小故升至7.4V供电，运行后刚好6V，中间串接电调进行pwm速度控制。

使用带有正交编码器的直流电机进行速度控制，编码器精度较低，仅有11线，进行四倍频后就有了44脉冲一圈编码器


修改建议：使用DMA进行蓝牙传输，对传感器数据滤波

![IMG_20240531_003047](https://github.com/user-attachments/assets/0d870a6b-b312-4baa-959f-9819f3cd4223)

![IMG_20240531_003038](https://github.com/user-attachments/assets/a9f3fea2-e0ad-4c32-b8c2-f7af3628289e)
