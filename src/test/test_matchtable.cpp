#include "tools/MatchTable.hpp"
#include "gtest/gtest.h"

#include <string>
#include <iostream>

struct FootballTeam
{
	FootballTeam() = default;
	FootballTeam(const char *name_) : name(name_)
	{

	}

	std::string name;
};

static void PrintRound(const MatchTable<FootballTeam> &world_cup)
{
	std::cout << "===round " << world_cup.GetRound() << "===\n";
	auto pk_list = world_cup.GetPKList();
	for (const auto &pk_item : pk_list)
	{
		auto item1 = world_cup.GetMatchItem(pk_item.first);
		auto item2 = world_cup.GetMatchItem(pk_item.second);

		std::cout << pk_item.first << (item1 ? item1->name : "") << " vs " << pk_item.second << (item2 ? item2->name : "") << "\n";
	}

	if (world_cup.GetRound() == world_cup.GetMaxRount())
	{
		int winner_index = world_cup.GetWinnerIndex();
		auto winner = world_cup.GetMatchItem(winner_index);

		if (winner)
		{
			std::cout << "winner: " << world_cup.GetWinnerIndex() << winner->name << "\n";
		}
	}
}

TEST(MatchTable, test)
{
	MatchTable<FootballTeam> world_cup;

	FootballTeam team1("巴西");
	FootballTeam team2("法国");
	FootballTeam team3("德国");
	FootballTeam team4("中国");
	FootballTeam team5("英国");
	FootballTeam team6("俄国");
	FootballTeam team7("美国");
	FootballTeam team8("印度");

	{
		world_cup.AddMatchItem(0, team1);
		world_cup.AddMatchItem(1, team2);
		world_cup.AddMatchItem(2, team3);
		world_cup.AddMatchItem(3, team4);
		world_cup.AddMatchItem(4, team5);
		world_cup.AddMatchItem(5, team6);
		world_cup.AddMatchItem(6, team7);
		world_cup.AddMatchItem(7, team8);

		world_cup.StartNextRound();

		PrintRound(world_cup);
	}

	// round 1
	{
		world_cup.SetWinner(0);
		world_cup.SetWinner(3);
		world_cup.SetWinner(4);
		world_cup.SetWinner(6);

		world_cup.StartNextRound();

		PrintRound(world_cup);
	}

	// round 2
	{
		world_cup.SetWinner(3);
		world_cup.SetWinner(6);

		world_cup.StartNextRound();

		PrintRound(world_cup);
	}

	auto pk_3vs4 = world_cup.Get3VS4PKItem();
	std::cout << "3vs4: " << pk_3vs4.first << " " << pk_3vs4.second << "\n";

	// round 3
	{
		world_cup.SetWinner(3);

		world_cup.Set3VS4WinnerAndLoser(4, 0);

		world_cup.StartNextRound();

		PrintRound(world_cup);
	}

	std::cout << "rank: ";
	auto rank = world_cup.GetRank4Item();
	for (int i : rank)
	{
		std::cout << i << " ";
	}
	std::cout << "\n";
}