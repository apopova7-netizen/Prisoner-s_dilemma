#pragma once
#include "Common.h"
#include <vector>
#include <string>
#include <optional>
#include <map>

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

class GameMatrix {
public:

private:
    std::map<RoundChoices,Scores> matrix_;
public:
    GameMatrix();
    ~GameMatrix();
    void set_matrix_row();
    std::map<RoundChoices,Scores> get_matrix_row();
    Scores get_scores_by_choices();




};
GameMatrix load_game_matrix(std::string path);
ParsedInputData parse_input_data(int argc, char *argv[]);
ValidatedInputData validate_input_data(ParsedInputData& parsed_data);


