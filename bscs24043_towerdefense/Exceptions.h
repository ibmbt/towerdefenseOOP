#pragma once
#include "myString.h"


class FileNotFound {
    String message;
public:
    FileNotFound() : message("File not found!") {}

    String getMessage() const {
        return message;
    }
};

class InsufficientCoinsError {
    String message;
public:
    InsufficientCoinsError() : message("Not enough coins for this action!") {}

    String getMessage() const {
        return message;
    }
};

class TowerPlacementError {
    String message;
public:
    TowerPlacementError() : message("Cannot place tower here!") {}

    String getMessage() const {
        return message;
    }
};
