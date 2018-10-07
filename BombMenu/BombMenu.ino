#include <Event.h>
#include <Timer.h>

#include "Scenario.h"
#include "Menu.h"
#include "ScenarioTimer.h"

Menu* menu;

void setup()
{
  Scenario* scenarios[3] = 
  {
    new ScenarioTimer("1 - Timer"),
    new ScenarioTimer("2 - Defuse"),
    new ScenarioTimer("3 - Settings")    
  };
  menu = new Menu("CAPS - BOMB", scenarios, 3);
}
  
void loop()
{  
  menu->initLCD();
  menu->setLCDText(menu->getCurrentMenu()->getName(), 0, 1, false, true);
  menu->runScenario();
}


