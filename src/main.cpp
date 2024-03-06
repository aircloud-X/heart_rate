
// #include <Arduino.h>
// #include "heartrate.h"
// #include <lvgl.h>

// #include <TFT_eSPI.h> // TFT库

// /*Change to your screen resolution*/
// static const uint16_t screenWidth = 320;
// static const uint16_t screenHeight = 240;

// static lv_disp_draw_buf_t draw_buf;
// static lv_color_t buf[screenWidth * 60];

// TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

// /* Display flushing */
// void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
// {
//   uint32_t w = (area->x2 - area->x1 + 1);
//   uint32_t h = (area->y2 - area->y1 + 1);

//   tft.startWrite();                                        // 使能写功能
//   tft.setAddrWindow(area->x1, area->y1, w, h);             // 设置填充区域
//   tft.pushColors((uint16_t *)&color_p->full, w * h, true); // 写入颜色缓存和缓存大小
//   tft.endWrite();                                          // 关闭写功能

//   lv_disp_flush_ready(disp); // 调用区域填充颜色函数
// }

// void setup()
// {
//   Serial.begin(115200);

//   tft.begin();        /* TFT init */
//   tft.setRotation(1); /* Landscape orientation, flipped */

//   lv_init();

//   lv_disp_draw_buf_init(&draw_buf, buf, buf, screenWidth * 60);
//   /*Initialize the display*/
//   static lv_disp_drv_t disp_drv;
//   lv_disp_drv_init(&disp_drv);
//   /*Change the following line to your display resolution*/
//   disp_drv.hor_res = screenWidth;
//   disp_drv.ver_res = screenHeight;
//   disp_drv.draw_buf = &draw_buf;
//   disp_drv.flush_cb = my_disp_flush;

//   lv_disp_drv_register(&disp_drv);
// }

// void loop()
// {
//   lv_task_handler(); // 处理LVGL任务
//   delay(10);         // 尽可能让系统休息一下，减少CPU负载
// }

// // void loop()
// // {
// // int bpm = calculateBPM();
// // if (bpm != -1)
// // {
// //   Serial.print(">Stable_BPM:");
// //   Serial.println(bpm);
// // }
// // delay(10);
// // }

// #include <Arduino.h>
// #include <lvgl.h>
// #include <TFT_eSPI.h>

// /*Change to your screen resolution*/
// static const uint16_t screenWidth = 320;
// static const uint16_t screenHeight = 240;

// static lv_disp_draw_buf_t draw_buf;
// static lv_color_t buf[screenWidth * 60];

// TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

// lv_chart_series_t *series;
// lv_obj_t *chart;

// float your_function_of_time(int timeElapsed)
// {
//   return sin((timeElapsed * 2 * 3.1415926) / 1000) * 100;
// }

// /* Display flushing */
// void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
// {
//   uint32_t w = (area->x2 - area->x1 + 1);
//   uint32_t h = (area->y2 - area->y1 + 1);

//   tft.startWrite();
//   tft.setAddrWindow(area->x1, area->y1, w, h);
//   tft.pushColors((uint16_t *)&color_p->full, w * h, true);
//   tft.endWrite();

//   lv_disp_flush_ready(disp);
// }

// void setup()
// {
//   Serial.begin(115200);

//   tft.begin();
//   tft.setRotation(1);

//   lv_init();

//   lv_disp_draw_buf_init(&draw_buf, buf, buf, screenWidth * 60);
//   static lv_disp_drv_t disp_drv;
//   lv_disp_drv_init(&disp_drv);
//   disp_drv.hor_res = screenWidth;
//   disp_drv.ver_res = screenHeight;
//   disp_drv.draw_buf = &draw_buf;
//   disp_drv.flush_cb = my_disp_flush;

//   lv_disp_drv_register(&disp_drv);

//   lv_obj_t *scr = lv_disp_get_scr_act(NULL);

//   chart = lv_chart_create(scr);
//   lv_obj_set_size(chart, screenWidth, screenHeight / 2);
//   lv_obj_align(chart, LV_ALIGN_CENTER, 0, 0);
//   lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
//   lv_chart_set_point_count(chart, screenWidth);
//   lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -100, 100);

//   series = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

//   // for (int i = 0; i < screenWidth; i++)
//   // {
//   //   lv_chart_set_next_value(chart, series, sin((i * 2 * 3.1415926) / screenWidth) * 100);
//   // }
// }

// void loop()
// {
//   static uint32_t lastUpdateTime = 0;
//   static int timeElapsed = 0;

//   uint32_t currentTime = millis();
//   uint32_t timeDiff = currentTime - lastUpdateTime;

//   if (timeDiff >= 100) // Update every 100 milliseconds
//   {
//     lastUpdateTime = currentTime;
//     timeElapsed += timeDiff;

//     lv_chart_set_next_value(chart, series, your_function_of_time(timeElapsed));
//   }

//   lv_task_handler();
//   delay(10);
// }

#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */

// 定义额外的缓冲区
#define BUFFER_WIDTH tft.width()
#define BUFFER_HEIGHT (tft.height() / 3) // 可以根据实际情况调整缓冲区高度

// 三个缓冲区
TFT_eSprite buffer1 = TFT_eSprite(&tft);
TFT_eSprite buffer2 = TFT_eSprite(&tft);
TFT_eSprite buffer3 = TFT_eSprite(&tft);

void setup()
{
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(1);

  // 设置每个缓冲区的大小为屏幕大小的一部分
  buffer1.createSprite(BUFFER_WIDTH, BUFFER_HEIGHT);
  buffer2.createSprite(BUFFER_WIDTH, BUFFER_HEIGHT);
  buffer3.createSprite(BUFFER_WIDTH, BUFFER_HEIGHT);

  tft.fillScreen(TFT_BLACK);
}

void loop()
{
  static float t = 0;           // 时间变量
  static float increment = 0.1; // 时间增量，调整这个值以改变变化的速度

  int x_prev, y_prev;
  float y_current;

  // 清除所有缓冲区的内容
  buffer1.fillSprite(TFT_BLACK);
  buffer2.fillSprite(TFT_BLACK);
  buffer3.fillSprite(TFT_BLACK);

  // 计算并绘制新的数据点到不同的缓冲区
  x_prev = 0;
  y_prev = BUFFER_HEIGHT / 2;
  for (int x = 1; x < BUFFER_WIDTH; x++)
  {
    y_current = sin(t) * (BUFFER_HEIGHT / 2 - 1); // 通过sin函数计算y坐标
    t += increment;                               // 增加时间变量
    if (x < BUFFER_WIDTH / 3)
    {
      buffer1.drawLine(x_prev, y_prev + BUFFER_HEIGHT / 2, x, y_current + BUFFER_HEIGHT / 2, TFT_RED);
    }
    else if (x < 2 * BUFFER_WIDTH / 3)
    {
      buffer2.drawLine(x_prev, y_prev + BUFFER_HEIGHT / 2, x, y_current + BUFFER_HEIGHT / 2, TFT_GREEN);
    }
    else
    {
      buffer3.drawLine(x_prev, y_prev + BUFFER_HEIGHT / 2, x, y_current + BUFFER_HEIGHT / 2, TFT_BLUE);
    }
    x_prev = x;
    y_prev = y_current;
  }

  // 将三个缓冲区内容一次性显示到屏幕上
  buffer1.pushSprite(0, 0);
  buffer2.pushSprite(0, BUFFER_HEIGHT);
  buffer3.pushSprite(0, 2 * BUFFER_HEIGHT);

  delay(30); // 延迟以控制绘制速度
}
