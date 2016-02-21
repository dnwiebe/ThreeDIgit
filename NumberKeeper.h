#ifndef NumberKeeper_h
#define NumberKeeper_h

class NumberKeeper {
private:
  signed int number;
  unsigned long prevNow;
  unsigned long millisecondsRemainingInDay;
  
  
public:
  NumberKeeper ();
  void update (unsigned long now);
  
  int value () const;
  void increaseBy (signed int increment);
};

#endif
