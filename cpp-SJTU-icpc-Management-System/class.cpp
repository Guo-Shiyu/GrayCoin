#include"class.h"

TeamInfo Team::new_team(const std::string& name, int score) 
{
	return std::make_shared<Team>(name, score);
}



ProblemInfo Problem::new_problem(const std::string& name)
{
	return std::make_shared<Problem>(name);
}


SubmitInfo Submit::new_submit(TeamInfo& team, ProblemInfo& prob, Status state, size_t time)
{
	return std::make_shared<Submit>(team, prob, state, time);
}

