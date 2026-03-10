#pragma once
#include <string>

#include "Point.h"
class Shape
{
public:
    virtual ~Shape() = default;//âèðòóàëüíûé äåñòðóêòîð

    virtual double getArea() const = 0;
    virtual Point getCenter() const = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double factor) = 0;//èçîòðîïíîå ìàøòàáèðîâàíèå îòí öåíòðà
    virtual std::string getName() const = 0;

};

