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

            auto team = core.get_team_by_name(who);
            auto prob = core.get_problem_by_name(which);
            
            // check for test 
            assert(team.has_value());
            assert(prob.has_value());

            // new submission 
            auto newsub = Submit::new_submit(team.value(), prob.value(), state, when);
            
            // record submitssion 
            core.submits().push_back(newsub);
            team.value()->record_ref().push_back(newsub);
            prob.value()->record_ref().push_back(newsub);


        });
    //SystemProxy::Map.emplace(OpKind::Flush,
    //    [](SystemCore& core, const Instruction::ParamPack& pack) -> Result{
    //        std::vector<TeamInfo> copy(teams().begin(),teams.end());
    //        std::sort(copy.begin(),copy.end(),greatermark);
    //    });
    SystemProxy::Map.emplace(OpKind::Freeze,
        [](SystemCore& core, const Instruction::ParamPack& pack) -> Result{
            
        });
    SystemProxy::Map.emplace(OpKind::Scroll,
        [](SystemCore& core, const Instruction::ParamPack& pack) -> Result{
            
        });
//    SystemProxy::Map.emplace(OpKind::QuerySub,
//        [](SystemCore& core, const Instruction::ParamPack& pack) -> Result{
//            auto [teamname,probname,status]=std::get<QueryArg>(pack);
//            if(!core.get_team_by_name(name).has_value()){
//                return Result(2, std::nullopt);
//            }
//            std::vector<TeamInfo> copy(teams().begin(),teams.end());
//            std::sort(copy.begin(),copy.end(),greatermark);
//        });
    SystemProxy::Map.emplace(OpKind::QueryRank,
        [](SystemCore& core, const Instruction::ParamPack& pack) -> Result{
            auto [team] = std::get<QueryRankArg>(pack);

        });
//    SystemProxy::Map.emplace(OpKind::End,
//        [](SystemCore& core, const Instruction::ParamPack& pack) -> Result{
//            
//        });
//
}


////todo submit ¼Óget/get_front_wrong;
//bool greatermark(const Team& team1,const Team& team2){
//    if(team1.score()!=team2.score()){
//        return team1.score()>team2.score();
//    }
//    else{
//        auto ref1=team1.record_ref();
//        auto ref2=team2.record_ref();
//        auto penalty1=ref1.front()->when()+(std::vector<SubmitInfo> ref)[ref1]{
//            for(auto iter:ref1){
//                if(iter.status().isaccepted())
//                    return iter.when();
//            }
//        }*20;
//        auto penalty2=ref2.front()->when()+(std::vector<SubmitInfo> ref)[ref2]{
//            for(auto iter:ref1){
//                if(iter.status().isaccepted())
//                    return iter.when();
//            }
//        }*20;
//        if(penalty1==penalty2){
//            return (std::vector<SubmitInfo> ref1,std::vector<SubmitInfo> ref2)[ref1,ref2]{
//                auto accepttime1=0,accepttime2=0;
//                for(auto iter:ref1){
//                    if(iter.status().isaccepted())
//                        accepttime1=iter.when();
//                }
//                for(auto iter:ref2){
//                    if(iter.status().isaccepted())
//                        accepttime2=iter.when();
//                }
//                if(accepttime1==accepttime2)
//                    return accepttime1>accepttime2;
//                else{
//                        std::sort(ref1.begin(),ref1.end(),(const Submit& submit1,const Submit& submit2){return submit1.when()>submit2.when()});
//                        std::sort(ref2.begin(),ref2.end(),(const Submit& submit1,const Submit& submit2){return submit1.when()>submit2.when()});
//                        for(auto iter1:ref1){
//                            for(auto iter2:ref2){
//                                if(iter1.when()==iter2.when()){
//                                    continue;
//                                }
//                                else{
//                                    return iter1.when()>iter2.when();
//                                }
//                            }
//                        }
//                }
//            }
//            return team1.name()>team2.name();
//        }
//        else{
//            return penalty1>penalty2;
//        }
//    }
//}
//
////