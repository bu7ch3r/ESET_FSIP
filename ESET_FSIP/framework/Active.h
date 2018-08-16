#ifndef ACTIVE_H
#define ACTIVE_H

#include "tsqueue.h"
using namespace std;

class Active {
public:
  typedef std::function<void()> Message;

private:

  Active( const Active& );           // no copying
  void operator=( const Active& );    // no copying

  bool done;                         // le flag
  tsqueue<Message> mq;        // le queue
  unique_ptr<thread> thd;          // le thread

  void Run() {
    while( !done ) {
      Message msg = mq.pop();
      msg();            // execute message
    } // note: last message sets done to true the one from destructor
  }

public:

  Active() : done(false) {
    thd = unique_ptr<thread>(
                  new thread( [=]{ this->Run(); } ) );
  }

  ~Active() {
    Send( [&]{ done = true; } ); ;
    thd->join();
  }

  void Send( Message m ) {
    mq.push(m);
  }
};
#endif // ACTIVE_H
