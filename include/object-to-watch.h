#ifndef OBJECT_TO_WATCH_H
#define OBJECT_TO_WATCH_H
#include <Arduino.h>

class ObjectToWatch
{
private:
public:
  String type;
  String code;
  String label;
  ObjectToWatch(
      String type,
      String code,
      String label);
  ObjectToWatch() : ObjectToWatch("", "", "") {};
};

#endif
