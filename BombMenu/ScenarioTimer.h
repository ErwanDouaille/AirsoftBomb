#ifndef ScenarioTimer_h
#define ScenarioTimer_h

#include "Scenario.h"
class ScenarioTimer;

ScenarioTimer* currentInstance;

static void counter(void*);

class ScenarioTimer : public Scenario
{
  public:
    ScenarioTimer(char* aName)
      : Scenario(aName), duration(-1), durationAlerte(-1), desamorced(false), delayBeeper(100), delayTimer(1000)
    {
      Serial.begin(9600);
      setCurrentState(State::PLAY);
      Keypad.SetHoldTime(100);  // Key held time in ms
      Keypad.SetDebounceTime(50); // Key Debounce time in ms
      currentInstance = this;      
    }

    void initSettings()
    {
      setCurrentState(State::PLAY);
      setLCDText("Duree en minutes", 0, 0, true, false);
      bool finishInit = false;
      String inString = "";
      while (!finishInit)
      {
        if ((Keypad.Key_State() == 3))
        {
          char keypress = Keypad.Getkey();
          if (keypress == 'A') {}
          else if (keypress == 'B') {}
          else if (keypress == 'C')
          {
            if (inString.length() != 0)
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
      timerId = timer.every(delayTimer, counter, NULL);
    }

    void reset()
    {
      pinMode(buzzerPin, OUTPUT);
      pinMode(desamorcedPin, INPUT);
      delayBeeper = 100;
      delayTimer = 1000;
      desamorced = false;
      digitalWrite(buzzerPin, LOW);
      initSettings();
    }

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
      setLCDText(timestr, 0, 0, true, false);
    }

    void counterScenarioTimer()
    {
      beeping(buzzerPin, 10, delayBeeper);
      duration = duration > 0 ? duration - 1 : duration;
      if (duration < durationAlerte )
      {
        duration = duration * 2;
        delayTimer = delayTimer / 2;
        delayBeeper = delayBeeper / 2;
        durationAlerte = -1;
        timer.stop(timerId);
        timerId = timer.every(delayTimer, counter, NULL);
      }
      if (duration == 0)
      {
        timer.stop(timerId);
        digitalWrite(buzzerPin, HIGH);
      }
      printDuration();
    }

    virtual void runScenario() override
    {
      reset();
      while (m_currentState == State::PLAY)
      {
        timer.update();
        desamorcedValue = digitalRead(desamorcedPin);   // read the input pin
        if (desamorcedValue == HIGH)
          succeed();
      }
      while (m_currentState == State::STOP && desamorced) {}
    };

    void succeed()
    {
      desamorced = true;
      printDuration();
      setLCDText("SUCCEED", 0, 1, false, false);
      m_currentState = State::STOP;
    }

  private:
    int timerId;
    int duration;
    int durationAlerte;
    bool desamorced;
    int delayBeeper;
    int delayTimer;
    int buzzerPin = 6;
    int desamorcedPin = 7;
    int desamorcedValue;
    Timer timer;
};

static void counter(void*) { currentInstance->counterScenarioTimer();}

#endif
