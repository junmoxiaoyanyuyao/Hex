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

map<vector<vector<int>>,MCTtree*> HashMap; //换位表

class MCTtree;
const int BOARD_SIZE = 11;
const int MAX_DISTANCE = 30; //最大距离
const int MWS = 1; //窗口区间
const int BRANCH= 8; //分支系数
const int MAX_DEPTH = 80; //最大搜索深度

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

	MCTtree(vector<vector<int>> board,MCTtree* Parent);
	MCTtree* SelectMaxUCBNode();
	void rollout();
	void Expand();
	// position getBestDecision();
	vector<MCTtree*>& getchild();
	double& getNi();

	void Back(int);
	int get_available_pos(int avail_x[], int avail_y[]);
	// void cloneBoardTo(int[][SIZE]);
	// void CalculateUCB();
	// bool Judgement(int board[][SIZE])
};

void FileSet(vector<vector<int>> &Board);
bool inBoard(pair<int,int> position);
void Double_Distance(const vector<vector<int>>& board,vector<vector<int>>& dist,bool isRed,bool isAXIS);
vector<pair<int,int>> get_neighbour(const vector<vector<int>>& board,const vector<vector<int>>& visit,bool isRed,pair<int,int> position);
void make_move(vector<vector<int>>& board,pair<int,int> position,bool isRed);
void undo_move(vector<vector<int>>& board,pair<int,int> position);
vector<pair<int,int>> generate_move(const vector<vector<int>>& board,const vector<vector<vector<int>>>& Dist,bool isRed,int lim);
int board_evaluate(const vector<vector<int>>& board,vector<vector<vector<int>>>& Dist);