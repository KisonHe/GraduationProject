#pragma once

#include <vector>
class Subject;

class Observer
{
public:
   Observer() {}
   virtual ~Observer() {}
   virtual void update(Subject *subject) = 0;
};

class Subject
{
public:
   Subject() {}
   virtual ~Subject() {}
   void attach(Observer &observer)
   {
      m_observers.push_back(&observer);
   }
   void notify()
   {
      if (!m_observers.empty())
      {
         std::vector<Observer *>::iterator it;
         for (it = m_observers.begin(); it != m_observers.end(); it++)
            (*it)->update(this);
      }
   }

private:
   std::vector<Observer *> m_observers;
};
