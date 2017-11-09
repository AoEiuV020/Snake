#include "model/Point.h"

Point::Point() {}

Point::~Point() {}

Point::Type Point::getType() const {
    return type;
}

void Point::setType(Type type_) {
    type = type_;
}

void Point::setParent(const Pos &p_) {
    parent = p_;
}

Pos Point::getParent() const {
    return parent;
}

