/* https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
 JF Hybrid of DMX and FastLED

This is the layout of DMX channels
Channel[0] Brightness for the strip
Channel[1] Mode- 255 = More below, controls mode of operations
Channel[2] Speed of the animations
Channel 3 
Channel 4
Channel 5  H1  When using fixed mode, Hue of the 1st section of LEDS
Channel 6  H2  2nd sections
Channel 7  H3   3rd sections


LED Strip Costco, 6leds/chip, 1 section per foot
Section 1.. 9 feet,   0 to 8
Section 2.. 9 feet   9 to 17
Section 3...8 feet   18 to 26


*/




#include <FastLED.h>
#include <DMXSerial.h>

#define LED_PIN     12
#define NUM_LEDS    26
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
//#define UPDATES_PER_SECOND  100
CRGB leds[NUM_LEDS];
#define DMXSTART 81  //First DMX Channel
#define DMXLENGTH 88  // number of DMX channels used

uint8_t Channel[8] = {128,0,0,0,0,0,0,0} ;
const int TestPin = 3; // JF Status LED
uint8_t Speed = 100;


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
extern const uint8_t bright[];

void setup() {
    pinMode(TestPin, OUTPUT);
    analogWrite(TestPin, 255);  // Turn on status led
    delay( 1000 ); // power-up safety delay

    
    DMXSerial.init(DMXProbe);
      DMXSerial.maxChannel(DMXLENGTH);

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


void loop()
{

  // wait for an incomming DMX packet.
  if (DMXSerial.receive()) {
    analogWrite(TestPin,0);
    // Channel 1 Brightness
    if (DMXSerial.read(DMXSTART+0) != Channel[0]) {Channel[0]=DMXSerial.read(DMXSTART+0); FastLED.setBrightness (pgm_read_byte(&bright[Channel[0]]));}
    //Channel 2 MODE
    if (DMXSerial.read(DMXSTART+1) != Channel[1]) {Channel[1]=DMXSerial.read(DMXSTART+1); ChangeShow(Channel[1]);}
    //Channel 3 SPEED
    if (DMXSerial.read(DMXSTART+2) != Channel[2]) {Channel[2] = DMXSerial.read(DMXSTART+2); }
    // Channel 6 On mode 255, R
    if (DMXSerial.read(DMXSTART+5) != Channel[5]) {Channel[5]=DMXSerial.read(DMXSTART+5);}
    //Channel 7 On mode255, G
    if (DMXSerial.read(DMXSTART+6) != Channel[6]) {Channel[6]=DMXSerial.read(DMXSTART+6);}
    //Channel 8 On mod25,B
    if (DMXSerial.read(DMXSTART+7) != Channel[7]) {Channel[7] = DMXSerial.read(DMXSTART+7);}




  } else {
    // don't update the Neopixels but signal a red.
    analogWrite(TestPin, 200);
   
  } // endif

    if (Channel[1]==0)   // If mode is 255, solid colors using RGB on 6-7-8..

      {
        //FastLED.showColor(CHSV(Channel[5],Channel[6],Channel[0]));
        CRGB H1 = CHSV(Channel[5],255,Channel[0]);
        CRGB H2 = CHSV(Channel[6],255,Channel[0]);
        CRGB H3 = CHSV(Channel[7],255,Channel[0]);
        
    for( int i = 0;i< 9; i++) {
        leds[i] = H1;
       }
    for( int i = 9; i<18; i++) {
        leds[i] = H2;
       }
    for( int i = 18;i<27; i++) {
        leds[i] = H3;
       }
        
        FastLED.show();
       // FastLED.delay(10);
      } else {   // if all other modes, use palettes
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
    FastLED.delay(128-(Channel[2]/2));
    
      } //end if

    
} // END LOOP

// ------------------------------------------------------------------------


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//


void ChangeShow(uint8_t channelval)   // called to change show
{
        if( channelval >=  0 and channelval <10)  {  }   //this is for the set colors 3 sections
        if( channelval >=  10 and channelval <20)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( channelval >=  20 and channelval <30)  { currentPalette = OceanColors_p;   currentBlending = LINEARBLEND;  }
        if( channelval >=  30 and channelval <40)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( channelval >=  40 and channelval <50)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( channelval >=  50 and channelval <60)  { SetupBlueAndRedPalette();             currentBlending = LINEARBLEND; }
        if(channelval >=  60 and channelval <70)  { currentPalette = PartyColors_p;              currentBlending = LINEARBLEND; }
        if(channelval >=  70 and channelval <80)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if(channelval >=  80 and channelval <90)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if(channelval >=  90 and channelval <100)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
  
        if(channelval >= 100 ){SetupJfPalette(); currentBlending = NOBLEND;  } 

    
    }



// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlueAndRedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Blue);
    // and set every fourth one to Red
    currentPalette[0] = CRGB::Red;
    currentPalette[4] = CRGB::Red;
    currentPalette[8] = CRGB::Red;
    currentPalette[12] = CRGB::Red;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}
// This function sets up a JF palette
void SetupJfPalette()
{
    CRGB red = CHSV( HUE_RED, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  black, red ,  black,
                                   black,  red,  black, black,
                                   red, black,  black, red,
                                   black, black, red, black );
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

const uint8_t PROGMEM bright[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
