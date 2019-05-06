#include <Event.h>
#include <Timer.h>

#include "Scenario.h"
#include "Menu.h"
#include "ScenarioTimer.h"
#include "ScenarioTimerPass.h"
#include "ScenarioDomination.h"

Menu* menu;

void setup()
{
  char* menuName = (char*) malloc(strlen("CAPS - BOMB")+1); // +1 for the terminator
  char* scenarioName1 = (char*) malloc(strlen("1 - Timer")+1); // +1 for the terminator
  char* scenarioName2 = (char*) malloc(strlen("2 - Timer Pass")+1); // +1 for the terminator
  char* scenarioName3 = (char*) malloc(strlen("3 - Domination")+1); // +1 for the terminator
  
  strcpy(menuName, "CAPS - BOMB");
  strcpy(scenarioName1, "1 - Timer");
  strcpy(scenarioName2, "2 - Defuse");
  strcpy(scenarioName3, "3 - Domination");
  
  Scenario* scenarios[3] = 
  {
    new ScenarioTimer(scenarioName1),
    new ScenarioTimerPass(scenarioName2),
    new ScenarioDomination(scenarioName3)
  };
  
  menu = new Menu(menuName, scenarios, 3);
}
  
void loop()
{  
  menu->initLCD();
  menu->setLCDText(menu->getCurrentMenu()->getName(), 0, 1, false, true);
  menu->runScenario();
}
