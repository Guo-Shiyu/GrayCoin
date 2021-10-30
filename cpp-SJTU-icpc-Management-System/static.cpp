#include "system.hpp"

ProcMap SystemProxy::Map {};

void SystemProxy::init_map()
{
    SystemProxy::Map.emplace(OpKind::AddTeam,
        [](SystemCore& core, const Instruction::ParamPack& pack) ->Result {
            auto [name] = std::get<AddTeamArg>(pack);
            if (core.has_started())   // fa: competition has start 
                return Result(2, std::nullopt);
            else if (core.get_team_by_name(name).has_value())   // fa: name has been used
                return Result(3, std::nullopt);
            else    // su
            {
                auto newone = Team::new_team(name);
                core.teams().insert(newone);
                return Result(1, std::nullopt);
            }
        });

    SystemProxy::Map.emplace(OpKind::Start,
        [](SystemCore& core, const Instruction::ParamPack& pack) ->Result {
            auto [dura, pbcount] = std::get<StartGameArg>(pack);
            if (core.has_started())   // fa
                return Result(2, std::nullopt);
            else    // su 
            {
                core.start_competition(dura, pbcount);
                return Result(1, std::nullopt);
            }
        });

    SystemProxy::Map.emplace(OpKind::Submit,
        [](SystemCore& core, const Instruction::ParamPack& pack) ->Result {
            auto [which, who, state, when] = std::get<SubmitArg>(pack);
            


        });
    SystemProxy::Map.emplace(OpKind)

    // other kind ... 

}