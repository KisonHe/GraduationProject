# ESP32 & RoboMaster motors
水毕设，3508+ESP_Dash，ESP32控制

## LVGL Note
注意检查style的生命周期，要与被赋予style的对象共存亡

```
node ./node_modules/lv_font_conv/lv_font_conv.js --font ~/Downloads/NotoSansMonoCJKsc-Regular.otf --symbol "123456abcdef我是中国人" --lcd --size 16 --bpp 4 --format bin -o myFont.bin
```
有空的时候建议修改`https://github.com/KisonHe/TFT_eSPI_Assi`，改成支持lv_font_conv