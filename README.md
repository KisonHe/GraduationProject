# ESP32 & RoboMaster motors
水毕设，3508+ESP_Dash，ESP32控制

## LVGL Note
注意检查style的生命周期，要与被赋予style的对象共存亡

```bash
node ./node_modules/lv_font_conv/lv_font_conv.js --font ~/Downloads/NotoSansMonoCJKsc-Regular.otf --symbol "123456abcdef我是中国人" --lcd --size 16 --bpp 4 --format bin -o data/customfont.bin
./node_modules/lv_img_conv/lv_img_conv.js ~/Downloads/gyro-200.png -f -c CF_TRUE_COLOR_ALPHA -t bin --binary-format 565 -o gyro.bin
```

获取Fonts `https://github.com/googlefonts/noto-cjk`

[Download](https://github.com/googlefonts/noto-cjk/raw/main/Sans/Mono/NotoSansMonoCJKsc-Regular.otf)

有空的时候记得修改 `https://github.com/KisonHe/TFT_eSPI_Assi` ，改成支持lv_font_conv

Get Offline Tool
```bash
npm i lv_font_conv
npm i lv_img_conv
```

## ESP32 Note
根据 https://github.com/espressif/arduino-esp32/issues/3079#issuecomment-520161166 
nnd,又要改，直接来吧！

## ESP32 - FreeRTOS
在Commit 754485b中，会出现manager或can read太久没有被执行的问题。TODO:目前没有完全理清楚，初步认定是TaskSkip导致的问题
https://docs.espressif.com/projects/esp-idf/en/v4.3.2/esp32/api-guides/freertos-smp.html#round-robin-scheduling

有趣的现象，如果把lvgl的优先级从2改到22,反而不会出现电机疯掉问题了。初步认为wifi task，isr等以及一大堆task彻底抢占了core0,使得剩下的都在core1上，使得taskskip不再出现

一定要搞清楚！目前为了先弄毕业设计先都pin to core 1

排除以上问题，原因是can的接收频率不稳定，如果can rx是tskNO_AFFINITY则会出现接收频率降低问题。注意官方V4的example
```
xTaskCreatePinnedToCore(twai_receive_task, "TWAI_rx", 4096, NULL, RX_TASK_PRIO, NULL, tskNO_AFFINITY);
```
其中RX_TASK_PRIO为9,需要搞清楚为什么

目前在canrx处于tskNO_AFFINITY情况下，即使将lvgl task降到0也会导致canrx频率下降，急需弄清楚

似乎是因为偷懒带来的惨痛教训，从网上抄的CAN初始化没有仔细看，换成CAN_MODE_NORMAL后似乎没问题了。那么，nnd,为什么从2改到22,反而不会出现电机疯掉问题了？
