#include "./base.hpp"
#include <deque>
#include <fstream>
#include <unordered_map>
#include <optional>
#include <regex>

namespace Input
{
using InputSeq = std::deque<Command>;

// deal with a string and return cmd  
Command GenCommandFromLine(std::string)
{
    static std::unordered_map<OpKind, std::string> HashInput {};
    static bool init_flag{ false };
    if (not init_flag)
    {
        HashInput.emplace(OpKind::AddTeam, R"(^(ADDTEAM)\s?(\w+)$)");
        HashInput.emplace(OpKind::AddTeam)
    }
}

// read a input file, return all command in a vector ( in order )
InputSeq ReadCommandFromFile(std::string filename)
{
    InputSeq ret {};
    std::ifstream in{ filename, std::ios::in };

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
