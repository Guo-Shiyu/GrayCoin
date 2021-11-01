#include "./base.hpp"
#include <deque>
#include <fstream>
#include <unordered_map>
#include <optional>
#include <regex>
#include <iostream>

#include <cassert>

namespace Input
{
using InputSeq = std::deque<Command>;

// get status enum from string 
Status GetSubStatusEnumFromString(std::string stmt)
{
    if (stmt.starts_with("ALL"))
        return SubStatus::Any;

    SubStatus state = SubStatus::A;
    if (stmt.starts_with("Ac"))
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
        HashInput.emplace(OpKind::AddTeam,      R"(ADDTEAM\s?(\w+))");
        HashInput.emplace(OpKind::Start,        R"(([A-Z]|\s)+(\d+)([A-Z]|\s)+(\d+))");
        HashInput.emplace(OpKind::Submit,       R"(SUBMIT\s(\w)\sBY\s(\w+)\sWITH\s(\w+)\sAT\s(\d+))");
        HashInput.emplace(OpKind::QueryRank,    R"(QUERY_RANKING\s(\w+))");
        HashInput.emplace(OpKind::QuerySub,     R"(QUERY_SUBMISSION\s(\w+)\s(\w|\s)+=(\w+)(\s|\w)+=(\w+))");
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
        size_t dura = std::stoull(match_seq[2].str());
        size_t pbnum = std::stoull(match_seq[4].str());
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
    std::cout << "file name in io:" << filename << std::endl;
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
    using FmtProvider = std::string;
    using LowerFmtMap = std::unordered_map<int, FmtProvider>;
    using UpperFilter  = std::unordered_map<OpKind, LowerFmtMap>;
    using FmtMapper = UpperFilter;
private:
    static std::ofstream Out;
    static FmtMapper Index;

public:

    void write(OpKind kind, int ret_code, std::optional<std::string> appendix)
    {
        static bool once_flag{ false };
        
        // lazy init
        if (not once_flag)
            init_mapper(), once_flag = true;
        
        // format out put 
        std::string result = Index.at(kind).at(ret_code);

        // append others
        if (appendix.has_value())
            result.push_back('\n'), result.append(appendix.value());

        for (auto it = result.rbegin(); it != result.rend(); it++)
            if (*it == '\n')
                result.pop_back();
            else
                break;
        
#ifdef LOCAL_TEST
#endif // LOCAL_TEST
        // avoid blank line 
        if (result.size() > 3U)
            std::cout << result << std::endl;;
    }

    static void init_mapper()
    {
        LowerFmtMap addteam, start, submit, flush, freeze, scroll, querysub, queryrank, end;

        addteam.emplace(1,  "[Info]Add successfully.");
        addteam.emplace(2,  "[Error]Add failed: competition has started.");
        addteam.emplace(3,  "[Error]Add failed : duplicated team name.");

        start.emplace(1, "[Info]Competition starts.");
        start.emplace(2, "[Error]Start failed: competition has started.");

        submit.emplace(1, "");  // is blank
        
        flush.emplace(1, "[Info]Flush scoreboard.");

        freeze.emplace(1, "[Info]Freeze scoreboard.");
        freeze.emplace(2, "[Error]Freeze failed : scoreboard has been frozen.");

        scroll.emplace(1, "[Info]Scroll scoreboard.");
        scroll.emplace(2, "[Error]Scroll failed: scoreboard has not been frozen.");

        queryrank.emplace(1, "[Error]Query ranking failed: cannot find the team.");
        queryrank.emplace(2, "[Info]Complete query ranking.");
        queryrank.emplace(3, "[Info]Complete query ranking.\n[Warning]Scoreboard is frozen. The ranking may be inaccurate until it were scrolled.");

        querysub.emplace(1, "[Info]Complete query submission.");
        querysub.emplace(2, "[Info]Complete query submission.\n[Error]Query submission failed: cannot find the team.");
        querysub.emplace(3, "[Info]Complete query submission.\nCannot find any submission.");

        end.emplace(1, "[Info]Competition ends.");

        // init upper filter
        Index.insert_or_assign(OpKind::AddTeam, std::move(addteam));
        Index.insert_or_assign(OpKind::Start, std::move(start));
        Index.insert_or_assign(OpKind::Submit, std::move(submit));
        Index.insert_or_assign(OpKind::Flush, std::move(flush));
        Index.insert_or_assign(OpKind::Freeze, std::move(freeze));
        Index.insert_or_assign(OpKind::Scroll, std::move(scroll));
        Index.insert_or_assign(OpKind::QuerySub, std::move(querysub));
        Index.insert_or_assign(OpKind::QueryRank, std::move(queryrank));
        Index.insert_or_assign(OpKind::End, std::move(end));

        // fvck!!!  today is also a day that miss macro in Rust ~.~
    }

    static Outputter& SendTo(std::string target) 
    {
        static Outputter put;
        
        if (Out.is_open())
            Out.close();
        Out.open(target, std::ios::out);
        
        return put;
    }
};

// static varibale define here
Output::Outputter::FmtMapper Output::Outputter::Index{};
std::ofstream Output::Outputter::Out{};
    
} // namespace Output
