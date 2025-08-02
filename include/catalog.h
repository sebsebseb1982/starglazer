#ifndef CATALOG_H
#define CATALOG_H
#include <Arduino.h>
#include "object-to-watch.h"
#include <list>

class Category
{
private:
    String code;
    String label;

public:
    std::list<ObjectToWatch> objectsToWatch;
    Category() : code(""), label("") {};
    Category(
        String code,
        String label);
};

class Catalog
{
private:

public:
    std::list<Category> categories;
    Catalog();
};

#endif