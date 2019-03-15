#ifndef ScenarioDomination_h
#define ScenarioDomination_h

#include "Scenario.h"

class ScenarioDomination : public Scenario 
{
  public: 
    ScenarioDomination(char* aName) 
    : Scenario(aName)
    {
      Serial.begin(9600); 
    }
    
    virtual void runScenario() override 
    {
    };
  
  private:
    Scenario** m_menuList;
    int m_length, m_currentId;
};


#endif
