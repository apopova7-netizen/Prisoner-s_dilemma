#pragma once

#include "Common.h"
#include "InputDataProcessing.h"

class GameSimulator {
public:
    GameSimulator(const StrategiesRegistration& registry, const GameMatrix& matrix);
    ~GameSimulator();
    void run(ValidatedInputData& data);
private:
    GameMatrix& matrix_;
    StrategiesRegistration& registry_;

};
