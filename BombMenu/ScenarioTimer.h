#ifndef ScenarioTimer_h
#define ScenarioTimer_h

#include "Scenario.h"
class ScenarioTimer;
class ScenarioTimer;
Timer timer;
ScenarioTimer* scenarioTimer;
int timerId;
int duration = -1;
int durationAlerte = -1;
int delayBeeper = 100;
int delayTimer = 1000;
int buzzerPin = 2;
int desamorcedPin = 3;
bool desamorced = false;

void beeping(int minDelay, int maxDelay)
{
    digitalWrite(buzzerPin, HIGH);
    delay(maxDelay);
    digitalWrite(buzzerPin, LOW);
    delay(minDelay);
}

void desamorcedLed()
{
    digitalWrite(desamorcedPin, HIGH);
}
    
void counter();

class ScenarioTimer : public Scenario 
{
  public: 
    ScenarioTimer(char* aName) 
    : Scenario(aName)
    {
      Serial.begin(9600); 
      setCurrentState(State::PLAY);
      Keypad.SetHoldTime(100);  // Key held time in ms
      Keypad.SetDebounceTime(50); // Key Debounce time in ms
      scenarioTimer = this;
    }
    
    void initSettings()
    {
      setCurrentState(State::PLAY);
      scenarioTimer->setLCDText("Duree en minutes", 0, 0, true, false);
      bool finishInit = false;
      String inString = "";
      while (!finishInit)
      {
        if ((Keypad.Key_State() == 3))
        {
          char keypress = Keypad.Getkey();
          if (keypress == 'A'){}
          else if (keypress == 'B'){}
          else if (keypress == 'C')
          {
            finishInit = true;
          }
          else if (keypress == 'D')
          {
            setCurrentState(State::STOP);
            return;
          }
          else 
          {
            if (isDigit(keypress))
              inString += keypress;
          }
          char copy[16];
          inString.toCharArray(copy, 16);
          setLCDText(copy, 0, 1, false, false);
          while ((Keypad.Key_State())) {} // Stay here while Key is held down
        }
      }
      duration = inString.toInt() * 60;
      durationAlerte = duration / 3;
      timerId = timer.every(delayTimer, counter, -1);
    }

    void reset()
    {      
      pinMode(buzzerPin, OUTPUT);
      delayBeeper = 100;
      delayTimer = 1000;
      desamorced = false;
      digitalWrite(buzzerPin, LOW);
      digitalWrite(desamorcedPin, LOW);
      initSettings();
    }    
    
    void down() { m_currentId = (m_currentId+1) > m_length ? 0 : m_currentId + 1;}
    void up() {m_currentId = (m_currentId-1) < 0 ? m_length : m_currentId - 1;}
    Scenario* select() {return m_menuList[m_currentId];}
    void back() {setCurrentState(State::STOP);}
    

    
    virtual void runScenario() override 
    {
      reset();
      while (m_currentState == State::PLAY)
      {
        timer.update();

        if (desamorced)
          desamorcedLed();
      }
    };
  
  private:
    Scenario** m_menuList;
    int m_length, m_currentId;
};

void printDuration()
{
  int n = durationAlerte == -1 ? duration / 2 : duration;
  int hour = n / 3600;   
  n %= 3600; 
  int minutes = n / 60 ; 
  n %= 60; 
  int seconds = n; 
  
  char timestr[16];
  timestr[0] = 'T';
  timestr[1] = 'I';
  timestr[2] = 'M';
  timestr[3] = 'E';
  timestr[4] = 'R';
  timestr[5] = ':';
  timestr[6] = ' ';
  timestr[7] = '0' + hour / 10;
  timestr[8] = '0' + hour % 10;
  timestr[9] = ':';
  timestr[10] = '0' + minutes / 10;
  timestr[11] = '0' + minutes % 10;
  timestr[12] = ':';
  timestr[13] = '0' + seconds / 10;
  timestr[14] = '0' + seconds % 10;
  timestr[15] = '\0';
  scenarioTimer->setLCDText(timestr, 0, 0, true, false);
}
    
void counter()
{
  beeping(10, delayBeeper);
  duration = duration > 0 ? duration-1 : duration;
  if (duration < durationAlerte )
  {
    duration = duration *2;
    delayTimer = delayTimer/2;
    delayBeeper = delayBeeper/2;
    durationAlerte = -1;
    timer.stop(timerId);
    timerId = timer.every(delayTimer, counter, -1);
  }
  if (duration == 0)
  {
    timer.stop(timerId);
    digitalWrite(buzzerPin, HIGH);
  }   
  printDuration();
}

#endif
