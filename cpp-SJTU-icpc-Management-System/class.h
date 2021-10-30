#include "base.hpp"
#include <vector>

class Team;
class Problem;
class Submit;
class Game;

using TeamInfo = std::shared_ptr<Team>;
using ProblemInfo = std::shared_ptr<Problem>;
using SubmitInfo = std::shared_ptr<Submit>;

class Team
{
    using SelfDataStructure = std::vector<SubmitInfo>;
private:
    std::string name_;
    int score_;

    // submit infomation (ptr) collection
    SelfDataStructure record_;
    SelfDataStructure buf_;

private:
    Team() = delete;

    void incr_score() { score_++; }
    void clear_score() { score_ = 0; }
public:
    Team(const std::string& name, int score) : name_(name), score_(score), record_(), buf_() {}
    Team(const Team&) = default;

    std::string name() const { return name_; }
    int score() const { return score_; }
    SelfDataStructure& record_ref() { return record_; }
    SelfDataStructure  record_copy() { return record_; }
    SelfDataStructure& buf_ref() { return buf_; }
    void calculate_score()
    {
        score_ = 0;
        for (auto& sub : record_)    // history submit
            if (sub->status().is_accepted())
                score_++;
        for (auto& sub : buf_)       // buffer submit
            if (sub->status().is_accepted())
                score_++;
    }

    static TeamInfo new_team(const std::string& name, int score = 0);
    static bool     compare(const TeamInfo&, const TeamInfo&);
};

class Problem
{
    using SelfDataStructure = std::vector<SubmitInfo>;
private:
    char name_;
    
    // submit infomation (ptr) collection
    SelfDataStructure record_;
private:
    Problem() = delete;

public:
    Problem(const char name) : name_(name), record_() {}
    Problem(const Problem&) = default;

    char name() const { return name_; }
    SelfDataStructure& record_ref() { return record_; }
    SelfDataStructure  record_copy() { return record_; }

    static ProblemInfo new_problem(const char name);
};

class Submit
{
private:
    TeamInfo    who_;
    ProblemInfo which_;
    Status      state_;
    size_t      when_;

private:
    Submit() = delete;

public:
    Submit(TeamInfo& team, ProblemInfo& prob, Status state, size_t time) : who_(team), which_(prob), state_(state), when_(time) {}
    Submit(const Submit& ) = default;

    Status status() const { return this->state_; }
    ProblemInfo problem() const { return this->which_; }
    TeamInfo team() const { return who_; }
    size_t time() const { return when_; }

    static SubmitInfo new_submit(TeamInfo& team, ProblemInfo& prob, Status state, size_t time);
};

