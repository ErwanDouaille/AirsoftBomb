#ifndef ScenarioTimer_h
#define ScenarioTimer_h

#include "Scenario.h"

class ScenarioTimer : public Scenario 
{
  public: 
    ScenarioTimer(char* aName) 
    : Scenario(aName)
    {
    }
        
    virtual void runScenario() override 
    {
      while (m_currentState == State::PLAY)
      {
        setLCDText(getName(), 0, 1, true, true);
      }
    };
  
};


#endif
