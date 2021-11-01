#include "base.hpp"
#include <vector>

class Team;
class Problem;
class Submit;
class Game;

using TeamInfo      = std::shared_ptr<Team>;
using ProblemInfo   = std::shared_ptr<Problem>;
using SubmitInfo    = std::shared_ptr<Submit>;

class Team
{
    using SelfDataStructure = std::vector<SubmitInfo>;
private:
    std::string name_;  
    int score_; // score = num of accepted prob 

    // submit infomation (ptr) collection
    SelfDataStructure record_;  // all history submit record
    SelfDataStructure buf_;     // cache submit when rank is in frozen, which wiill be merge into record after unfreeze 

private:
    Team() = delete;

public:
    Team(const std::string& name, int score) : name_(name), score_(score), record_(), buf_() {}
    Team(const Team&) = default;

    std::string name() const { return name_; }
    int score() const { return score_; }
    SelfDataStructure& record_ref() { return record_; }
    SelfDataStructure  record_copy() { return record_; }
    SelfDataStructure& buf_ref() { return buf_; }
    SelfDataStructure  all_ac_sub();    
    void    calculate_score();
    size_t  calculate_penalty();

    static TeamInfo new_team(const std::string& name, int score = 0);
    static bool     compare(const TeamInfo&, const TeamInfo&);
};

class Problem
{
    using SelfDataStructure = std::vector<SubmitInfo>;
private:
    char name_;
    
    // submit infomation (ptr) collection
    SelfDataStructure record_;  // all submit inf about this problem 
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
    size_t      when_;  // submit time 

private:
    Submit() = delete;

public:
    Submit(TeamInfo& team, ProblemInfo& prob, Status state, size_t time) : who_(team), which_(prob), state_(state), when_(time) {}
    Submit(const Submit& ) = default;

    Status      status() const { return this->state_; }
    ProblemInfo problem() const { return this->which_; }
    TeamInfo    team() const { return who_; }
    size_t      time() const { return when_; }

    static SubmitInfo new_submit(TeamInfo& team, ProblemInfo& prob, Status state, size_t time);
};

