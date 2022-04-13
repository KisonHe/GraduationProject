# ESP32 & RoboMaster motors
水毕设，3508+ESP_Dash，ESP32控制

## LVGL Note
注意检查style的生命周期，要与被赋予style的对象共存亡

```bash
node ./node_modules/lv_font_conv/lv_font_conv.js --font ~/Downloads/NotoSansMonoCJKsc-Regular.otf --symbol "123456abcdef我是中国人" --lcd --size 16 --bpp 4 --format bin -o data/customfont.bin
./node_modules/lv_img_conv/lv_img_conv.js ~/Downloads/gyro-200.png -f -c CF_TRUE_COLOR_ALPHA -t bin --binary-format 565 -o gyro.bin
```

获取Fonts `https://github.com/googlefonts/noto-cjk`

有空的时候记得修改 `https://github.com/KisonHe/TFT_eSPI_Assi` ，改成支持lv_font_conv

Get Offline Tool
```bash
npm i lv_font_conv
npm i lv_img_conv
```