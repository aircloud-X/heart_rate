#include "heartrate.h"
#include <Arduino.h>

int avg = 0;                       // 平均值
int data_index = 0;                // 数据索引
bool peak = false;                 // 峰值标志
int reading = 0;                   // 读数
float BPM = 0.0;                   // 心率
float lastBPM = -1.0;              // 上一次心率
bool IgnoreReading = false;        // 忽略读数标志
bool FirstPulseDetected = false;   // 第一次脉冲检测标志
unsigned long lastPeakTime = 0;    // 上一个R波的时间戳
unsigned long FirstPulseTime = 0;  // 第一次脉冲时间
unsigned long SecondPulseTime = 0; // 第二次脉冲时间
unsigned long PulseInterval = 0;   // 脉冲间隔时间
CircularBuffer<int, 10> buffer;    // 循环缓冲区，用于存储脉冲间隔时间

// 获取峰值函数
bool Getpeak(float new_sample)
{
    // 数据、均值和标准差缓冲区
    static float data_buffer[DATA_LENGTH];
    static float mean_buffer[DATA_LENGTH];
    static float standard_deviation_buffer[DATA_LENGTH];

    // 检测峰值
    if (new_sample - mean_buffer[data_index] > 0.4 * DATA_LENGTH * standard_deviation_buffer[data_index])
    {
        data_buffer[data_index] = new_sample + data_buffer[data_index];
        peak = true;
    }
    else
    {
        data_buffer[data_index] = new_sample;
        peak = false;
    }

    // 计算均值
    float sum = 0.0, mean, standard_deviation = 0.0;
    for (int i = 0; i < DATA_LENGTH; ++i)
    {
        sum += data_buffer[(data_index + i) % DATA_LENGTH];
    }
    mean = sum / DATA_LENGTH;

    // 计算标准差
    for (int i = 0; i < DATA_LENGTH; ++i)
    {
        standard_deviation += pow(data_buffer[(i) % DATA_LENGTH] - mean, 2);
    }

    // 更新均值缓冲区
    mean_buffer[data_index] = mean;

    // 更新标准差缓冲区
    standard_deviation_buffer[data_index] = sqrt(standard_deviation / DATA_LENGTH);

    // 更新数据索引
    data_index = (data_index + 1) % DATA_LENGTH;

    // 返回峰值
    return peak;
}

// 带通Butterworth IIR数字滤波器函数
float ECGFilter(float input)
{
    float output = input;
    {
        static float z1, z2; // 滤波器状态
        float x = output - 0.70682283 * z1 - 0.15621030 * z2;
        output = 0.28064917 * x + 0.56129834 * z1 + 0.28064917 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // 滤波器状态
        float x = output - 0.95028224 * z1 - 0.54073140 * z2;
        output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // 滤波器状态
        float x = output - -1.95360385 * z1 - 0.95423412 * z2;
        output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // 滤波器状态
        float x = output - -1.98048558 * z1 - 0.98111344 * z2;
        output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    return output;
}

int calculateBPM()
{
    // 计算已过时间
    static unsigned long past = 0;
    unsigned long present = micros();
    unsigned long interval = present - past;
    past = present;

    // 运行计时器
    static long timer = 0;
    timer -= interval;

    // 采样
    if (timer < 0)
    {
        timer += 1000000 / SAMPLE_RATE;
        // 采样并归一化输入数据（-1 到 1）
        int tmp_BPM = 0;
        for (size_t i = 0; i < 10; i++)
        {
            tmp_BPM += analogRead(INPUT_PIN);
        }
        float sensor_value = tmp_BPM / 10;
        Serial.print(">sensor_value:");
        Serial.println(sensor_value);
        // float sensor_value = analogRead(INPUT_PIN);
        float signal = ECGFilter(sensor_value) / 512;
        // 获取峰值
        peak = Getpeak(signal);
        // 打印传感器数值和峰值
        Serial.print(">signal:");
        Serial.println(signal);

        Serial.print(">peak:");
        Serial.println(peak);

        if (peak && IgnoreReading == false && (millis() - lastPeakTime > 250))
        {
            if (FirstPulseDetected == false)
            {
                FirstPulseTime = millis();
                FirstPulseDetected = true;
            }
            else
            {
                SecondPulseTime = millis();
                PulseInterval = SecondPulseTime - FirstPulseTime;
                buffer.unshift(PulseInterval);
                FirstPulseTime = SecondPulseTime;
            }
            IgnoreReading = true;
            lastPeakTime = millis();
        }
        if (!peak)
        {
            IgnoreReading = false;
        }
        if (buffer.isFull())
        {
            for (int i = 0; i < buffer.size(); i++)
            {
                avg += buffer[i];
            }
            if (avg != 0)
                BPM = (1.0 / avg) * 60 * 1000 * buffer.size();
            if (BPM > 20 && BPM < 240)
            {

                Serial.print(">BPM: ");
                Serial.println(BPM);
                // 检查两次测得的BPM值差值
                if (lastBPM >= 0 && fabs(BPM - lastBPM) < 1.0)
                {
                    int Stable_BPM = (int)BPM;
                    // Serial.print(">Stable_BPM:");
                    // Serial.println(Stable_BPM);
                    return Stable_BPM;
                }

                lastBPM = BPM; // 更新上一次的BPM值
                // return (int)BPM;
            }

            avg = 0;
            buffer.pop();
        }
    }
    return -1;
}
