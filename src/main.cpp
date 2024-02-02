#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#ifdef BUILD_MAPSCREEN_T4       // defined in platform.io env for the T4 display
  #include "MapScreen_T4.h"
  #include "LilyGo_amoled.h"
  #include <TFT_eSPI.h>

  TFT_eSPI tft = TFT_eSPI();
  LilyGo_AMOLED amoled;

  #include <lv_conf.h>
  #include <LV_Helper.h>
  //#include <lvgl.h>

#else
  #include <M5StickCPlus.h>
  #include <MapScreen_M5.h>
#endif  


//#include "Fonts/GFXFF/FreeSansBold24pt7b.h"
//#include "Fonts/GFXFF/FreeSansBoldOblique24pt7b.h"
//#include "Final_Frontier_28.h"
//#include "fonts/NotoSansBold36.h"
#include "fonts/NotoSansMonoSCB20.h"
//#include "Fonts/Custom/Yellowtail_32.h"

// .pio/libdeps/T-Display-AMOLED/TFT_eSPI/Fonts/GFXFF/FreeSansBold24pt7b.h

#include "dive_track.h"
extern const location diveTrack[];
extern "C" int getSizeOfDiveTrack();

MapScreen_ex* mapScreen;

extern const unsigned short lily_wraysbury_all[];
extern const unsigned short lily_wraysbury_N[];
extern const unsigned short lily_wraysbury_SE[];
extern const unsigned short lily_wraysbury_S[];
extern const unsigned short lily_wraysbury_SW[];
extern const unsigned short lily_wraysbury_W[];
extern const unsigned short Model_600x400[];
extern const unsigned short bride_600x342[];
extern const unsigned short Colorful_590x450[];
extern const unsigned short lion_600x400[];
extern const unsigned short visualCheck_410x304[];

class geo_location
{
  public:
    double la;
    double lo;
    int heading;
};

geo_location pos;

int trackIndex=0;
int trackLength=getSizeOfDiveTrack();
int testIteration=0;

void cycleTrackIndex();
void amoledTestRotatePhotos();

void setup()
{
  Serial.begin(115200);
  Serial.flush();
  delay(50);

#ifdef BUILD_MAPSCREEN_T4
  amoled.begin();
  amoled.setBrightness(255);
  mapScreen = new MapScreen_T4(tft,amoled);
#else
  M5.begin();
  mapScreen = new MapScreen_M5(M5.Lcd, M5);
  M5.Axp.ScreenBreath(100);
#endif

  mapScreen->setDrawAllFeatures(true);
  mapScreen->setUseDiverHeading(true);

  pos.la = 51.4601028571429;    // spitfire car
  pos.lo = -0.54883835;

  pos.la = 51.4605855;    // lightning boat
  pos.lo = -0.54890166666666; 

  return;
}

void loop()
{ 
  const bool testZoom = false;

  mapScreen->setTargetWaypointByLabel("Sub"); // Cafe Jetty

  if (trackIndex == 0)
  {
    mapScreen->setAllLakeShown(false);
    mapScreen->setTargetWaypointByLabel("Sub"); // Cafe Jetty
  }
  else if (trackIndex == 200)
  {
    mapScreen->setDrawAllFeatures(true);
  }
  else if (trackIndex == 400)
  {
    mapScreen->setTargetWaypointByLabel("Canoe"); // Mid Jetty    
  }
  else if (trackIndex == 500)
  {
    if (testZoom) mapScreen->setAllLakeShown(true);
  }
  else if (trackIndex == 600)
  {
     if (testZoom) mapScreen->setZoom(1);
  }
  else if (trackIndex == 700)
  {
     if (testZoom) mapScreen->setZoom(2);
  }
  else if (trackIndex == 800)
  {
     if (testZoom) mapScreen->setZoom(3);
  }
  else if (trackIndex == 900)
  {
    if (testZoom)  mapScreen->setZoom(4);
  }
  else if (trackIndex == 1000)
  {
     if (testZoom) mapScreen->setZoom(3);
  }
  else if (trackIndex == 1100)
  {
     if (testZoom) mapScreen->setZoom(2);
  }
  else if (trackIndex == 1100)
  {
     if (testZoom) mapScreen->setZoom(1);
     mapScreen->setDrawAllFeatures(false);
  }

  bool useTrack = true;
  
  if (useTrack)
  {
    mapScreen->drawDiverOnBestFeaturesMapAtCurrentZoom(diveTrack[trackIndex]._la,diveTrack[trackIndex]._lo,diveTrack[trackIndex]._h);
  }
  else
  {
    mapScreen->setTargetWaypointByLabel("Sub"); // Cafe Jetty

    mapScreen->drawDiverOnBestFeaturesMapAtCurrentZoom(pos.la,pos.lo,pos.heading);

    mapScreen->setTargetWaypointByLabel("Sub"); // Cafe Jetty

    const uint16_t headingStep = 5;

    pos.heading+=headingStep;
    pos.heading%=360;
  }
    
  cycleTrackIndex();
  testIteration++;
}

void cycleTrackIndex()
{
  trackIndex = (trackIndex + 1) % trackLength;

  pos.lo += 0.00004;
  pos.la -= 0.00002;
}

#ifdef BUILD_MAPSCREEN_T4

void amoledPushImage(const uint16_t* map,const int width,int height)
{
  amoled.pushColors(0,0,width,height,const_cast<uint16_t*>(map));
}

void amoledDisplayImageWithSprite(const uint16_t* image,int width, int height)
{
  TFT_eSprite* sprite = new TFT_eSprite(&tft);
  sprite->createSprite(width,height);
  sprite->pushImage(0,0,width,height,image);

  amoled.pushColors(0,0,width,height,(uint16_t*)(sprite->getPointer()));
  delete sprite; sprite=nullptr;
}

void amoledTestRotatePhotos()
{
  repeat:
  amoledDisplayImageWithSprite(Model_600x400,600,400);
  delay(3000);
  amoledPushImage(bride_600x342,600,342);
  delay(3000);
  amoledPushImage(Colorful_590x450,590,450);
  delay(3000);
  amoledPushImage(lion_600x400,600,400);
  delay(3000);
  amoledPushImage(visualCheck_410x304,410,304);
  delay(3000);
  mapScreen->drawFeaturesOnSpecifiedMapToScreen(0);
  delay(3000);
  mapScreen->drawFeaturesOnSpecifiedMapToScreen(1);
  delay(3000);
  mapScreen->drawFeaturesOnSpecifiedMapToScreen(2);
  delay(3000);
  mapScreen->drawFeaturesOnSpecifiedMapToScreen(3);
  delay(3000);
  mapScreen->drawFeaturesOnSpecifiedMapToScreen(4);
  delay(3000);
  mapScreen->drawFeaturesOnSpecifiedMapToScreen(5);
  delay(3000);
  mapScreen->drawFeaturesOnSpecifiedMapToScreen(6);
  delay(3000);
  mapScreen->drawFeaturesOnSpecifiedMapToScreen(7);
  delay(3000);
  goto repeat;
}

void amoledTestScroll()
{
  // set entire background to white

  TFT_eSprite* whiteScreen = new TFT_eSprite(&tft);
  whiteScreen->createSprite(450,600);
  whiteScreen->fillSprite(TFT_WHITE);
  amoled.pushColors((uint16_t*)(whiteScreen->getPointer()),450*600);
  delete whiteScreen; whiteScreen=nullptr;

  const int backgroundWidth=370;    // sprite width has to be divisible by 2 or screen will be corrupted
  const int backgroundHeight=255;   // sprite height can be odd or even

  TFT_eSprite* blackBackground = new TFT_eSprite(&tft);
  blackBackground->createSprite(backgroundWidth,backgroundHeight);   

  TFT_eSprite* tile = new TFT_eSprite(&tft);
  tile->createSprite(200,256);
  
  tile->fillCircle(100,100,50,TFT_WHITE);
  tile->fillCircle(100,190,30,TFT_RED);
  tile->drawRect(0,0,199,249,TFT_GREEN);

// To use smooth fonts in sprite - WORKING

  const int labelWidth  = 450, labelHeight = 195;
  TFT_eSprite* label = new TFT_eSprite(&tft);
  label->createSprite(labelWidth,labelHeight);
  label->setTextColor(TFT_YELLOW,TFT_BLACK);
  label->setCursor (0, 0);
//  label->loadFont(Final_Frontier_28);     // use smooth font    -D SMOOTH_FONT=1
// https://www.google.com/get/noto/
//  label->loadFont(NotoSansBold36);     // use smooth font    -D SMOOTH_FONT=1
  label->loadFont(NotoSansMonoSCB20);     // use smooth font    -D SMOOTH_FONT=1

  label->printToSprite(String("AgainTesting"));   // for anti-aliased, smooth fonts

  const int labelXOffset = 30, labelYOffset = 350;
  amoled.setAddrWindow(labelXOffset,labelYOffset,labelXOffset+labelWidth-1,labelYOffset+labelHeight-1);
  amoled.pushColors((uint16_t*)(label->getPointer()),labelWidth * labelHeight);

  const int maxLateralShift=152, minLateralShift=0;

  int lateralShift=minLateralShift;

  int step=1;

  while(true)
  {
    blackBackground->fillSprite(TFT_WHITE);
    tile->pushToSprite(blackBackground,testIteration,0);

    const int spriteOffsetX=30;      // x offset has to be divisible by 2 or screen will be corrupted
    const int spriteOffsetY=89;      // y offset can be odd or even

    amoled.setAddrWindow(spriteOffsetX, spriteOffsetY, backgroundWidth+spriteOffsetX-1, backgroundHeight+spriteOffsetY-1);

    amoled.pushColors((uint16_t*)(blackBackground->getPointer()),backgroundWidth * backgroundHeight);
    
    if (testIteration == maxLateralShift)
      step=-4;
    else if(testIteration == minLateralShift)
      step=4;

    testIteration+=step;
  }
}
#endif

