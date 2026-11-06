#include "InputDataProcessing.h"
#include <stdexcept>


ParsedInputData parse_input_data(int argc, char *argv[]) {
    ParsedInputData parsed_data; // сюда соханяю распарс. из строки входные параметры

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.find("--")==0) {
            //строчка (аргумет) начинается с --/ ищет первое вхождение --
            int pos_equal = arg.find("="); // индекс первого вхождения =
            if (pos_equal == std::string::npos) {
                throw std::invalid_argument("Invalid input data format");
            }
            std::string key = arg.substr(0, pos_equal); //строка до знака =
            std::string value = arg.substr(pos_equal+1);//строка после =

            if (key == "--mode") {
                if (value == "detailed")
                    parsed_data.mode = Mode::DETAILED;

                else if (value == "fast")
                    parsed_data.mode = Mode::FAST;

                else if (value == "tournament")
                    parsed_data.mode = Mode::TOURNAMENT;

                else
                    throw std::invalid_argument("Invalid mode:" + value);

            }
            else if (key == "--steps") {
                try {
                    parsed_data.steps=  std::stoi(value); //string to int пытается перевести в int
                }
                catch (const std::invalid_argument&) {
                    throw std::invalid_argument("Invalid argument: " + value + ". " + "An int-type number is expected.");
                } //если не число
                catch (const std::out_of_range&) {
                    throw std::out_of_range( "The number " + value + " is out of range for type int");
                } //не влезает в int


            }
            else if (key == "--configs") {
                parsed_data.config_dir = value;

            }
            else if (key == "--matrix") {
                parsed_data.matrix_file = value;
            }
            else {
                throw std::invalid_argument("Invalid key: " + key);
            }
        }

        else
            parsed_data.strategies.push_back(arg);

    }
    return parsed_data;
}
ValidatedInputData validate_input_data(ParsedInputData& parsed_data) {

    ValidatedInputData validated_data;

    int num_strategies = parsed_data.strategies.size();

    if (num_strategies < 3)
        throw std::invalid_argument("You must provide at least 3 strategies to play");
    validated_data.strategies = parsed_data.strategies;


    if (parsed_data.mode.has_value()) {
        Mode mode = parsed_data.mode.value();
        if (num_strategies > 3 && mode != Mode::TOURNAMENT)
            throw std::invalid_argument("If there are more than 3 strategies, only tournament mode is available.");
        validated_data.mode = mode;
    }
    else {
        if (num_strategies > 3)
            validated_data.mode = Mode::TOURNAMENT;
        else
            validated_data.mode = Mode::DETAILED;
    }

    if (parsed_data.steps.has_value()) {

        int steps = parsed_data.steps.value();

        if (steps <= 0)
            throw std::invalid_argument("The number of steps must be greater than 0");

        validated_data.steps = steps;

    }
    else {
        if (validated_data.mode ==  Mode::DETAILED)
            validated_data.steps = 0;
        else {
            validated_data.steps = 100;
        }
    }
    validated_data.config_dir = parsed_data.config_dir;
    validated_data.matrix_file = parsed_data.matrix_file;

    return validated_data;

}
GameMatrix load_game_matrix(std::string path) {
    GameMatrix game_matrix;

    return game_matrix;
}