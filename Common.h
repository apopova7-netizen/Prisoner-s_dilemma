#pragma once


enum class Choice: char {
    COOPERATE ='C',
    DEFECT ='D'
};

enum class Mode {
    DETAILED,
    FAST,
    TOURNAMENT
};

struct RoundChoices {
    Choice first_strategy_choice;
    Choice second_strategy_choice;
    Choice third_strategy_choice;
};

struct Scores {
    int score1;
    int score2;
    int score3;
};

