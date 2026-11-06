#pragma once
#include "Common.h"
#include <vector>
#include <string>
#include <optional>


struct ParsedInputData {
    std::vector<std::string> strategies;
    std::optional<Mode> mode;
    std::optional<int> steps;
    std::string config_dir;
    std::string matrix_file;
};

struct ValidatedInputData {
    std::vector<std::string> strategies;
    Mode mode;
    int steps;
    std::string config_dir;
    std::string matrix_file;
};






GameMatrix load_game_matrix(std::string path);
ParsedInputData parse_input_data(int argc, char *argv[]);
ValidatedInputData validate_input_data(ParsedInputData& parsed_data);


