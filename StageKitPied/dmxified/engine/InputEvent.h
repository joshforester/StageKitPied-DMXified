#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include <string>
#include <cstdint>  // For uint8_t

class InputEvent {
public:
	InputEvent();

	// Constructor to initialize the InputEvent
    InputEvent(const std::string& id,
               uint8_t left_weight = 0,
               uint8_t right_weight = 0);

    // Getters for each attribute
    const std::string& getId() const;
    uint8_t getLeftWeight() const;
    uint8_t getRightWeight() const;

    // Setters for each attribute
    void setId(const std::string& id);
    void setLeftWeight(const uint8_t left_weight);
    void setRightWeight(const uint8_t right_weight);

private:
    std::string id;
    uint8_t left_weight;
    uint8_t right_weight;
};

#endif // INPUTEVENT_H
