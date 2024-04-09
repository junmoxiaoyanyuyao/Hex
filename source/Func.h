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
const int MAX_DEPTH = 5; //最大搜索深度
const int C = 2;

enum Player{red,blue};

class ConnectedGraph;
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
	bool hasExpand;
	ConnectedGraph* RedGraph;
	ConnectedGraph* BlueGraph;

	MCTtree(MCTtree* Parent);
	MCTtree(vector<vector<int>>& board,MCTtree* Parent);
	MCTtree(const MCTtree& tree,MCTtree* Parent);

	// MCTtree* SelectMaxUCBNode();
	// void rollout();
	// void Expand();
	// position getBestDecision();
	// vector<MCTtree*>& getchild();
	// double& getNi();

	// void Back(int);
	void CalculateUCB();
};

//连通分量
class ConnectedGraph{
    public:
    vector<vector<pair<int,int>>> ConnectedPoint;
    map<pair<int,int>,int> GraphPosition;
	vector<bool> AXIS;
	vector<bool> Far_AXIS;

	ConnectedGraph();
    ConnectedGraph(const ConnectedGraph* C); //拷贝构造
};

void solve();
bool inBoard(pair<int,int> position);

vector<pair<int,int>> get_neighbour(const vector<vector<int>>& board,const vector<vector<int>>& dist,const vector<vector<int>>& visit,bool isRed,pair<int,int> position);


void BFS_Init(const vector<vector<int>>& board,Player isRed,ConnectedGraph& CG);
void Connected_Unit(MCTtree* t,pair<int,int> pos,Player isRed);