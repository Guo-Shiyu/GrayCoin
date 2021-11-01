#include"class.h"

void Team::calculate_score()
{
	score_ = 0;
	for (auto& sub : record_)    // history submit
		if (sub->status().is_accepted())
			score_++;
	for (auto& sub : buf_)       // buffer submit
		if (sub->status().is_accepted())
			score_++;
}

Team::SelfDataStructure Team::all_ac_sub()
{
	auto record = record_copy();
	decltype(record) ret{};
	for (auto& sub : record)
		ret.push_back(sub);

	return ret;
}

size_t Team::calculate_penalty()
{
	// P = 20X + Y;
	auto ac_subs = all_ac_sub();
	if (ac_subs.empty())	// no ac any problem
		return (record_.size() + buf_.size()) * 20U;
	else	// ac some problem 
	{
		auto first_ac_sub = ac_subs.front();
		auto Y = first_ac_sub->time();

		auto X = [&]()
		{
			auto ref = record_ref();
			size_t count = 0;
			for (auto it = ref.begin(); it != ref.end(); ++it)
				if (*it != first_ac_sub)
					count++;
				else
					break;

			return count;
		}();

		return 20U * X + Y;
	}
}