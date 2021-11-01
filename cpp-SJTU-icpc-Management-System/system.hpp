#include "base.hpp"
#include "class.h"
#include <tuple>
#include <set>
#include <map>
#include <deque>
#include <optional>
#include <functional>
#include <algorithm>
#include <format>

#include <cassert>

class SystemCore
{
private:

	// variable table 
	std::set<TeamInfo> teams_;      // todo: perhaps hash faster
	std::set<ProblemInfo> pblms_;
	std::vector<SubmitInfo> submits_;

	// competition info 
	size_t dura_;
	std::deque<TeamInfo> rank_;
	size_t last_freeze_;

	// competition state
	bool started_;
	bool frozen_;

private:
	SystemCore(const SystemCore&) = delete;

public:
	SystemCore() : teams_(), pblms_(), submits_(), dura_(0U), rank_(), started_(false), frozen_(false) {}
	std::set<TeamInfo>&		teams() { return this->teams_; }
	std::set<ProblemInfo>&	pblms() { return this->pblms_; }
	decltype(submits_)&		submits() { return this->submits_; }
	std::deque<TeamInfo>&	rank() { return this->rank_; }
	size_t	last_freeze() const { return this->last_freeze_; }

	void start_competition(size_t dura, size_t problem_count)
	{
		assert(started_ == false);    // check for test

		// blip 'start_' flag to true 
		started_ = !started_;

		// set 'dura_'
		dura_ = dura;

		// init problems table
		for (size_t i = 0; i < problem_count; i++)
		{
			auto newprob = Problem::new_problem((char)('A' + i));
			pblms_.insert(newprob);
		}

		// init current rank
		for (auto& team : teams_)
			this->rank_.push_back(team);

		// sort rank by dictionary 
		std::sort(rank_.begin(), rank_.end(), [](const TeamInfo& front, const TeamInfo& back) { return front->name() < back->name(); });
	}


	std::optional<TeamInfo> get_team_by_name(const std::string& name)
	{
		auto ret = std::find_if(this->teams_.begin(), this->teams_.end(),
			[&](const TeamInfo& info) { return info->name() == name; });
		return ret == this->teams_.end() ? std::nullopt : std::make_optional(*ret);
	}

	std::optional<ProblemInfo> get_problem_by_name(const char name)
	{
		auto ret = std::find_if(this->pblms_.begin(), this->pblms_.end(),
			[&](const ProblemInfo& info) { return info->name() == name; });
		return ret == this->pblms_.end() ? std::nullopt : std::make_optional(*ret);
	}

	bool has_started() { return this->started_; }

	bool in_frozen() { return this->frozen_; }

	void blip_freeze_flag(size_t when = 0) { frozen_ = !frozen_; if (when != 0) last_freeze_ = when; }

};

// condition code + optional appendix 
using Result	= std::tuple<int, std::optional<std::string>>;

// process each instruction
using Reactor	= std::function<Result(/*mutable*/ SystemCore&, const Instruction::ParamPack&)>;

// instruction processor map 
using ProcMap	= std::map<OpKind, Reactor>;

class SystemProxy
{
private:
	SystemCore core_;

	static ProcMap Map;
private:
	SystemProxy() = default;

public:

	Result react(const Command& cmd)
	{
		auto& functor = SystemProxy::Map.at(cmd->opcode().as_enum());
		return std::invoke(functor, this->core_, cmd->args());
	}

	std::string on_query_submit(const SubmitInfo& sub)
	{
		return std::format("{} {} {} {}", sub->team()->name(), sub->problem()->name(), sub->status().as_str(), sub->time());
	}

	std::string on_query_rank(size_t rank, const TeamInfo& team)
	{
		std::string ret = std::format("{} NOW AT RANKING {}", team->name(), rank + 1); // 0 -> 1
		ret.push_back('\n');
		return ret;
	}

	std::string on_scroll(const TeamInfo& old, const TeamInfo& newone)
	{
		return std::format("{} {} {} {}", newone->name(), old->name(), newone->score(), newone->calculate_penalty());
	}

	std::string rank_as_str()
	{
		auto fmt_team = [&](const TeamInfo& team) -> std::string
		{
			static std::string buf;
			buf.clear();

			// append base info
			buf = std::format(
				"{} {} {} {}",
				team->name(),
				std::distance(
					core_.rank().begin(),
					std::find_if(core_.rank().begin(), core_.rank().end(),
						[&](const TeamInfo& target) { return target->name() == team->name(); })) + 1,	// 0 -> 1
				team->score(),
				team->calculate_penalty()
			);

			// some help func 
			auto fmt_frozen_pb = [](size_t before, size_t after) -> std::string 
			{
				std::string buf;
				if (before != 0)
					buf.push_back('-');
				return buf.append(std::format("{}/{}", before, after));
			};

			auto unac_num_berfore_freeze = [](const TeamInfo& team, const ProblemInfo& pb, size_t fzetime) -> size_t
			{
				size_t count = 0;
				for (auto& sub : team->record_ref())
				{
					if (sub->problem()->name() == pb->name())
						if (sub->time() <= fzetime)
						{
							if (not sub->status().is_accepted())
								count++;
						}
						else
							break;
				}
				return count;
			};

			auto sub_num_after_freeze = [](const TeamInfo& team, const ProblemInfo& pb, size_t fzetime) -> size_t
			{
				size_t count = 0;
				for (auto& sub : team->buf_ref())
					if (sub->time() > fzetime)
						if (sub->problem()->name() == pb->name())
							count++;

				return count;
			};

			auto wrong_num_before_fstac = [](const TeamInfo& team, const ProblemInfo& pb, size_t fzetime) -> size_t
			{
				size_t count = 0;
				for (auto& sub : team->record_ref())
					if (sub->problem()->name() == pb->name())
						if (not sub->status().is_accepted())
							count++;
						else
							break;
				return count;
			};

			auto all_wrong_num = [](const TeamInfo& team, const ProblemInfo& pb) -> size_t
			{
				size_t count = 0;
				for (auto& sub : team->record_ref())
					if (sub->problem()->name() == pb->name() && not sub->status().is_accepted())
						count++;
				return count;
			};

			// problem solve info 
			for (size_t i = 0; i < core_.pblms().size(); i++)
			{
				char pbname = (char)('A' + i);
				auto pbinfo = core_.get_problem_by_name(pbname).value();
				buf.push_back(' ');
				auto condition = [&](const SubmitInfo& info) -> bool { return info->problem()->name() == pbname; };
				if (std::find_if(team->buf_ref().begin(), team->buf_ref().end(), condition) != team->buf_ref().end())
				{
					// frozen prob 
					buf.append(
						fmt_frozen_pb(
							unac_num_berfore_freeze(team, pbinfo, core_.last_freeze()),
							sub_num_after_freeze(team, pbinfo, core_.last_freeze())));
				}
				else // unfrozen prob
				{
					// const auto& ac_sub = team->all_ac_sub();

					// use submit info in 'record' but not in 'buf'
					decltype(team->all_ac_sub()) ac_sub;
					for (auto& sub : team->record_ref())
						if (sub->status().is_accepted())
							ac_sub.push_back(sub);
					
					std::string ret;
					if (std::find_if(ac_sub.begin(), ac_sub.end(),
						[&](const SubmitInfo& info) { return info->problem()->name() == pbname; }) != ac_sub.end())
					{
						// has ac this pb 
						ret = "+";
						auto x = wrong_num_before_fstac(team, pbinfo, core_.last_freeze());
						if (x != 0)
							ret.append(std::to_string(x));
					}
					else
					{
						// not through this pb
						auto x = all_wrong_num(team, pbinfo);
						if (x == 0)
							ret = ".";
						else
						{
							ret.push_back('-');
							ret.append(std::to_string(x));
						}
					}
					buf.append(ret);
				}
			}
			return buf;
		};

		std::string ret;
		ret.reserve((core_.pblms().size() + 32U) * core_.teams().size());
		for (auto& t : core_.rank())
			ret.append(fmt_team(t)).push_back('\n');
		return ret;
	}

	static void init_map();

public:
	// singleton pattern 
	static SystemProxy* instance()
	{
		static SystemProxy proxy;
		static bool init_flag{ false };
		if (not init_flag) {

			// call once 
			SystemProxy::init_map();
			init_flag = true;
		}

		return &proxy;
	}
};