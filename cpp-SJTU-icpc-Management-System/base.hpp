#pragma once
#include <string>
#include <tuple>
#include <memory>
#include <optional>
#include <variant>

// pure virtual class marker
#define abstract      

// abstract class iAsStr
// {
// public:
//     // format to output
//     virtual std::string as_str() = 0;
// };

enum class SubStatus : int
{
    A,
    W,
    R,
    T,
};

// wrapper for enum class 'SubStauts'
class Status
{
public:
    SubStatus status_;

    Status() = delete;
    Status(const Status &) = default;
    Status(SubStatus status) : status_(status) {}

    SubStatus as_enum() const { return status_; }
    int as_int() const { return (int)status_; }
    bool is_accepted() const { return status_ == SubStatus::A; }
};

enum class OpKind : int
{
    // command with params are less than zero
    AddTeam = -5,
    Start = -4,
    Submit = -3,
    QueryRank = -2,
    QuerySub = -1,

    // command without params are greater than zero
    Flush = 1,
    Freeze,
    Scroll,
    End,
};

// wrapper for enum class 'OpKind'
class Operation
{
public:
    OpKind kind_;

    Operation() = delete;
    Operation(const Operation &) = default;
    Operation(OpKind kind) : kind_(kind) {}

    OpKind as_enum() const { return kind_; }
    int as_int() const { return (int)kind_; }
};

// name of new team
using AddTeamArg = std::tuple<std::string>;

// competition duration + problem count
using StartGameArg = std::tuple<size_t, size_t>;

// problem name + team name + submit status + submit time
using SubmitArg = std::tuple<std::string, std::string, Status, size_t>;

//// team name
using QueryRankArg = AddTeamArg;

// team name + problem name + submit status
using QuerySubArg = std::tuple<std::string, std::string, Status>;

// base class for input command ( a line of file '*.in' )
class Instruction
{
public:
    using ParamPack = std::variant<AddTeamArg, StartGameArg, SubmitArg, QuerySubArg /*QueryRankArg = AddTeamArg*/>;
private:
    // read-only variable
    Operation opcode_;
    ParamPack args_;

public:
    Instruction() = delete;
    Instruction(const Instruction&) = default;
    Instruction(OpKind no_param_kind) : opcode_(no_param_kind), args_() {}
    Instruction(OpKind with_param_kind, ParamPack&& arg_pack) : opcode_(with_param_kind), args_(arg_pack) {}

    // especiallly 
    Instruction(std::tuple<std::string>&& arg, bool is_addteam) : opcode_(is_addteam? OpKind::AddTeam: OpKind::QueryRank), args_(arg){};

    const Operation& opcode() const { return opcode_; }
    bool has_params() const { return this->opcode_.as_int() < 0; }
    const ParamPack& args() const { return args_; }
};

using Command = std::shared_ptr<Instruction>;