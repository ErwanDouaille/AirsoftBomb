#ifndef Menu_h
#define Menu_h

#include "Scenario.h"

class Menu : public Scenario 
{
  public: 
    Menu(char* aName, Scenario* menuList[], int menuLength) 
    : m_menuList(menuList), m_length(menuLength-1), m_currentId(0), Scenario(aName)
    {
      Serial.begin(9600); 
      setCurrentState(State::PLAY);
      Keypad.SetHoldTime(100);  // Key held time in ms
      Keypad.SetDebounceTime(50); // Key Debounce time in ms
    }
    
    void down() { m_currentId = (m_currentId+1) > m_length ? 0 : m_currentId + 1;}
    void up() {m_currentId = (m_currentId-1) < 0 ? m_length : m_currentId - 1;}
    void select() {getCurrentMenu()->runScenario();}
    
    Scenario* getCurrentMenu() {return m_menuList[m_currentId];}
    
    virtual void runScenario() override 
    {
      while (m_currentState == State::PLAY)
      {
        if ((Keypad.Key_State() == 3))
        {
          char keypress = Keypad.Getkey();
          Serial.print("\nValue ");
          Serial.println(keypress);
          if (keypress == 'A')
          {
            up();
          }
          else if (keypress == 'B')
          {
            down();
          }
          else if (keypress == 'C')
          {
            select();
          }
          
          setLCDText(getCurrentMenu()->getName(), 0, 1, true, true);
          setLCDText(getCurrentMenu()->getName(), 0, 1, true, true);
          while ((Keypad.Key_State())) {} // Stay here while Key is held down
        }
      }
    };
  
  private:
    Scenario** m_menuList;
    int m_length, m_currentId;
};


#endif
