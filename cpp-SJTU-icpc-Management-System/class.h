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
    
private:
    Team() = delete;

public:
    Team(const std::string& name, int score) : name_(name), score_(score), record_() {}
    Team(const Team&) = default;

    std::string name() const { return name_; }
    int score() const { return score_; }
    SelfDataStructure& record_ref() { return record_; }
    SelfDataStructure  record_copy() { return record_; }

    static TeamInfo new_team(const std::string& name, int score = 0);
    
    // bool contains(std::string name) { return this->name_ == name; }
    // bool begin() { return this->begin_; }
    // void start(size_t, size_t);
};

class Problem
{
    using SelfDataStructure = std::vector<SubmitInfo>;
private:
    std::string name_;
    
    // submit infomation (ptr) collection
    SelfDataStructure record_;
private:
    Problem() = delete;

public:
    Problem(const std::string& name) : name_(name), record_() {}
    Problem(const Problem&) = default;

    std::string name() const { return name_; }
    SelfDataStructure& record_ref() { return record_; }
    SelfDataStructure  record_copy() { return record_; }

    static ProblemInfo new_problem(const std::string& name);
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

    static SubmitInfo new_submit(TeamInfo& team, ProblemInfo& prob, Status state, size_t time);
};

