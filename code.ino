/*THIS CODE WAS WRITTEN BY DEVASHISH LAHARIYA [https://github.com/devashishlahariya9/SIMPLE-PEDOMETER-USING-ARDUINO]
 *THIS CODE USES BASIC GYRO AND ACCELEROMETER READINGS FROM AN MPU-6050 IMU MODULE.
 *THANK YOU FOR DOWNLOADING THE CODE.
 *NOTE THIS CODE WILL NOT STORE ANY VALUES IF THE ARDUINO POWER IS DISCONNECTED.
 *ONCE THE POWER IS DISCONNECTED, ALL THE VALUES WILL BE RESETED TO ZERO.
 *
 *CONNECTIONS:  ONLY FOR ARDUINO NANO/UNO
 *OLED -> ARDUINO
 *
 *SCL   -> A5
 *SDA  -> A4
 *
 *BATTERY -> ARDUINO  (MAKE SURE TO USE A 3.7V LIPO BATTERY)
 *
 *+VE        -> 5V PIN
 *-VE         -> GND PIN
 *+VE       -> A0
 *
 *MPU-6050 -> ARDUINO
 *
 *SCL   -> A5
 *SDA  -> A4
 */

#include <Adafruit_MPU6050.h>  //MPU-6050 LIBRARY
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>  //LIBRARY FOR OLED SCREEN
#include <Adafruit_SSD1306.h>  //OLED DRIVER LIBRARY
#include <Wire.h>  //FOR I2C COMMUNICATON

#define SCREEN_WIDTH 128  //SCREEN WIDTH
#define SCREEN_HEIGHT 32   //SCREEN HIEGHT. SET THIS TO 64 IF USING 0.91" 128x64 OLED.

#define OLED_RESET 4  //NO NEED TO CONNECT THIS PIN FOR I2C OLED SCREEN WITH ONLY 4 PINS
Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,OLED_RESET);  //CONSTRUTOR FOR OLED

Adafruit_MPU6050 mpu;  //CONSTRUCTOR FOR MPU-6050

//SOME INTEGERS AND FLOAT VALUES TO STORE CALCULATIONS AND DATA

int steps=0;
int calories=0;
int t;
int i=10;
int batteryPercentage;
float distance=0.00;
bool checkStep=false;
bool displayOn=false;

int oled_address=0x3C;  //SET YOUR OLED ADDRESS HERE

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC,oled_address); 
  display.clearDisplay();
  display.setTextSize(1);
  display.setRotation(1);
  //display.dim(true);  //UNCOMMENT THIS IF YOU WANT TO SET BRIGHTNESS TO 50%
  
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
}

void stepCursor()  //FUNCTION FOR SETTING THE CURSOR FOR NUMBER FO STEPS
{
   if(steps < 10)
  {
    display.setCursor(14,15);
  }

  if(steps >= 10 and steps < 100)
  {
    display.setCursor(10,15);
  }

  if(steps >= 100 and steps < 1000)
  {
    display.setCursor(5,15);
  }

  if(steps >= 1000 and steps < 10000)
  {
    display.setCursor(0,15);
  }
}

void calCursor()  //FUNCTION TO SET THE CURSOR FOR CALORIES
{
  if(calories < 10)
  {
    display.setCursor(14,43);
  }

  if(calories >= 10 and calories < 100)
  {
    display.setCursor(10,43);
  }

  if(calories >= 100 and calories < 1000)
  {
    display.setCursor(5,43);
  }

  if(calories >= 1000 and calories < 10000)
  {
    display.setCursor(0,43);
  }
}

void batteryCursor()  //FUNCTION TO SET THE CURSOR FOR BATTERY VALUES
{
  if(batteryPercentage < 10)
  {
    display.setCursor(i,109);
  }

  if(batteryPercentage >= 10 and batteryPercentage < 100)
  {
    display.setCursor(i-4,109);
  }

  if(batteryPercentage == 100)
  {
    display.setCursor(i-8,109);
  }
}

void displayGraphics()  //THIS FUNCTION CONTAINS ALL THE DATA TO BE DISPLAYED ON THE OLED
{
  display.clearDisplay();
  display.setCursor(1,0);
  display.print("STEPS");
  display.drawLine(0,11,32,11,WHITE);
  stepCursor();
  display.print(steps);
  display.drawLine(0,25,32,25,WHITE);
  display.setCursor(8,29);
  display.print("CAL");
  display.drawLine(0,39,32,39,WHITE);
  calCursor();
  display.print(calories);
  display.drawLine(0,53,32,53,WHITE);
  display.setCursor(4,57);
  display.print("DIST");
  display.drawLine(0,67,32,67,WHITE);
  display.setCursor(4,71);
  display.print(distance);
  display.setCursor(11,81);
  display.print("KM");
  display.drawLine(0,91,32,91,WHITE);
  display.setCursor(5,95);
  display.print(t);
  display.setCursor(17,92);
  display.print("o");
  display.setCursor(23,95);
  display.print("C");
  display.drawLine(0,105,32,105,WHITE);
  batteryCursor();
  display.print(batteryPercentage);
  display.setCursor(25,109);
  display.print("%");
  display.display();  //THIS LINE IS VERY IMPORTANT FOR THE OLED TO WORK!
}

void wakeUp()  //THIS FUNCTION IS FOR THE FEATURE THAT TURNS ON THE OLED WHEN THE USER GIVES A LITTLE JERK TO THE DEVICE
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
   if(g.gyro.y < -3 and a.acceleration.x >= 2.5)
   {
    displayOn=true;
   }
   
   if(displayOn == true)
   {
    displayGraphics();
    delay(10000);  //YOU CAN SET THE NUMBER OF SECONDS FOR WHICH THE SCREEN SHOULD STAY ON
    display.clearDisplay();
    displayOn=false;
   }

   else
   {
    display.clearDisplay();
   }
}

void countSteps()  //THIS FUNCTION COUNTS THE NUMBER OF STEPS
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  while(a.acceleration.y >=6 and g.gyro.z <= -0.3 and checkStep == false)
  {
     steps+=1;
     checkStep=true;
  }

  if(a.acceleration.y >=6 and g.gyro.z >= -0.3)
  {
    checkStep=false;
  }
}

void calculateTemp()  //THIS FUNCTION CALCULATES THE TEMPERATURE USING THE BUILT-IN TEMPERATURE SENSOR OF THE MPU-6050
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  t=temp.temperature;
}

void calculations()  //THIS FUNTIONS CONTAINS ALL THE FORMULAS FOR THE CALORIE, DISTANCE AND BATTERY PERCENTAGE CALCULATIONS
{
  distance = (steps*0.70)/1000;  //SET THE 0.70 ACCORDING TO YOUR STEP LENGTH IN METERS 
  calories = (steps*0.04);
  
  float value = analogRead(A0);  //THE BATTERY POSTIVE SHOULD BE CONNECTED TO ANALOG PIN "0". NOTE: MAKE SURE THAT THE VOLTAGE IS 3.7V
  batteryPercentage = map(value,573,756,0,100);

  if(batteryPercentage >= 100)
  {
    batteryPercentage=100;
  }

  if(batteryPercentage <= 0)
  {
    batteryPercentage=0;
  }
}

void loop()
{
  display.setTextColor(SSD1306_WHITE);  //SET THE DISPLAY COLOUR AND CALL ALL THE FUNCTIONS
  countSteps();
  calculations();
  calculateTemp();
  wakeUp();
  display.display();
}
