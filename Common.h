#pragma once
#include <map>

enum class Choice: char { //общий
    COOPERATE ='C',
    DEFECT ='D'
};

struct RoundChoices { //общий
    Choice str1_choice;
    Choice str2_choice;
    Choice str3_choice;
};

enum class Mode {
    DETAILED,
    FAST,
    TOURNAMENT
};


struct Scores {
    int score1;
    int score2;
    int score3;
};

class GameMatrix {
public:
    ////////////////
    //////////////
    /////////////
private:
    std::map<RoundChoices,Scores> matrix_;
public:
    GameMatrix();
    ~GameMatrix();
    void set_matrix_row();
    std::map<RoundChoices,Scores> get_matrix_row();
    Scores get_scores_by_choices();
};

