#ifndef Scenario_h
#define Scenario_h

// LCD
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>

static LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// KEYBOARD
#include "OnewireKeypad.h"
char keys[] =   // Define keys' values of Keypad
{
  '1', '2', '3', 'A',
  '4', '5', '6', 'B',
  '7', '8', '9', 'C',
  '*', '0', '#', 'D'
};
int values[] =   // Define keys' values of Keypad
{
  948, 479, 320, 240,
  784, 434, 298, 230,
  668, 396, 275, 218,
  571, 368, 261, 208
};
static OnewireKeypad <Print, 16 > Keypad(Serial, keys, values, 4, 4, A0, 220, 1000);

class Scenario
{
      
  public: 
    enum State{STOP, PLAY, PAUSE};
    Scenario(char* scenarioName) 
    : m_name(scenarioName), m_currentState(State::STOP)
    {
    }
    
    virtual void runScenario() =0;
    void setLCDText(char* aString, int x, int y, bool clearLCD = false, bool defaultTitle = true)
    {
      if (clearLCD)
         lcd.clear();
      if (defaultTitle)
      {
        lcd.setCursor(0, 0);
        lcd.print(getName());
      }
      lcd.setCursor(x, y);
      lcd.print(aString);
    }

    void initLCD()
    {
      lcd.begin(16,2);
    
      for(int i = 0; i< 3; i++) 
      {
        lcd.backlight();
        delay(250);
        lcd.noBacklight();
        delay(250);
      }
      lcd.backlight();
      setLCDText("", 0, 1, true, true);
    }
    
    char* getName() {return m_name;}
    void setCurrentState(State state) { m_currentState = state;}
    State getCurrentState() { return m_currentState;}
  
  protected:
    char* m_name; 
    State m_currentState;
};


#endif
