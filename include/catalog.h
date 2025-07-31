#ifndef CATALOG_H
#define CATALOG_H
#include <Arduino.h>
#include "object-to-watch.h"

class Category
{
private:
    ObjectToWatch *objectsToWatch[10];
    String code;
    String label;

public:
    Category(
    String code,
    String label);
};

class Catalog
{
private:
    Category *categories[3];

public:
    Catalog();
};

#endif