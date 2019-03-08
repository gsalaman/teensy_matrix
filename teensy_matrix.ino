#include <SmartLEDShieldV4.h>
#include <SmartMatrix3.h>

#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kMatrixWidth = 128;        // known working: 32, 64, 96, 128
const uint8_t kMatrixHeight = 32;       // known working: 16, 32, 48, 64
const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN; // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels, or use SMARTMATRIX_HUB75_64ROW_MOD32SCAN for common 64x64 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);
//const uint8_t kIndexedLayerOptions = (SM_INDEXED_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);
//SMARTMATRIX_ALLOCATE_INDEXED_LAYER(indexedLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kIndexedLayerOptions);

rgb24 myColor = {0x80,0,0};

#define BLINK_TIME 250
#define LED_PIN 13

void setup() 
{
    pinMode(LED_PIN, OUTPUT);
    
    // Initialize Matrix
    matrix.addLayer(&backgroundLayer); 
    matrix.addLayer(&scrollingLayer); 
    matrix.begin();

    matrix.setBrightness(255);

    scrollingLayer.setColor({0xff, 0xff, 0xff});
    scrollingLayer.setMode(wrapForward);
    scrollingLayer.setSpeed(40);
    //scrollingLayer.setFont(font6x10);
    scrollingLayer.setFont(font8x13);
    scrollingLayer.start("Teensy 128x64 matrix test", -1);
     
}


void loop()
{  
  static unsigned long last_blink_time=0;
  unsigned long current_time;
  static bool led_state=LOW;
  static int x=0;
  static int y=31;

  current_time = millis();
  if (current_time > last_blink_time + BLINK_TIME) 
  {
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
    last_blink_time = current_time;

    backgroundLayer.fillScreen({0,0,0});
    backgroundLayer.drawRectangle(x,y,x+10,y-10,myColor);
    backgroundLayer.swapBuffers();
    x = (x+1) % (kMatrixWidth - 10);    
  }

}
