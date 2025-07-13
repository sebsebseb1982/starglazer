#ifndef TRACKED_OBJECT_H
#define TRACKED_OBJECT_H
#include <Arduino.h>

class TrackedObject {
private:

public:
  String type;
  String name;
  TrackedObject(
    String type,
    String name);
};

#endif
