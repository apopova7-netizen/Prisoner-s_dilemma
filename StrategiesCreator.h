#pragma once

#include <memory>
#include "Common.h"
#include "Strategies.h"

class StrategiesRegistration;

class StrategiesCreator {
public:
    virtual ~StrategiesCreator()=default;
    virtual std::unique_ptr<Strategy> CreateStrategy() const = 0;
};

class AlwaysCooperateCreator final: public StrategiesCreator {
public:
    std::unique_ptr<Strategy> CreateStrategy() const override {
        return std::make_unique<StrAlwaysCooperate>();
    }

};

class AlwaysDefectCreator final: public StrategiesCreator {
public:
    std::unique_ptr<Strategy> CreateStrategy() const override {
        return std::make_unique<StrAlwaysDefect>();
    }

};

class NoSelfRepeatCreator final: public StrategiesCreator {
public:

    std::unique_ptr<Strategy> CreateStrategy() const override {
        return std::make_unique<StrNoSelfRepeat>();
    };
};

class CountingRetaliatorCreator final: public StrategiesCreator {
public:
    explicit CountingRetaliatorCreator(std::string config): config_(std::move(config)) {}; ///////////////почему бы не использовать указатель на строку config

    std::unique_ptr<Strategy> CreateStrategy() const override {
        return std::make_unique<StrCountingRetaliator>(config_);
    }
private:
    std::string config_;
};

class VoterCreator final: public StrategiesCreator {
public:
    explicit VoterCreator(const std::string& config, const StrategiesRegistration& registry): config_(config), registry_(registry) {};
private:
    std::string config_;
    const StrategiesRegistration& registry_;

};