//============================================================================
// Name        : main.cpp
// Author      : Vishwajeet Thoke
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <bits/stdc++.h>
using namespace std;

enum BallType {
	NORMAL = 1, WIDE_BALL = 2, NO_BALL = 3, WICKET = 4
};

class Run {
private:
	int runs;
public:
	void addRuns(int runs) {
		this->runs = runs;
	}

	int getRuns() {
		return runs;
	}
};

class Ball {
private:
	BallType ballType;
	Run* run;
public:
	void addBallType(BallType ballType) {
		this->ballType = ballType;
	}

	BallType getBallType() {
		return ballType;
	}

	void addRun(Run* run, int runs) {
		this->run = run;
		this->run->addRuns(runs);
	}

	Run* getRun() {
		return run;
	}
};

class PlayerStat {
private:
	int score;
	int noOfFours;
	int noOfSixes;
	int noOfBallsFaced;
public:
	PlayerStat(int score, int noOfFours, int noOfSixes, int noOfBallsFaced) {
		this->score += score;
		this->noOfFours += noOfFours;
		this->noOfSixes += noOfSixes;
		this->noOfBallsFaced += noOfBallsFaced;
	}

	int getNoOfBallsFaced() const {
		return noOfBallsFaced;
	}

	void setNoOfBallsFaced(int noOfBallsFaced) {
		this->noOfBallsFaced += noOfBallsFaced;
	}

	int getNoOfFours() const {
		return noOfFours;
	}

	void setNoOfFours(int noOfFours) {
		this->noOfFours += noOfFours;
	}

	int getNoOfSixes() const {
		return noOfSixes;
	}

	void setNoOfSixes(int noOfSixes) {
		this->noOfSixes += noOfSixes;
	}

	int getScore() const {
		return score;
	}

	void setScore(int score) {
		this->score += score;
	}
};

class Player {
private:
	string name;
	map<string, PlayerStat*> playerStats;
public:
	Player(string name) {
		this->name = name;
	}

	string getPlayerName() {
		return name;
	}

	void addPlayerStat(string name, int score = 0, int fours = 0, int sixes = 0,
			int noOfBallsFaced = 0) {
		if (playerStats.find(name) == playerStats.end()) {
			playerStats[name] = new PlayerStat(score, fours, sixes,
					noOfBallsFaced);
		} else {
			PlayerStat* playerStat = playerStats[name];
			playerStat->setNoOfBallsFaced(noOfBallsFaced);
			playerStat->setNoOfFours(fours);
			playerStat->setNoOfSixes(sixes);
			playerStat->setScore(score);
			playerStats[name] = playerStat;
		}
	}

	PlayerStat* getPlayerStat(string name) {
		return playerStats[name];
	}
};

class Team {
private:
	string name;
	map<string, Player*> playersMap;
	map<string, list<Player*> > playerTeamMap;
	deque<Player*> playerQueue;
public:
	Team(string name) {
		this->name = name;
	}

	string getTeamName() {
		return name;
	}

	map<string, Player*> getPlayersMap() {
		return playersMap;
	}

	void setPlayersMap(string name, Player* player) {
		playersMap[name] = player;
	}

	map<string, list<Player*> > getPlayersTeamMap() {
		return playerTeamMap;
	}

	void setPlayersTeamMap(string name, list<Player*> players) {
		playerTeamMap[name] = players;
	}

	deque<Player*> getPlayerQueue() {
		return playerQueue;
	}

	void setPlayerQueue(Player* player) {
		playerQueue.push_back(player);
	}

	void modifyPlayerQueue(int index, Player* player) {
		playerQueue[index] = player;
	}

	int getPlayerQueueSize() {
		return playerQueue.size();
	}

	void removeBatsMan() {
		playerQueue.pop_front();
	}

	void changePlayerStrike() {
		int playerQueueSize = getPlayerQueueSize();
		if (playerQueueSize > 1) {
			Player* tempPlayer;
			tempPlayer = playerQueue[0];
			playerQueue[0] = playerQueue[1];
			playerQueue[1] = tempPlayer;
		}
	}
};

class Over {
private:
	int number;
	list<Ball> balls;
public:
	Over(int number) {
		this->number = number;
	}

	list<Ball> getBalls() {
		return balls;
	}

	void addBall(Ball &ball) {
		balls.push_back(ball);
	}

	int printOverStats(Team* team) {
		cout << "Scorecard for team: " << team->getTeamName() << endl;
		cout << "Name\t" << "Score\t" << "4s\t" << "6s\t" << "Balls\t" << endl;
		int totalRuns = 0;
		for (auto it : team->getPlayersMap()) {
			PlayerStat* playerStat = it.second->getPlayerStat(it.first);
			totalRuns += playerStat->getScore();
			cout << it.first << "\t" << playerStat->getScore() << "\t"
					<< playerStat->getNoOfFours() << "\t"
					<< playerStat->getNoOfSixes() << "\t"
					<< playerStat->getNoOfBallsFaced() << endl;
		}
		return totalRuns;
	}
};

class Inning {
private:
	int number;
	int noOfOvers;
	list<Over> overs;
	int totalScore;
	int totalWickets;
	int totalBalls;
public:
	Inning(int number, int noOfOvers, int totalScore, int totalWickets,
			int totalBalls) {
		this->number = number;
		this->noOfOvers = noOfOvers;
		this->totalScore = totalScore;
		this->totalWickets = totalWickets;
		this->totalBalls = totalBalls;
	}

	void addOver(Over &over) {
		overs.push_back(over);
	}

	void setTotalScore(int totalScore) {
		this->totalScore = totalScore;
	}

	int getTotalScore() {
		return totalScore;
	}

	void setTotalBalls(int totalBalls) {
		this->totalBalls = totalBalls;
	}

	int getTotalBalls() {
		return totalBalls;
	}

	int getTotalWickets() {
		return totalWickets;
	}

	void setTotalWickets(int totalWickets) {
		this->totalWickets += totalWickets;
	}

	void startInning(Team *team,
			unordered_map<string, BallType> inputBallTypeMap) {
		bool isInningOver = false;
		int totalScore = 0;
		for (int i = 1; i <= noOfOvers; i++) {
			cout << "Over: " << i << endl;
			Over over(i);
			int ballNumber = 1;
			while (ballNumber <= 6) {
				if (team->getPlayerQueueSize() < 2) {
					isInningOver = true;
					break;
				}
				Player* firstPlayer = team->getPlayerQueue().front();
				string ballType;
				Ball ball;
				cin >> ballType;
				switch (inputBallTypeMap[ballType]) {
				case WIDE_BALL:
					ball.addBallType(WIDE_BALL);
					ball.addRun(new Run, 1);
					over.addBall(ball);
					firstPlayer->addPlayerStat(firstPlayer->getPlayerName(), 1,
							0, 0, 1);
					team->modifyPlayerQueue(0, firstPlayer);
					team->setPlayersMap(firstPlayer->getPlayerName(),
							firstPlayer);
					break;
				case WICKET:
					ball.addBallType(WICKET);
					ball.addRun(new Run, 0);
					over.addBall(ball);
					firstPlayer->addPlayerStat(firstPlayer->getPlayerName(), 0,
							0, 0, 1);
					team->modifyPlayerQueue(0, firstPlayer);
					team->setPlayersMap(firstPlayer->getPlayerName(),
							firstPlayer);
					team->removeBatsMan();
					ballNumber++;
					break;
				case NO_BALL:
					ball.addBallType(NO_BALL);
					ball.addRun(new Run, 1);
					over.addBall(ball);
					firstPlayer->addPlayerStat(firstPlayer->getPlayerName(), 0,
							0, 0, 1);
					team->modifyPlayerQueue(0, firstPlayer);
					team->setPlayersMap(firstPlayer->getPlayerName(),
							firstPlayer);
					break;
				case NORMAL:
					ball.addBallType(NORMAL);
					ball.addRun(new Run, stoi(ballType));
					over.addBall(ball);
					switch (stoi(ballType)) {
					case 0:
						firstPlayer->addPlayerStat(firstPlayer->getPlayerName(),
								0, 0, 0, 1);
						team->modifyPlayerQueue(0, firstPlayer);
						team->setPlayersMap(firstPlayer->getPlayerName(),
								firstPlayer);
						break;
					case 1:
						firstPlayer->addPlayerStat(firstPlayer->getPlayerName(),
								1, 0, 0, 1);
						team->modifyPlayerQueue(0, firstPlayer);
						team->setPlayersMap(firstPlayer->getPlayerName(),
								firstPlayer);
						team->changePlayerStrike();
						break;
					case 2:
						firstPlayer->addPlayerStat(firstPlayer->getPlayerName(),
								2, 0, 0, 1);
						team->modifyPlayerQueue(0, firstPlayer);
						team->setPlayersMap(firstPlayer->getPlayerName(),
								firstPlayer);
						break;
					case 3:
						firstPlayer->addPlayerStat(firstPlayer->getPlayerName(),
								3, 0, 0, 1);
						team->modifyPlayerQueue(0, firstPlayer);
						team->setPlayersMap(firstPlayer->getPlayerName(),
								firstPlayer);
						team->changePlayerStrike();
						break;
					case 4:
						firstPlayer->addPlayerStat(firstPlayer->getPlayerName(),
								4, 1, 0, 1);
						team->modifyPlayerQueue(0, firstPlayer);
						team->setPlayersMap(firstPlayer->getPlayerName(),
								firstPlayer);
						break;
					case 6:
						firstPlayer->addPlayerStat(firstPlayer->getPlayerName(),
								6, 0, 1, 1);
						team->modifyPlayerQueue(0, firstPlayer);
						team->setPlayersMap(firstPlayer->getPlayerName(),
								firstPlayer);
						break;
					}
					ballNumber++;
					break;
				default:
					cout << "Invalid Input, Please try again..." << endl;
					continue;
				}
			}

			this->addOver(over);
			totalScore = over.printOverStats(team);
			int totalWickets = this->getTotalWickets();
			for (auto it : over.getBalls()) {
				if (it.getBallType() == WICKET) {
					totalWickets++;
				}
			}
			this->setTotalBalls(this->getTotalBalls() + ballNumber - 1);
			int totalBalls = this->getTotalBalls();
			this->setTotalWickets(totalWickets);
			cout << "Total: " << totalScore << "/" << totalWickets << endl;
            cout<< "Overs: "<<totalBalls/6<<"."<<totalBalls%6<<endl;
		if (isInningOver)
		break;
	}
	this->setTotalScore(totalScore);
}
};

class Match {
private:
	string name;
	vector<Inning*> innings;
public:
	Match(string name) {
		this->name = name;
	}
	void addInning(Inning* inning) {
		innings.push_back(inning);
	}

	void printMatchStatus() {
		int diff = innings[0]->getTotalScore() - innings[1]->getTotalScore();
		if (diff > 0) {
			cout << "Result: Team-1 won the match by " << abs(diff) << " runs"
					<< endl;
		} else if (diff < 0) {
			cout << "Result: Team-2 won the match by " << -abs(diff) << " runs"
					<< endl;
		} else {
			cout << "Result: Match Draw: " << endl;
		}
	}
};

void registerBallTypes(unordered_map<string, BallType> &inputBallTypeMap) {
	inputBallTypeMap["w"] = WICKET;
	inputBallTypeMap["wb"] = WIDE_BALL;
	inputBallTypeMap["nb"] = NO_BALL;
	inputBallTypeMap["0"] = NORMAL;
	inputBallTypeMap["1"] = NORMAL;
	inputBallTypeMap["2"] = NORMAL;
	inputBallTypeMap["3"] = NORMAL;
	inputBallTypeMap["4"] = NORMAL;
	inputBallTypeMap["6"] = NORMAL;
}

int main() {

	int noOfPlayersInTeam;
	int noOfOvers;
	int noOfInnings = 2;
	unordered_map<string, BallType> inputBallTypeMap;

	cout << "Number of players for each team: ";
	cin >> noOfPlayersInTeam;

	cout << "Number of overs: " << endl;
	cin >> noOfOvers;

	registerBallTypes(inputBallTypeMap);
	Match* match = new Match("IndiaVsEngland");

	for (int i = 1; i <= noOfInnings; i++) {
		Inning* inning = new Inning(i, noOfOvers, 0, 0, 0);
		string teamName = "Team-" + to_string(i);
		Team *team = new Team(teamName);
		cout << "Batting order for " << teamName << endl;
		for (int i = 0; i < noOfPlayersInTeam; i++) {
			string playerName;
			cin >> playerName;
			Player* player = new Player(playerName);
			player->addPlayerStat(playerName);
			team->setPlayersMap(playerName, player);
			team->setPlayerQueue(player);
		}
		inning->startInning(team, inputBallTypeMap);
		match->addInning(inning);
	}

	match->printMatchStatus();
	return 0;
}
