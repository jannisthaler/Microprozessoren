
#include <Wire.h>
#include <Zumo32U4.h>

const uint16_t maxSpeed = 300;

Zumo32U4LineSensors sensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4LCD lcd;

int16_t lastError = 0;

#define NUM_SENSORS 5
unsigned int lineSensorValues[NUM_SENSORS];

void loadCustomCharacters()
{
  static const char levels[] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 63, 63, 63, 63, 63, 63, 63
  };
  lcd.loadCustomCharacter(levels + 0, 0);  
  lcd.loadCustomCharacter(levels + 1, 1);  
  lcd.loadCustomCharacter(levels + 2, 2);  
  lcd.loadCustomCharacter(levels + 3, 3);  
  lcd.loadCustomCharacter(levels + 4, 4);  
  lcd.loadCustomCharacter(levels + 5, 5);  
  lcd.loadCustomCharacter(levels + 6, 6);  
}

void printBar(uint8_t height)
{
  if (height > 8) { height = 8; }
  const char barChars[] = {' ', 0, 1, 2, 3, 4, 5, 6, 255};
  lcd.print(barChars[height]);
}

void calibrateSensors()
{
  lcd.clear();

  delay(1000);
  for(uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    else
    {
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}

void showReadings()
{
  lcd.clear();

  while(!buttonA.getSingleDebouncedPress())
  {
    sensors.readCalibrated(lineSensorValues);

    lcd.gotoXY(0, 0);
    for (uint8_t i = 0; i < NUM_SENSORS; i++)
    {
      uint8_t barHeight = map(lineSensorValues[i], 0, 1000, 0, 8);
      printBar(barHeight);
    }
  }
}

void setup()
{

  sensors.initFiveSensors();

  loadCustomCharacters();

  // Wartet bis button A gedrückt wurde.
  lcd.clear();
  lcd.print(F("Press A"));
  lcd.gotoXY(0, 1);
  lcd.print(F("to calib"));
  buttonA.waitForButton();

  calibrateSensors();

  showReadings();

  // Screen gibt Go aus
  lcd.clear();
  lcd.print(F("Go!"));
}

void loop()
{
  
  //Get linien Position
  int16_t position = sensors.readLine(lineSensorValues);

  // Wie weit sind wir von der mitte der linie entfernt? default
  int16_t error = position - 2000;

  //Motoren geschwindigkeits unterschied
  int16_t speedDifference = error / 4 + 6 * (error - lastError);

  lastError = error;

  //Get Motorgeschwindigkeit
  int16_t leftSpeed = (int16_t)maxSpeed + speedDifference;
  int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;

  //Schränkt die geschwindigkeit ein.
  leftSpeed = constrain(leftSpeed, 0, (int16_t)maxSpeed);
  rightSpeed = constrain(rightSpeed, 0, (int16_t)maxSpeed);

  motors.setSpeeds(leftSpeed, rightSpeed);
}
