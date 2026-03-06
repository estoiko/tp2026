#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>

#include "shape.h"
#include "rectangle.h"
#include "ellipse.h"
#include "point.h"
#include "rectangular_trapezoid.h"
#include "composite_shape.h"

void printInfo(const std::vector<std::unique_ptr<Shape>>& shapes) {
    std::cout << std::fixed << std::setprecision(2);

    std::cout << "[ \n";

    for (const auto& shape_ptr : shapes) {
        if (shape_ptr) {
            std::cout << shape_ptr->getName() << ", ";
            Point center = shape_ptr->getCenter();
            std::cout << "(" << center.x << ", " << center.y << ")" << ", ";
            std::cout << shape_ptr->getArea() << "\n";

        }
    }

    std::cout << "]\n";
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point(0, 0), Point(2, 3)));
    shapes.push_back(std::make_unique<Ellipse>(Point(5, 5), 2.0, 1.5));
    shapes.push_back(std::make_unique<Rectangle>(Point(-1, -2), Point(7, 8)));
    shapes.push_back(std::make_unique<RectangularTrapezoid>(Point(1, 1), 3.0, 1.0, 2.0));

    CompositeShape composite;
    composite.addShape(std::make_unique<Rectangle>(Point(10, 10), Point(12, 13)));
    composite.addShape(std::make_unique<Ellipse>(Point(15, 15), 3.0, 2.0));

    auto compositePtr = std::make_unique<CompositeShape>();
    compositePtr->addShape(std::make_unique<Rectangle>(Point(10, 10), Point(12, 13)));
    compositePtr->addShape(std::make_unique<Ellipse>(Point(15, 15), 3.0, 2.0));
    shapes.push_back(std::move(compositePtr));

    printInfo(shapes);

    for (const auto& shape_ptr : shapes) {
        if (shape_ptr) {
            shape_ptr->scale(2.0);
        }
    }

    printInfo(shapes);

    return 0;
}
