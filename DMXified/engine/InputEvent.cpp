#include "InputEvent.h"

InputEvent::InputEvent() {
	// Empty constructor
}

// Constructor to initialize the InputEvent
InputEvent::InputEvent(const std::string& id, uint8_t left_weight, uint8_t right_weight)
    : id(id), left_weight(left_weight), right_weight(right_weight) {}

// Getters
const std::string& InputEvent::getId() const {
    return id;
}

uint8_t InputEvent::getLeftWeight() const {
    return left_weight;
}

uint8_t InputEvent::getRightWeight() const {
    return right_weight;
}

// Setters
void InputEvent::setId(const std::string& id) {
    this->id = id;
}

void InputEvent::setLeftWeight(const uint8_t left_weight) {
    this->left_weight = left_weight;
}

void InputEvent::setRightWeight(const uint8_t right_weight) {
    this->right_weight = right_weight;
}
