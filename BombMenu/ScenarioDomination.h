#ifndef ScenarioDomination_h
#define ScenarioDomination_h

#include "Scenario.h"
class ScenarioDomination;

ScenarioDomination* currentInstanceScenarioDomination;

static void counterScenarioDomination(void*);

class ScenarioDomination : public Scenario 
{
  public: 
    ScenarioDomination(char* aName) 
      : Scenario(aName), duration(-1), durationAlerte(-1), delayBeeper(100), delayTimer(1000)
    {
      Serial.begin(9600);
      currentInstanceScenarioDomination = this;     
      Keypad.SetHoldTime(100);  // Key held time in ms
      Keypad.SetDebounceTime(50); // Key Debounce time in ms 
      reset();
    }

    void reset()
    {
      setCurrentState(State::PLAY);
      pinMode(buzzerPin, OUTPUT);
      delayBeeper = 100;
      delayTimer = 1000;
      scoreListSize = 0;
      digitalWrite(buzzerPin, LOW);
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

    void printScore()
    {
      String ps = "SCORE";
      char copy[16];
      ps.toCharArray(copy, 16);
      setLCDText(copy, 0, 1, false, false);
    }

    void initSettingsTime()
    {
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
          else if (keypress == 'D') {}
          else if (keypress == 'C')
          {
            finishInit = true;
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
      timerId = timer.every(delayTimer, counterScenarioDomination, NULL);
    }

    void initSettingsTeams()
    {
      setLCDText("Nb team", 0, 0, true, false);
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
      scoreListSize = inString.toInt();
    }

    void initSettingsDelay()
    {
      setLCDText("Delay in sec", 0, 0, true, false);
      bool finishInit = false;
      String inString = "";
      while (!finishInit)
      {
        if ((Keypad.Key_State() == 3))
        {
          char keypress = Keypad.Getkey();
          if (keypress == 'A') {}
          else if (keypress == 'B') {}
          else if (keypress == 'D') {}
          else if (keypress == 'C')
          {
            finishInit = true;
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
      scoreListDelay = inString.toInt();
    }

    void initSettings()
    {
      setCurrentState(State::PLAY);
      do {
        initSettingsTime();
      } while (duration <= 0 || duration > 600);
      do {
        initSettingsTeams();      
      } while (scoreListSize <= 0 || scoreListSize > 9);
      do {
        initSettingsDelay();      
      } while (scoreListDelay <= 0);
      scoreList = new int[scoreListSize];
      scoreListTimer = new int[scoreListSize];
      for (int i = 0; i < scoreListSize; i++)
      {
        scoreList[i] = 0;
        scoreListTimer[i] = 0;
      }
    } 
    
    void readKey()
    {
      if ((Keypad.Key_State() == 3))
      {
        char keypress = Keypad.Getkey();
        if (keypress == 'A') {}
        else if (keypress == 'B') {}
        else if (keypress == '*') {}
        else if (keypress == '#') {}
        else if (keypress == 'C') {}
        else if (keypress == 'D') {}
        else
        {
          String inString;
          if (isDigit(keypress))
          {
            inString += keypress;
            if (inString.toInt() <= scoreListSize)
            {
              if (scoreListTimer[inString.toInt()-1] == 0)
              {
                scoreList[inString.toInt()-1] = scoreList[inString.toInt()-1]+1;
                scoreListTimer[inString.toInt()-1] = scoreListDelay;
              }
            }
          }
        }
      }
    }

    void startScoreListTimer(int i)
    {
      if (duration < durationAlerte || durationAlerte == -1)
        scoreListTimer[i] = scoreListDelay *2;
      else 
        scoreListTimer[i] = scoreListDelay;
    }

    void counterScenarioTimer()
    { 
      Serial.println("SCORE --- ");
      for (int i = 0; i < scoreListSize; i++)
      {
        Serial.print(i+1);
        Serial.print(" - ");
        Serial.println(scoreList[i]);
      }
      Serial.println("DELAY --- ");
      for (int i = 0; i < scoreListSize; i++)
      {
        Serial.print(i+1);
        Serial.print(" - ");
        Serial.println(scoreListTimer[i]);
      }
      beeping(buzzerPin, 10, delayBeeper);
      duration = duration > 0 ? duration - 1 : duration;
      for (int i = 0; i < scoreListSize; i++)
        if (scoreListTimer[i] > 0)
          scoreListTimer[i] = scoreListTimer[i]-1;
      if (duration < durationAlerte )
      {
        for (int i = 0; i < scoreListSize; i++)
          if (scoreListTimer[i] > 0)
            scoreListTimer[i] = scoreListTimer[i]*2;
        duration = duration * 2;
        delayTimer = delayTimer / 2;
        delayBeeper = delayBeeper / 2;
        durationAlerte = -1;
        timer.stop(timerId);
        timerId = timer.every(delayTimer, counterScenarioDomination, NULL);
      }
      if (duration == 0)
      {
        timer.stop(timerId);
        digitalWrite(buzzerPin, HIGH);
      }
      printDuration();
      printScore();
    }

    virtual void runScenario() override
    {
      reset();
      initSettings();
      while (m_currentState == State::PLAY)
      {
        timer.update();
        readKey();
      }
      
      while (m_currentState == State::STOP) {}
    };

  private:
    int timerId;
    int duration;
    int durationAlerte;
    int delayBeeper;
    int delayTimer;
    int buzzerPin = 6;
    Timer timer;
    int *scoreList;
    int *scoreListTimer;
    int scoreListDelay;
    int scoreListSize;
};

static void counterScenarioDomination(void*) { currentInstanceScenarioDomination->counterScenarioTimer();}

#endif
