// Include libraries
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <BleMouse.h>

BleMouse bleMouse("saiman motion mouse", "Espressif", 100);

// Gyro variables
MPU6050 mpu;
int16_t gx, gy, gz; // Define gyro output variables
int vx, vy;         // Define pointer displacement

// Touch buttons and LED
const int touchLeftPin = 12;   // Touch button for left click
const int touchRightPin = 4;   // Touch button for right click
const int ledPin = 2;          // LED indicator

// Define rate of refresh
int rate = 10;

// State variables for touch buttons
bool leftButtonPressed = false;
bool rightButtonPressed = false;

// Sensitivity thresholds for touch buttons
const int touchThreshold = 3000;

// setup() initializes I2C communication, MPU-6050 module connection, HID mouse functionality, touch buttons, LED, and serial communication.
void setup()
{
  Wire.begin();
  mpu.initialize();
  bleMouse.begin();
  Serial.begin(115200); // Begin serial communication for reading sensor readings

  pinMode(touchLeftPin, INPUT);
  pinMode(touchRightPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Initially turn off the LED
}

// loop() executes continuously the whole functionality
void loop()
{
  moveMouse();
  checkButtons();
  printValues();
  delay(rate);
}

// moveMouse() gets rotational values from the gyro sensor and feeds them into the variable assigned.
// Then, it converts the output values into pointer displacement values in the form of x and y displacement coordinates.
// In the end, it calls the Mouse.move() function, which moves according to the displacement values calculated.
void moveMouse()
{
  mpu.getRotation(&gx, &gy, &gz); // Get rotation values from gyroscope

  // Convert output values to pointer displacement
  vx = -(gz + 10) / 500; // Adjust sensitivity as needed
  vy = -(gy + 50) / 250;  // Adjust sensitivity as needed

  bleMouse.move(vx, vy); // Move mouse according to gyro rotation
}

void checkButtons()
{
  if (analogRead(touchLeftPin) > touchThreshold) {
    if (!leftButtonPressed) {
      bleMouse.press(MOUSE_LEFT);
      digitalWrite(ledPin, HIGH); // Turn on LED
      leftButtonPressed = true;
    }
  } else {
    if (leftButtonPressed) {
      bleMouse.release(MOUSE_LEFT);
      digitalWrite(ledPin, LOW); // Turn off LED
      leftButtonPressed = false;
    }
  }

  if (analogRead(touchRightPin) > touchThreshold) {
    if (!rightButtonPressed) {
      bleMouse.press(MOUSE_RIGHT);
      digitalWrite(ledPin, HIGH); // Turn on LED
      rightButtonPressed = true;
    }
  } else {
    if (rightButtonPressed) {
      bleMouse.release(MOUSE_RIGHT);
      digitalWrite(ledPin, LOW); // Turn off LED
      rightButtonPressed = false;
    }
  }
}

void printValues()
{
  Serial.print("\n");
  Serial.print("Gyroscope values - gx=");
  Serial.print(gx);
  Serial.print(" gy=");
  Serial.print(gy);
  Serial.print(" gz=");
  Serial.print(gz);
  Serial.print("\n");

  Serial.print("X and Y coordinate values-");
  Serial.print(" vx=");
  Serial.print(vx);
  Serial.print(" vy=");
  Serial.print(vy);
  Serial.print("\n");
}
