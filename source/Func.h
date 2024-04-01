#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>


//函数声明
void BFS_Connected
(int board[SIZE][SIZE],int n, bool isRed,
vector<vector<int>> &visit,
int x,int y,connectedGraph& CG);

bool inBoard(int n,int x,int y);