#include <Adafruit_NeoPixel.h>

/** First Pot (Red) */
#define POT1_PIN 0

/** Second Pot (Green) */
#define POT2_PIN 2

/** Third Pot (Blue) */
#define POT3_PIN 4

/** Pushbutton to cycle animations */
#define MOMENTARY_PIN 5

/** Info to strip */
#define PIXEL_PIN 6

/** Number of LEDs on strip */
#define PIXELS 30

/** Maximum value from Pot */
#define POT_MAX 1023.0

/** Initializing strip object */
Adafruit_NeoPixel strip = Adafruit_NeoPixel( PIXELS, PIXEL_PIN );

/** Old State for reading pushbutton */
int oldState = HIGH;

/** Value for switch case */
int style = 0;

/** Color to be read */
uint32_t c1;

void setup() {
  pinMode( MOMENTARY_PIN, INPUT_PULLUP );
  strip.begin();
  strip.show();
  c1 = strip.Color( 255, 255, 255 );
}

void loop() {
  //Read from Pots
  c1 = getStripColor(); 

  //Current state of push button
  int momState = digitalRead( MOMENTARY_PIN );

  //Seeing if button was pushed
  if ( momState == LOW && oldState == HIGH ) {
    delay( 20 );
    momState = digitalRead( MOMENTARY_PIN );
    if ( momState == LOW ) {
      //Change style number
      style++;
      //clearStrip();
      if ( style > 3 ) {
        style = 0;
      }
      //Chnage style
      styleSwitch( style, c1 );
    }
  } else {
    //Keep same style
    styleSwitch( style, c1 );
  }
  //Update old state
  oldState = momState;
  
}

/**
 * Used to read potentiometers to change color
 * 
 * @return updated color
 */
uint32_t getStripColor() {
  int r = 0;
  int g = 0;
  int b = 0;

  double red = 0;
  double green = 0;
  double blue = 0;
  
  //Use Pot value as a percent and multiply by max color val (255)
  red = 255.0 * ( analogRead( POT1_PIN ) / POT_MAX );
  green = 255 * ( analogRead( POT2_PIN ) / POT_MAX );
  blue = 255 * ( analogRead( POT3_PIN ) / POT_MAX );
  //Cast doubles to ints
  r = (int)red;
  g = (int)green;
  b = (int)blue;

  return strip.Color( r, g, b );
}

/**
 * Call animations based on style val
 * 
 * @param
 *      Stlye val
 * @param
 *      Color read from pots
 * @param
 *      White
 */
void styleSwitch( int s, uint32_t c1 ) {
  switch(s) {
    case 0: 
            white( c1 );
            break;
    case 1: 
            solid ( c1 );
            break;
    case 2: 
            middle( c1 );
            break;
    case 3: 
            fill( c1 );
            break;
  }
}

/**
 * Set strip colors to 0
 */
void clearStrip() {
  for ( int i = 0; i < PIXELS; i++ ) {
    strip.setPixelColor(i, 0);
  }
}

/**
 * Alternates filling strip with Pot color
 * and white.
 * 
 * @param
 *      Color from pots
 */
void white( uint32_t c1 ) {
  uint8_t wait = 30;
  //Fill with Pot color
  for ( int i = 0; i < PIXELS; i++ ) {
    strip.setPixelColor( i, c1 );
    strip.show();
    delay( wait );
  }
  //Fill with white
  for ( int i = 0; i < PIXELS; i++ ) {
    strip.setPixelColor( i, strip.Color( 255, 255, 255 ) );
    strip.show();
    delay( wait );
  }
}

/**
 * Strip shows a solid color, no effects
 * 
 * @param
 *      Color from pots
 */
void solid( uint32_t c ) {
  for ( int i = 0; i < PIXELS; i++ ) {
    strip.setPixelColor( i, c );
  }
  strip.show();
}

/**
 * Color starts at middle of strip and fans out and
 * then back in.
 * 
 * @param
 *      Color from pots
 */
void middle( uint32_t c1 ) {
  uint8_t wait = 50;
  //Start and middle and fan out in both directions
  for ( int i = 14; i >= 0; i-- ) {
    strip.setPixelColor( i, c1 );
    strip.setPixelColor( 15 + ( 14 - i ), c1 );
    strip.show();
    delay( wait );
  }
  //Turn pixels off, starting at each end of the strip
  for ( int i = 0; i < 15; i++ ) {
    strip.setPixelColor( i, 0 );
    strip.setPixelColor( 29 - i, 0 );
    strip.show();
    delay( wait );
  }
}

/**
 * Fills one pixel at a time from end to end
 * 
 * @param
 *      Color from pots
 */
void fill( uint32_t c1 ) {
  uint8_t wait = 65;
  //Set pixel to color values
  for ( int i = 0; i < PIXELS; i++ ) {
    strip.setPixelColor( i, c1 );
    strip.show();
    delay(wait);
  }
  //Clear pixels
  for ( int i = 0; i < PIXELS; i++ ) {
    strip.setPixelColor( i, 0 );
    strip.show();
    delay(wait);
  }
}

