#ifndef ScenarioTimerPass_h
#define ScenarioTimerPass_h

#include "Scenario.h"
class ScenarioTimerPass;

ScenarioTimerPass* currentInstanceScenarioTimerPass;

static void counterScenarioTimerPass(void*);

class ScenarioTimerPass : public Scenario
{
  public:
    ScenarioTimerPass(char* aName)
      : Scenario(aName), duration(-1), durationAlerte(-1), desamorced(false), delayBeeper(100), delayTimer(1000)
    {
      Serial.begin(9600);
      setCurrentState(State::PLAY);
      Keypad.SetHoldTime(100);  // Key held time in ms
      Keypad.SetDebounceTime(50); // Key Debounce time in ms
      currentInstanceScenarioTimerPass = this;      
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

    void printPass()
    {
      String ps = "Pass: ";
      ps += String(a);
      ps += String("x");
      ps += String(b);
      char copy[16];
      ps.toCharArray(copy, 16);
      setLCDText(copy, 0, 1, false, false);
    }

    void initSettings()
    {
      setCurrentState(State::PLAY);
      randomSeed(analogRead(0));
      a = random(1,20);
      randomSeed(analogRead(1));  
      b = random(1,20);
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
      timerId = timer.every(delayTimer, counterScenarioTimerPass, NULL);
      printPass();
    } 
    
    void readResult()
    {
      if ((Keypad.Key_State() == 3))
      {
        String inString = "";
        setLCDText("Code ?", 0, 0, true, false);
        bool finishCode = false;
        while (!finishCode)
        {
          if ((Keypad.Key_State() == 3))
          {
            char keypress = Keypad.Getkey();
            if (keypress == 'A') {}
            else if (keypress == 'B') {}
            else if (keypress == '*') {}
            else if (keypress == '#') {}
            else if (keypress == 'C')
            {
              finishCode = true;
            }
            else if (keypress == 'D')
            {
              if (inString.length() > 0)
              {
                Serial.println(inString);
                inString = inString.substring(0, inString.length() - 1);
                Serial.println(inString);
              }
            }
            else
            {
              if (isDigit(keypress))
                inString += keypress;
            }
            char copy[16];
            inString.toCharArray(copy, 16);
            setLCDText("Code ?", 0, 0, true, false);
            setLCDText(copy, 0, 1, false, false);
            while ((Keypad.Key_State())) {} // Stay here while Key is held down
          }
        }
        int code = inString.toInt();
        if (code == (a*b))
        {
          succeed();
        } else 
        {
          fail();
        }
      }
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

    void desamorcedLed()
    {
      digitalWrite(desamorcedPin, HIGH);
    }

    void succeed()
    {
      desamorced = true;
      desamorcedLed();
      printDuration();
      setLCDText("SUCCEED", 0, 1, false, false);
      m_currentState = State::STOP;
    }

    void fail()
    {
      duration -= duration/4;
      beeping(buzzerPin, 10, 400);
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
        timerId = timer.every(delayTimer, counterScenarioTimerPass, NULL);
      }
      if (duration == 0)
      {
        timer.stop(timerId);
        digitalWrite(buzzerPin, HIGH);
      }
      printDuration();
      printPass();
    }

    virtual void runScenario() override
    {
      reset();
      while (m_currentState == State::PLAY)
      {
        timer.update();
        readResult();

      }
      while (m_currentState == State::STOP && desamorced) {}
    };

  private:
    int timerId;
    int a, b;
    int duration;
    int durationAlerte;
    bool desamorced;
    int delayBeeper;
    int delayTimer;
    int buzzerPin = 6;
    int desamorcedPin = 7;
    Timer timer;
};

static void counterScenarioTimerPass(void*) { currentInstanceScenarioTimerPass->counterScenarioTimer();}

#endif
