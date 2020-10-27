/**************************
 * Written by Elliott Gear for University of Vermont Senior Design
 * October 2020
 * SEED Team #9: WiFi Enhanced Electronic Scale Project
 * https://www.uvm.edu/cems/seed
 * egear@uvm.edu
 * MIT license, all text above must be included in any redistribution
 ***************************/

 /*****
  * Moving average code for Sprint #2 of Fall 2020
  *****/

// Import libraries
#include "HX711.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

// Set with Connor on 10/26/2020
#define calibration_factor -10730.00

// Load cell pins
#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2

// LCD pins
#define TFT_CS        10
#define TFT_RST        9 
#define TFT_DC         8

// Create the scale
HX711 scale;

// create the screen
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


void setup() {
  Serial.begin(9600);
  Serial.println("Prototype 2 scale demo");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");

  // fills screen black
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  
}


void loop() {
  float weight = getAverages();
  //print to console
  Serial.print("Reading: ");
  Serial.print(weight, 1); //scale.get_units() returns a float
  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();

  // print to screen
  tft.setCursor(0,0);
  tft.print(weight, 6);

  delay(1000);
}

// super janky implimentation of moving (lol not actually) average. i miss C++ :(
long getAverages(){

  //WRITE HOW MANY YOU ARE DOING HERE SO IT IS NOT HARDCODED GRRRR
  int howMany = 20; 
  
  // 20 seems kinda good but idk
  long weights[howMany];

  // add 20 weights into the array
  for(int i = 0; i < howMany; i++){
    weights[i] = scale.get_units();
  }

  // find the average, i hate arrays
  long sum, average;
  for(int i = 0; i < howMany; i++){
    sum = sum + weights[i];
  }
  
  average = sum/howMany;
  return average;
  
}
