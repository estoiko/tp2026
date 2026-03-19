#include "composite_shape.h"
#include <algorithm>
#include <cstdio>

void CompositeShape::addShape(Shape* shape) {
    shapes_.push_back(std::unique_ptr<Shape>(shape));
}

double CompositeShape::getArea() const {
    double total = 0;
    for (const auto& shape : shapes_) {
        total += shape->getArea();
    }
    return total;
}

Point CompositeShape::getCenter() const {
    if (shapes_.empty()) return Point(0, 0);

    double minX = shapes_[0]->getCenter().x;
    double minY = shapes_[0]->getCenter().y;
    double maxX = minX;
    double maxY = minY;

    for (const auto& shape : shapes_) {
        Point center = shape->getCenter();
        minX = std::min(minX, center.x);
        minY = std::min(minY, center.y);
        maxX = std::max(maxX, center.x);
        maxY = std::max(maxY, center.y);
    }

    return Point((minX + maxX) / 2, (minY + maxY) / 2);
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    Point compositeCenter = getCenter();

    for (auto& shape : shapes_) {
        Point shapeCenter = shape->getCenter();

        Point newCenter(
            compositeCenter.x + (shapeCenter.x - compositeCenter.x) * factor,
            compositeCenter.y + (shapeCenter.y - compositeCenter.y) * factor
        );

        shape->move(newCenter.x - shapeCenter.x, newCenter.y - shapeCenter.y);
        shape->scale(factor);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

void CompositeShape::print() const {
    printf("[%s, (%.2f, %.2f), %.2f:\n",
        getName().c_str(),
        getCenter().x, getCenter().y,
        getArea());

    for (size_t i = 0; i < shapes_.size(); ++i) {
        printf("  ");
        shapes_[i]->print();
        if (i < shapes_.size() - 1) {
            printf(",\n");
        }
        else {
            printf("\n");
        }
    }
    printf("]");
}
