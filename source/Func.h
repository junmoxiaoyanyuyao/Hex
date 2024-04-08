#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

const int BOARD_SIZE = 11;
const int MAX_DISTANCE = 200; //最大距离
const int MWS = 1; //窗口区间
const int BRANCH = 8; //分支系数
const int MAX_DEPTH = 10; //最大搜索深度
const int C = 2;

class MCTtree{
public:
	MCTtree* parent;
	vector<MCTtree*> children;
	double value;
	int ni;//每个结点模拟了多少次
	double ucb;
    pair<int,int> decision;
	vector<vector<int>> board;
    vector<vector<vector<int>>> Dist;

	MCTtree(MCTtree* Parent);
	MCTtree(vector<vector<int>>& board,MCTtree* Parent);
	// MCTtree(const MCTtree& tree);

	// MCTtree* SelectMaxUCBNode();
	// void rollout();
	// void Expand();
	// position getBestDecision();
	// vector<MCTtree*>& getchild();
	// double& getNi();

	// void Back(int);
	void CalculateUCB();
};

void solve();
bool inBoard(pair<int,int> position);

vector<pair<int,int>> get_neighbour(const vector<vector<int>>& board,const vector<vector<int>>& visit,bool isRed,pair<int,int> position);