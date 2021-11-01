#include "system.hpp"
#include <ranges>
#include <algorithm>

ProcMap SystemProxy::Map {};

TeamInfo Team::new_team(const std::string& name, int score)
{
    return std::make_shared<Team>(name, score);
}

ProblemInfo Problem::new_problem(const char name)
{
    return std::make_shared<Problem>(name);
}

SubmitInfo Submit::new_submit(TeamInfo& team, ProblemInfo& prob, Status state, size_t time)
{
    return std::make_shared<Submit>(team, prob, state, time);
}

bool Team::compare(const TeamInfo& front, const TeamInfo& back)
{
    // main logic 
    if (front->score() != back->score())
        return front->score() > back->score();
    else    // same score
    {
        auto first = front->calculate_penalty();
        auto second = back->calculate_penalty();
        if (first != second)
            return first > second;
        else  // same punish time 
        {
            auto fst_acs = front->all_ac_sub();
            auto sec_acs = back->all_ac_sub();
            if (fst_acs.size() != sec_acs.size())    
                return fst_acs.size() > sec_acs.size();
            else  // same through problem count
            {
                auto it1 = fst_acs.begin();
                auto it2 = sec_acs.begin();
                for (size_t i = 0; i < fst_acs.size(); i++) // compare time of each through problem 
                {
                    auto& info1 = *it1;
                    auto& info2 = *it2;
                    if (info1->time() != info2->time())
                        return info1->time() > info2->time();
                }

                // finally, compare name by dict-order
                return front->name() > back->name();
            }
        }
    }
}

void SystemProxy::init_map()
{
    SystemProxy::Map.emplace(OpKind::AddTeam,
        [](SystemCore& core, const Instruction::ParamPack& pack) ->Result {
            auto [who] = std::get<AddTeamArg>(pack);

            if (core.has_started())   // fa: competition has start 
                return Result(2, std::nullopt);
            else if (core.get_team_by_name(who).has_value())   // fa: name has been used
                return Result(3, std::nullopt);
            else    // su
            {
                auto newone = Team::new_team(who);
                core.teams().insert(newone);
                return Result(1, std::nullopt);
            }

        });

    SystemProxy::Map.emplace(OpKind::Start,
        [](SystemCore& core, const Instruction::ParamPack& pack) ->Result {
            auto [dura, pbcount] = std::get<StartGameArg>(pack);

            if (core.has_started())   // fa: competition has started 
                return Result(2, std::nullopt);
            else    // su 
            {
                core.start_competition(dura, pbcount);
                return Result(1, std::nullopt);
            }

        });

    SystemProxy::Map.emplace(OpKind::Submit,
        [](SystemCore& core, const Instruction::ParamPack& pack) ->Result 
        {
            auto [which, who, state, when] = std::get<SubmitArg>(pack);

            auto team_opt = core.get_team_by_name(who);
            auto prob_opt = core.get_problem_by_name(which.front());

            // check for test 
            assert(team_opt.has_value());
            assert(prob_opt.has_value());

            auto team = team_opt.value();
            auto prob = prob_opt.value();

            // new submission 
            auto newsub = Submit::new_submit(team, prob, state, when);

            // record_new_submission
            core.submits().push_back(newsub);
            prob->record_ref().push_back(newsub);
            if (core.in_frozen())
                team->buf_ref().push_back(newsub);
            else 
                team->record_ref().push_back(newsub);

            // not recalculate score
            //if (state.is_accepted())
            //    team->incr_score();

            return Result(1, std::nullopt); // su, but no output

        });

    SystemProxy::Map.emplace(OpKind::Flush,
        [](SystemCore& core, const Instruction::ParamPack& pack) -> Result
        {
            if (not core.in_frozen())
            {
                // recalculate score of each team 
                for (auto& team : core.teams())
                    team->calculate_score();

                // sort rank 
                std::sort(core.rank().begin(), core.rank().end(), [](const TeamInfo& front, const TeamInfo& back)
                    {
                        return Team::compare(front, back);
                    });
            }
            
            return Result(1, std::nullopt); // su

        });

    SystemProxy::Map.emplace(OpKind::Freeze,
        [](SystemCore& core, const Instruction::ParamPack& pack) -> Result
        {
            // record freeze time 


            if (core.in_frozen())   // er
                return Result(2, std::nullopt);
            else    // su
            {
                core.blip_freeze_flag(core.submits().back()->time() + 1);   // +1 contains risk
                assert(core.in_frozen());
                return Result(1, std::nullopt);
            }

        });

    SystemProxy::Map.emplace(OpKind::Scroll,
        [](SystemCore& core, const Instruction::ParamPack& pack) -> Result
        {
            // fa: 2, not freeze
            if (not core.in_frozen())
                return Result(2, std::nullopt);
			else
            {
                std::string apdx;

                // scroll one team
                for (auto it = core.teams().rbegin(); it != core.teams().rend(); ++it)
                {
                    auto& team = *it;
                    if (auto& cache = team->buf_ref(); cache.size() > 0) // have submit in freeze 
                    {
                        // min problem id 
                        auto min_it = std::min_element(cache.begin(), cache.end(), [](const SubmitInfo& front, const SubmitInfo& back)
                            {
                                return front->problem()->name() < back->problem()->name();
                            });

                        // unfreeze this problem 
                        team->record_ref().push_back(*min_it);
                        cache.erase(min_it);

                        auto& rank = core.rank();
                        team->calculate_score(); // recalcul score
                        auto cur_pos = std::lower_bound(rank.begin(), rank.end(), team);

                        // sort locally 
                        std::sort(rank.begin(), cur_pos, Team::compare);
                        auto replaced_it = std::lower_bound(rank.begin(), rank.end(), team);
                        if (replaced_it != rank.end())
                            ++replaced_it;
                        if (replaced_it == rank.end())
                            --replaced_it;

                        apdx.append(SystemProxy::instance()->on_scroll(*replaced_it, team)).push_back('\n');
                    }
                }

                return Result(1, apdx);
            }
            assert(core.in_frozen());

            // todo: must change way, ret with appendix
            for (auto& team : core.teams())
            {
                for (auto& cached_sub : team->buf_ref())
                    team->record_ref().push_back(cached_sub);
                team->buf_ref().clear();
                team->calculate_score();
            }
            
            //// unfreeze and flush rank
            //core.blip_freeze_flag();
            //Map.at(OpKind::Flush)(core, Instruction::ParamPack());

            return Result(1, std::nullopt);

        });

    SystemProxy::Map.emplace(OpKind::QuerySub,
        [](SystemCore& core, const Instruction::ParamPack& pack) -> Result
        {
            auto [who, which, state]=std::get<QuerySubArg>(pack);
            
            auto team_opt = core.get_team_by_name(who);
            
            if(not team_opt.has_value())    // er : no such team 
                return Result(2, std::nullopt);
            else
            {
                auto team = team_opt.value();

                // auto view = std::ranges::views::reverse(team->record_copy())
                //             | std::ranges::views::filter([](const SubmitInfo& info) {return info->status().is_accepted(); });
        
                bool any_prob{ false }, any_status{ false };
                if (which == "ALL")
                    any_prob = true;

                if (state.as_enum() == SubStatus::Any)
                    any_status = true;
                 
                auto prob_restriction = [&](const SubmitInfo& info) 
                    {
                        if (any_prob)
                            return true;
                        else 
                            return info->problem()->name() == which.front();
                    };

                auto statu_restriction = [&](const SubmitInfo& info)
                    {
                        if (any_status)
                            return true;
                        else 
                            return info->status().as_enum() == state.as_enum(); 
                    };
                
                std::vector<SubmitInfo> perhaps{};
                perhaps.reserve(team->buf_ref().size() + team->record_ref().size());
                auto reserve_push = [&](const decltype(perhaps)& container)
                {
                    for (auto it = container.cbegin(); it != container.cbegin(); ++it)
                        perhaps.push_back(*it);
                };
                reserve_push(team->buf_ref());
                reserve_push(team->record_ref());

                auto target_iter = std::remove_if(perhaps.begin(), perhaps.end(), [&](const SubmitInfo& info)
                    {
                        return not prob_restriction(info) || not statu_restriction(info);
                    });

                if (target_iter == perhaps.end()) // not an error : can not find any sub    
                    return Result(3, std::nullopt);
                else    // su : find ther target team 
                    return Result(1, SystemProxy::instance()->on_query_submit(*target_iter));
            }

        });

    SystemProxy::Map.emplace(OpKind::QueryRank,
        [](SystemCore& core, const Instruction::ParamPack& pack) -> Result
        {
            auto [who] = std::get<QueryRankArg>(pack);
            
            // find target team
            auto team_opt = core.get_team_by_name(who);

            if (not team_opt.has_value())   // er
                return Result(1, std::nullopt);
            else
            {
                auto team = team_opt.value();
                
                int ret_code{ 0 };
                if (core.in_frozen())   // wa
                    ret_code = 3;
                else    // su
                    ret_code = 2;
                
                // get rank
                auto& rank = core.rank();
                size_t position = std::distance(rank.begin(), std::lower_bound(rank.begin(), rank.end(), team));
                return Result(ret_code, SystemProxy::instance()->on_query_rank(position, team)); // su | wa, with some appendix 
            }

        });

    SystemProxy::Map.emplace(OpKind::End,
        [](SystemCore& core, const Instruction::ParamPack& pack) -> Result
        {
            // do nothing 
            return Result(1, std::nullopt); // su

        });

}


