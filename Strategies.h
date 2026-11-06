#pragma once

#include "Common.h"
#include <vector>
#include <fstream>

class Strategy {
public:
    virtual Choice make_choice(const std::vector<RoundChoices>& history) = 0;
    virtual ~Strategy() = default;
    virtual void reset(int my_position) = 0;
};



class StrAlwaysCooperate final : public Strategy {
public:

    Choice make_choice(const std::vector<RoundChoices>& history) override {
        return Choice::COOPERATE;
    }
    void reset(int my_position) override{};

};



class StrAlwaysDefect final : public Strategy {
public:

    Choice make_choice(const std::vector<RoundChoices>& history) override {
        return Choice::DEFECT;
    }
    void reset(int my_position) override{};
};



class StrNoSelfRepeat final : public Strategy {
public:

    Choice make_choice(const std::vector<RoundChoices>& history) override {

        if (history.empty())
            return Choice::COOPERATE;

        Choice my_last_choice;
        if (my_position_ == 1)
            my_last_choice = history.back().str1_choice;
        else if (my_position_ == 2)
            my_last_choice = history.back().str2_choice;
        else
            my_last_choice = history.back().str3_choice;

        return my_last_choice == Choice::COOPERATE ? Choice::DEFECT: Choice::COOPERATE;

    }

    void reset(const int my_position) override {
        my_position_ = my_position;
    }
private:
    int my_position_ = 0;
};



class StrForgivingRetaliator final : public Strategy {
public:

    Choice make_choice(const std::vector<RoundChoices>& history) override {

        if (history.empty())
            return Choice::COOPERATE;

        bool double_defect = false;
        const RoundChoices& last_round = history.back();

        if (my_position_ == 1)
            double_defect = (last_round.str2_choice == Choice::DEFECT) && (last_round.str3_choice == Choice::DEFECT);
        else if (my_position_ == 2)
            double_defect = (last_round.str1_choice == Choice::DEFECT) && (last_round.str3_choice == Choice::DEFECT);
        else
            double_defect = (last_round.str1_choice == Choice::DEFECT) && (last_round.str2_choice == Choice::DEFECT);

        return double_defect ? Choice::DEFECT : Choice::COOPERATE;
    }
    void reset(const int my_position) override {
        my_position_ = my_position;
    }
private:
    int my_position_ = 0;
};



class StrMajorityFollower final : public Strategy {
public:

    Choice make_choice(const std::vector<RoundChoices>& history) override {

        if (history.empty())
            return Choice::COOPERATE;

        int opponents_defect = 0;
        int opponents_cooperate = 0;
        for (const auto& round : history) {

            if (my_position_ != 1) {
                opponents_cooperate += round.str1_choice == Choice::COOPERATE ? 1 : 0;
                opponents_defect += round.str1_choice == Choice::DEFECT ? 1 : 0;
            }
            if (my_position_ != 2) {
                opponents_cooperate += round.str2_choice == Choice::COOPERATE ? 1 : 0;
                opponents_defect += round.str2_choice == Choice::DEFECT ? 1 : 0;
            }
            if (my_position_ != 3) {
                opponents_cooperate += round.str3_choice == Choice::COOPERATE ? 1 : 0;
                opponents_defect += round.str3_choice == Choice::DEFECT ? 1 : 0;
            }
        }

        return opponents_defect > opponents_cooperate ? Choice::DEFECT : Choice::COOPERATE;

    }
    void reset(const int my_position) override {
        my_position_ = my_position;
    }
private:
    int my_position_ = 0;
};


class StrCountingRetaliator final : public Strategy {
public:
    explicit StrCountingRetaliator(const std::string& config): num_cooperates_(DEFAULT_NUM_COOPERATES), cooperate_count_(DEFAULT_NUM_COOPERATES), defect_count_(0) {
        if (!config.empty()) {
            const std::string config_counting_retaliator = config + "counting_retaliator";
            std::ifstream file;
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            file.open(config_counting_retaliator);
            file >> num_cooperates_;
            file.close();
            if (num_cooperates_ <= 0) {
                throw std::invalid_argument("Incorrect data in the configuration file for CountingRetaliator. The number of cooperation steps must be > 0.");
            }
            cooperate_count_ = num_cooperates_;
        }

    }

    Choice make_choice(const std::vector<RoundChoices>& history) override {
        if (cooperate_count_ > 0) {
            cooperate_count_--;
            return Choice::COOPERATE;
        }
        if (defect_count_ > 0) {
            defect_count_--;
            if (defect_count_ == 0) {
                cooperate_count_ = num_cooperates_;
            }
            return Choice::DEFECT;
        }


        int opponents_defect = 0;
        for (int i = 0; i < num_cooperates_; i++) {

            const size_t check_idx = history.size() - i - 1;
            const RoundChoices& round = history[check_idx];

            if (my_position_ != 1 && round.str1_choice == Choice::DEFECT)
                    opponents_defect++;
            if (my_position_ != 2 && round.str2_choice == Choice::DEFECT)
                opponents_defect++;
            if (my_position_ != 3 && round.str3_choice == Choice::DEFECT)
                opponents_defect++;

        }

        if (opponents_defect > 0) {
            defect_count_ = opponents_defect;
            defect_count_--;
            if (defect_count_ == 0) {
                cooperate_count_ = num_cooperates_;
            }
            return Choice::DEFECT;
        }
        cooperate_count_ = num_cooperates_;
        cooperate_count_--;
        return Choice::COOPERATE;

    };

    void reset(const int my_position) override {
        my_position_ = my_position;
        defect_count_ = 0;
        cooperate_count_ = num_cooperates_;
    };

private:
    static constexpr int DEFAULT_NUM_COOPERATES = 5;
    int num_cooperates_;
    int cooperate_count_;
    int defect_count_;
    int my_position_ = 0;

};


class StrConsensusTrigger final : public Strategy {
public:

    Choice make_choice(const std::vector<RoundChoices>& history) override {

        if (history.empty())
            return Choice::COOPERATE;

        const RoundChoices& last_round = history.back();

        if (my_position_ == 1) {
            if (last_round.str2_choice != regime_ && last_round.str3_choice != regime_)
                regime_ = (regime_ == Choice::COOPERATE ? Choice::DEFECT : Choice::COOPERATE);
        }

        else if (my_position_ == 2){
            if (last_round.str1_choice != regime_ && last_round.str3_choice != regime_)
                regime_ = (regime_ == Choice::COOPERATE ? Choice::DEFECT : Choice::COOPERATE);
        }

        else{
            if (last_round.str1_choice != regime_ && last_round.str2_choice != regime_)
                regime_ = (regime_ == Choice::COOPERATE ? Choice::DEFECT : Choice::COOPERATE);
        }

        return regime_;
    }


    void reset(const int my_position) override {
        my_position_ = my_position;
        regime_ = Choice::COOPERATE;
    }
private:
    Choice regime_ = Choice::COOPERATE;
    int my_position_ = 0;
};


class StrVoter final : public Strategy {
public:
    explicit StrVoter(std::string& config);
    Choice make_choice(const std::vector<RoundChoices>& history) override {
        int cooperate_choice = 0;
        int defect_choice = 0;
        for
    };
    void reset(int my_position) override{};
};

explicit StrCountingRetaliator(const std::string& config): num_cooperates_(DEFAULT_NUM_COOPERATES), cooperate_count_(DEFAULT_NUM_COOPERATES), defect_count_(0) {
    if (!config.empty()) {
        const std::string config_counting_retaliator = config + "counting_retaliator";
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        file.open(config_counting_retaliator);
        file >> num_cooperates_;
        file.close();
        if (num_cooperates_ <= 0) {
            throw std::invalid_argument("Incorrect data in the configuration file for CountingRetaliator. The number of cooperation steps must be > 0.");
        }
        cooperate_count_ = num_cooperates_;
    }

}
class StrAlternator final : public Strategy {
public:
    StrAlternator();
    Choice make_choice(const std::vector<RoundChoices>& history) override;
    void reset(int my_position) override{};
};

class StrDisappointedChanger final : public Strategy {
public:
    StrDisappointedChanger();
    Choice make_choice(const std::vector<RoundChoices>& history) override;
    void reset(int my_position) override{};
};

