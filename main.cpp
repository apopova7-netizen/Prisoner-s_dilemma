#include <iostream>

#include "Common.h"
#include "InputDataProcessing.h"



#include <iostream>
#include <memory>
#include <stdexcept>

#include "Common.h"
#include "InputDataProcessing.h"
#include "StrategiesRegistration.h"
#include "StrategiesCreator.h"
#include "GameSimulator.h"

int main(int argc, char *argv[]) {
    try {
        // 1. Парсинг и валидация аргументов командной строки (ваши Файлы 4 и 5)
        ParsedInputData parsed_data = parse_input_data(argc, argv);
        ValidatedInputData validated_data = validate_input_data(parsed_data);

        // 2. Инициализация матрицы игры (ваш Файл 7)
        GameMatrix game_matrix;
        if (!validated_data.matrix_file.empty()) {
            game_matrix = load_game_matrix(validated_data.matrix_file);
        }

        // 3. Создание реестра фабрик
        // Объекты фабрик завернуты в std::unique_ptr, реестр заберет владение ими
        StrategiesRegistration registry;

        // ====================================================================
        // РЕГИСТРАЦИЯ ОБЫЧНЫХ СТРАТЕГИЙ
        // ====================================================================

        // Тривиальные стратегии (не требуют папки с конфигурациями)
        registry.add("always_cooperate", std::make_unique<AlwaysCooperateCreator>());
        registry.add("always_defect", std::make_unique<AlwaysDefectCreator>());
        registry.add("no_self_repeat", std::make_unique<NoSelfRepeatCreator>());

        // Сложные контекстные стратегии (без файлов конфигурации)
        // (Предполагаем, что вы добавили аналогичные простые фабрики для них)
        registry.add("forgiving_retaliator", std::make_unique<ForgivingRetaliatorCreator>());
        registry.add("consensus_trigger", std::make_unique<ConsensusTriggerCreator>());

        // Параметризуемые стратегии (им нужен путь к файлам конфигурации)
        registry.add("counting_retaliator", std::make_unique<CountingRetaliatorCreator>(validated_data.config_dir));
        registry.add("majority_follower", std::make_unique<MajorityFollowerCreator>(validated_data.config_dir));

        // ====================================================================
        // РЕГИСТРАЦИЯ МЕТАСТРАТЕГИЙ
        // ====================================================================
        // Передаем ссылку на registry. На этом этапе подстратегии еще НЕ создаются,
        // фабрика просто запоминает ссылку на реестр для будущего использования.
        registry.add("voter", std::make_unique<VoterCreator>(validated_data.config_dir, registry));
        registry.add("alternator", std::make_unique<AlternatorCreator>(validated_data.config_dir, registry));
        registry.add("disappointed_changer", std::make_unique<DisappointedChangerCreator>(validated_data.config_dir, registry));

        // ====================================================================
        // ЗАПУСК ИГРЫ
        // ====================================================================
        // Передаем заполненный реестр и матрицу в симулятор (ваш Файл 6)
        GameSimulator simulator(registry, game_matrix);
        simulator.run(validated_data);

    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка выполнения программы: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}