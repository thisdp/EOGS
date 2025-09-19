```cpp
//u8g2 is the screen instance

constexpr int16_t screenWidth = 120;
constexpr int16_t screenHeight = 64;

U8G2_EOGS_HAL EOGS_HAL(u8g2);
EOGS eogs = EOGS(EOGS_HAL,screenWidth,screenHeight);

void setup() {
  u8g2.begin();
  eogs.create<EOGSLabelContainer>(0,40,20,50,false,"This is a very simple embedded OLED UI System");
  label -> setFont(u8g2_font_t0_11_tf)
        -> setHAlign(HAlign::LEFT)
        -> setVAlign(VAlign::TOP)
        -> setClip(true)
        -> setScroll(true);
}

void loop(){
  eogs.render();
}
```
