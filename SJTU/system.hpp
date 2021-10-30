#include "base.hpp"
#include <map>
#include <functional>
#include <optional>

class SystemCore
{
private:
    // vec> teams_;
    // vec<Problems> pblms_;

public:

};

// condition code + optional appendix 
using Result = std::tuple<int, std::optional<std::string>>;

// instruction processor map 
using ProcMap = std::map<OpKind, std::function<Result(/*mutable*/ SystemCore&, const Instruction::ParamPack&)>>;

class SystemProxy
{
private:
    SystemCore core_;
    
    static ProcMap Map;
private:
    SystemProxy() = default;
    // Result add_team(const AddTeamArg& arg)
    // {
    //     
    // }

public:

    Result react(const Command& cmd)
    {
        auto& functor = SystemProxy::Map.at(cmd->opcode().as_enum());
        return functor(this->core_, cmd->args());
    }
    // todo diff with local file !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
public:
    // singleton pattern 
    static SystemProxy* instance()
    {
        static SystemProxy proxy;
        return &proxy;
    }
};