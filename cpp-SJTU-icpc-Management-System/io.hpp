#include "./base.hpp"
#include <deque>
#include <fstream>
#include <unordered_map>
#include <optional>
#include <regex>

namespace Input
{
using InputSeq = std::deque<Command>;

// get status enum from string 
Status GetSubStatusEnumFromString(std::string stmt)
{
    SubStatus state = SubStatus::A;
    if (stmt.starts_with("A"))
        ;// do nothing 
    else if (stmt.starts_with("W"))
        state = SubStatus::W;
    else if (stmt.starts_with("R"))
        state = SubStatus::R;
    else
        state = SubStatus::T;
    return Status(state);
}

// deal with a string and return cmd  
Command GenCommandFromLine(std::string line)
{
    static std::unordered_map<OpKind, std::regex> HashInput {};
    static bool init_flag{ false };
    if (not init_flag)
    {
        HashInput.emplace(OpKind::AddTeam,      R"(^ADDTEAM\s?(\w+)$)");
        HashInput.emplace(OpKind::Start,        R"(^([A-Z]|\s)+(\d+)([A-Z]|\s)+(\d+)$)");
        HashInput.emplace(OpKind::Submit,       R"(^SUBMIT\s(\w)\sBY\s(\w+)\sWITH\s(\w+)\sAT\s\d+$)");
        HashInput.emplace(OpKind::QueryRank,    R"(^QUERY_RANKING\s(\w+)$)");
        HashInput.emplace(OpKind::QuerySub,     R"(^QUERY_SUBMISSION\s(\w+)\s(\w|\s)+=(\w+)(\s|\w)+=(\w+)$)");
        init_flag = true;
    }

    if (line.starts_with("FLUSH")) // flush
        return std::make_shared<Instruction>(OpKind::Flush);
    else if (line.starts_with("SCROLL"))
        return std::make_shared<Instruction>(OpKind::Scroll);
    else if (line.starts_with("FREEZE"))
        return std::make_shared<Instruction>(OpKind::Freeze);
    else if (line.starts_with("END")) 
        return std::make_shared<Instruction>(OpKind::End);
    
    OpKind type = OpKind::AddTeam;  // '=' is used for hold place 
    if (line.starts_with("ADD"))    // add team 
        type = OpKind::AddTeam;
    else if (line.starts_with("STA"))   // start game 
        type = OpKind::Start;
    else if (line.starts_with("SUB"))   // submit 
        type = OpKind::Submit;
    else if (line.starts_with("QUERY_RANK"))
        type = OpKind::QueryRank;
    else if (line.starts_with("QUERY_SUB"))   // todo 
        type = OpKind::QuerySub;
    else
        assert(false);  // unreachable code

    std::smatch match_seq;
    auto ret = std::regex_match(line, match_seq, HashInput.at(type));
    assert(ret);    // search arg successfully

    switch (type)
    {
    case OpKind::AddTeam:
        return std::make_shared<Instruction>(OpKind::AddTeam, AddTeamArg(match_seq[1].str()));
        
    case OpKind::Start:
    {
        size_t dura = std::stoull(match_seq[1].str());
        size_t pbnum = std::stoull(match_seq[2].str());
        return std::make_shared<Instruction>(OpKind::Start, StartGameArg(dura, pbnum));
    }
    
    case OpKind::Submit:
    {
        auto prob = match_seq[1].str();
        auto team = match_seq[2].str();
        auto time = std::stoull(match_seq[4].str());
        auto state = GetSubStatusEnumFromString(match_seq[3].str());
        return std::make_shared<Instruction>(OpKind::Submit, SubmitArg(prob, team, state, time));
    }

    case OpKind::QueryRank:
        return std::make_shared<Instruction>(OpKind::QueryRank, AddTeamArg(match_seq[1].str()));
                                                            // ^^^^^^^^^ here is correct, do not change it

    case OpKind::QuerySub:
    {
        auto team = match_seq[1].str();
        auto prob = match_seq[2].str();
        auto state = GetSubStatusEnumFromString(match_seq[3].str());
        return std::make_shared<Instruction>(OpKind::QuerySub, QuerySubArg(team, prob, state));
    }
        
    // unreachable code 
    default:
        assert(false);
        return std::make_shared<Instruction>(OpKind::End);
        break;
    }
}

// read a input file, return all command in a vector ( in order )
InputSeq ReadCommandFromFile(std::string filename)
{
    InputSeq ret {};
    std::ifstream in{ filename, std::ios::in };
    assert(in.is_open());   // file must exist
    
    std::string linebuf;
    while (in.good())
    {
        std::getline(in, linebuf);
        if (linebuf.size() > 3)
            ret.push_back(GenCommandFromLine(linebuf));
        linebuf.clear();
    }
    return ret;
}
    
} // namespace Input

namespace Output
{

class Outputter
{
private:
    /* data */
public:
    Outputter(/* args */);
    ~Outputter();

    void write(OpKind kind, int ret_code, std::optional<std::string> appendix)
    {

    }

    static Outputter& SendTo(std::string target) 
    {
        // todo 
        static Outputter put;
        return put;
    }
};

Outputter::Outputter(/* args */)
{
}

Outputter::~Outputter()
{
}


    
} // namespace Output
