// heartrate.h
#ifndef HEARTRATE_H
#define HEARTRATE_H

#include <CircularBuffer.h>
// #include <Arduino.h>

#define SAMPLE_RATE 125  // 定义采样率
#define BAUD_RATE 115200 // 波特率
#define INPUT_PIN A1     // 输入引脚
#define DATA_LENGTH 16   // 数据长度

// 声明全局变量
extern int avg;                        // 平均值
extern int data_index;                 // 数据索引
extern bool peak;                      // 峰值标志
extern int reading;                    // 读数
extern float BPM;                      // 心率
extern float lastBPM;                  // 上一次心率
extern bool IgnoreReading;             // 忽略读数标志
extern bool FirstPulseDetected;        // 第一次脉冲检测标志
extern unsigned long lastPeakTime;     // 上一个R波的时间戳
extern unsigned long FirstPulseTime;   // 第一次脉冲时间
extern unsigned long SecondPulseTime;  // 第二次脉冲时间
extern unsigned long PulseInterval;    // 脉冲间隔时间
extern CircularBuffer<int, 10> buffer; // 循环缓冲区，用于存储脉冲间隔时间

// 声明函数原型
bool Getpeak(float new_sample); // 获取峰值函数
float ECGFilter(float input);   // 带通Butterworth IIR数字滤波器函数
int calculateBPM();             // 计算BPM函数

#endif