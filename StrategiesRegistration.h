#pragma once

#include <memory>
#include "Common.h"
#include "Strategies.h"
#include "StrategiesCreator.h"


class StrategiesRegistration {
public:
    void add(const std::string& strategy_name, std::unique_ptr<StrategiesCreator> creator) {

        registry_.insert_or_assign(strategy_name, std::move(creator));
    }

    std::unique_ptr<Strategy> CreateStrategy(const std::string& strategy_name) const {
        auto found_strategy = registry_.find(strategy_name);
        if (found_strategy == registry_.end()) {
            throw std::invalid_argument("No such strategy: " + strategy_name);
        }
        return found_strategy->second->CreateStrategy(); // вызвали метод фбрики( в каждой строчке реестра лежит указатель на объяект фабрики конкретной стратегии (мы создали с момент регистрации объект фабрики конкретной стратегии и передали в указатель под фидоб общей фабрики, но там по факту лежит конкретная и у нее мы вызвали метод поэтому он точно знает как создавать) (мы у него вызываем метод create Strategy)
    }
private:
    std::map<std::string, std::unique_ptr<StrategiesCreator>> registry_;
};

