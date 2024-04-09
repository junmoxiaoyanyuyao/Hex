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
#include <ctime>

#include "Func.h"

using namespace std;

const int SIZE = 15;
// int board[SIZE][SIZE];//本方1，对方-1，空白0




int main()
{
	
	solve();
	//恢复目前的棋盘信息

	//cin >> n;
	// for (int i = 0; i < n - 1; i++) {
	// 	cin >> x >> y; if (x != -1) board[x][y] = -1;	//对方
	// 	cin >> x >> y; if (x != -1) board[x][y] = 1;	//我方
	// }
	// cin >> x >> y;
	// if (x != -1) board[x][y] = -1;	//对方

	//此时board[][]里存储的就是当前棋盘的所有棋子信息,x和y存的是对方最近一步下的棋

	/************************************************************************************/
	/***********在下面填充你的代码，决策结果（本方将落子的位置）存入new_x和new_y中****************/
	//下面仅为随机策略的示例代码，可删除
	// int avail_x[SIZE*SIZE], avail_y[SIZE*SIZE]; //合法位置表
	// int cnt = 0;
	// for (int i = 0; i < SIZE; i++)
	// 	for (int j = 0; j < SIZE; j++)
	// 		if (board[i][j] == 0) {
	// 			avail_x[cnt] = i;
	// 			avail_y[cnt] = j;
	// 			cnt++;
	// 		}
	// srand(time(0));
	// int rand_pos = rand() % cnt;
	// int new_x = avail_x[rand_pos];
	// int new_y = avail_y[rand_pos];
	// MCTtree* root = new MCTtree(board,nullptr);
	/***********在上方填充你的代码，决策结果（本方将落子的位置）存入new_x和new_y中****************/
	/************************************************************************************/

	// 向平台输出决策结果
	// cout << new_x << ' ' << new_y << endl;
	return 0;
}





//常量
// #define MAX_DISTANCE 0x3f3f3f3f
// enum STATUS{EMPTY,DEAD,RED,RED_CAPTURED,RED_DEAD,BLUE,BLUE_CAPTURED,BLUE_DEAD}; //状态
// int dx[6] = {0,-1,-1,0,1,1}; //方向
// int dy[6] = {-1,0,1,1,0,-1};

// //声明

// void solve();
// //设置
// void FileSet(int &x,int &y);
// //广搜计算连通
// void BFS_Connected
// (vector<vector<STATUS>>& board,bool isRed,vector<vector<bool>> &visit,int x,int y,
// ConnectedGraph& CG,vector<vector<int>>& Dist_AXIS,vector<vector<int>>& Dist_Far_AXIS);
// //广搜初始化
// void BFS_Init
// (vector<vector<STATUS>>& board,vector<vector<bool>> &visit,bool isRed,
// ConnectedGraph& CG,vector<vector<int>>& Dist_AXIS,vector<vector<int>>& Dist_Far_AXIS);
// //判断在棋盘中
// bool inBoard(int x,int y);
// //状态初始化
// void Board_Init(vector<vector<STATUS>>& board,int BOARD[SIZE][SIZE],bool isRed,ConnectedGraph& CG_Red,ConnectedGraph& CG_Blue);
// //初始化计算距离表
// void Distance_Init(vector<vector<STATUS>> &board,vector<vector<int>> &Dist,bool isRed,bool isAXIS);

//连通分量
// class ConnectedGraph{
//     public:
//     vector<vector<pair<int,int>>> ConnectedPoint;
//     map<pair<int,int>,int> GraphPosition;
//     vector<int> Dist_AXIS;
//     vector<int> Dist_Far_AXIS;

//     ConnectedGraph(); //构造
//     ConnectedGraph(ConnectedGraph& C); //拷贝构造
// };

// ConnectedGraph::ConnectedGraph(){

// }

// ConnectedGraph::ConnectedGraph(ConnectedGraph& C){

// }




// void solve(){

// }

// void BFS_Connected
// (vector<vector<STATUS>>& board,bool isRed,vector<vector<bool>> &visit,int x,int y,
// ConnectedGraph& CG,vector<vector<int>>& Dist_AXIS,vector<vector<int>>& Dist_Far_AXIS){
//     STATUS status = isRed ? RED : BLUE;
//     int NUM = CG.ConnectedPoint.size();
//     vector<pair<int,int>> tempV;
//     visit[x][y] = true;
//     queue<pair<int,int>> q;
//     q.push(make_pair(x,y));
// 	CG.Dist_AXIS.push_back(MAX_DISTANCE);
// 	CG.Dist_Far_AXIS.push_back(MAX_DISTANCE);
//     while(!q.empty()){
//         pair<int,int> point = q.front();
//         q.pop();
// 		tempV.push_back(point);
// 		CG.Dist_AXIS[NUM] = min(CG.Dist_AXIS[NUM],Dist_AXIS[point.first][point.second]);
// 		CG.Dist_Far_AXIS[NUM] = min(CG.Dist_Far_AXIS[NUM],Dist_Far_AXIS[point.first][point.second]);
//         for(int i = 0;i < 6;i++){
//             int tempX = point.first + dx[i];
//             int tempY = point.second + dy[i];
//             if(inBoard(tempX,tempY) && board[tempX][tempY] == status && visit[tempX][tempY] == false){
//                 pair<int,int> tempPoint = make_pair(tempX,tempY);
//                 q.push(tempPoint);
// 				visit[tempX][tempY] = true;
//             }
//         }
//     }
// 	CG.ConnectedPoint.push_back(tempV);
//     return;
// }

// void BFS_Init
// (vector<vector<STATUS>>& board,vector<vector<bool>> &visit,bool isRed,
// ConnectedGraph& CG,vector<vector<int>>& Dist_AXIS,vector<vector<int>>& Dist_Far_AXIS){
//     STATUS status = isRed ? RED : BLUE;
//     for(int i = 0;i < BOARD_SIZE;i++){
//         for(int j = 0;j < BOARD_SIZE;j++){
//             if(board[i][j] == status && visit[i][j] == false){
//                 BFS_Connected(board,isRed,visit,i,j,CG,Dist_AXIS,Dist_Far_AXIS);
//             }
//         }
//     }
//     return;
// }

// // int DFS_Distance(vector<vector<STATUS>> &board,vector<vector<int>>& Dist,int x,int y,bool isRed,bool isAXIS){
// //     if(inBoard(x,y) && Dist[x][y] >= 0)
// //         return Dist[x][y];
// //     if(isRed){
// //         if(isAXIS){
// //             if(y == -1)
// //                 return -1;
// //             if(board[x][y] == BLUE || x < 0 || x >= BOARD_SIZE)
// //                 return MAX_DISTANCE;
// //             int dist_0 = DFS_Distance(board,Dist,x + dx[0],y + dy[0],isRed,isAXIS) + 1;
// //             int dist_1 = DFS_Distance(board,Dist,x + dx[1],y + dy[1],isRed,isAXIS) + 1;
// //             int dist_2 = DFS_Distance(board,Dist,x + dx[2],y + dy[2],isRed,isAXIS) + 1;
// //             int dist_3 = DFS_Distance(board,Dist,x + dx[3],y + dy[3],isRed,isAXIS) + 1;
// //             int min_dist = min(min(dist_0,dist_1),min(dist_2,dist_3));
// //             Dist[x][y] = min_dist;
// //             return min_dist;
// //         }
// //         else{

// //         }
// //     }
// //     else{
// //         if(isAXIS){

// //         }
// //         else{

// //         }
// //     }
// // }

// void Distance_Init(vector<vector<STATUS>> &board,vector<vector<int>> &Dist,bool isRed,bool isAXIS){
//     vector<vector<int>> dp(BOARD_SIZE,vector<int>(BOARD_SIZE,-1));
//     if(isRed){
//         if(isAXIS){
//             for(int j = 0;j < BOARD_SIZE;j++){ //初始化第一行
//                 Dist[0][j] = (board[0][j] == BLUE) ? MAX_DISTANCE : 0;
//             }
//             for(int i = 1;i < BOARD_SIZE;i++){
//                 stack<pair<int,int>> s;
//                 bool hasNoMatch = false;
//                 for(int j = 0;j < BOARD_SIZE;j++){
//                     if((!inBoard(i + dx[1],j + dy[1]) || board[i + dx[1]][j + dy[1]] == BLUE) && 
// 					   (!inBoard(i + dx[2],j + dy[2]) || board[i + dx[2]][j + dy[2]] == BLUE)){
//                         s.push({i,j});
//                         hasNoMatch = true;
//                     }
//                     else{
//                         Dist[i][j] = 1 + min(Dist[i + dx[1]][j + dy[1]],Dist[i + dx[2]][j + dy[2]]);
//                         if(hasNoMatch){
//                             while(!s.empty()){
//                                 auto temp = s.top();
//                                 s.pop();
//                                 Dist[temp.first][temp.second] = Dist[temp.first + dx[3]][temp.second + dy[3]];
//                             }
//                             hasNoMatch = false;
//                         }
//                     }
//                 }
//             }
//         }
//         else{
//             for(int j = 0;j < BOARD_SIZE;j++){
//                 Dist[BOARD_SIZE - 1][j] = (board[BOARD_SIZE - 1][j] == BLUE) ? MAX_DISTANCE : 0;
//             }
//             for(int i = BOARD_SIZE - 2;i >= 0;i--){
//                 stack<pair<int,int>> s;
//                 bool hasNoMatch = false;
//                 for(int j = BOARD_SIZE - 1;j >= 0;j--){
//                     if((!inBoard(i + dx[4],j + dy[4]) || board[i + dx[4]][j + dy[4]] == BLUE) && 
// 					   (!inBoard(i + dx[5],j + dy[5]) || board[i + dx[5]][j + dy[5]] == BLUE)){
//                         s.push({i,j});
//                         hasNoMatch = true;
//                     }
//                     else{
//                         Dist[i][j] = 1 + min(Dist[i + dx[4]][j + dy[4]],Dist[i + dx[5]][j + dy[5]]);
//                         if(hasNoMatch){
//                             while(!s.empty()){
//                                 auto temp = s.top();
//                                 s.pop();
//                                 Dist[temp.first][temp.second] = Dist[temp.first + dx[0]][temp.second + dy[0]];
//                             }
//                             hasNoMatch = false;
//                         }
//                     }
//                 }
//             }			
//         }
//     }
//     else{
//         if(isAXIS){

//         }
//         else{

//         }
//     }
//     return;
// }

// bool inBoard(int x,int y){
//     return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
// }

// void Board_Init(vector<vector<STATUS>>& board,int BOARD[SIZE][SIZE],bool isRed,ConnectedGraph& CG_Red,ConnectedGraph& CG_Blue){
//     for(int i = 0;i < MAX_DISTANCE;i++){
//         for(int j = 0;j < MAX_DISTANCE;j++){
//             switch(BOARD[i][j]){
//                 case 0 :
//                     board[i][j] = EMPTY;
//                     break;
//                 case 1 :
//                     board[i][j] = RED;
//                     break;
//                 case -1 :
//                     board[i][j] = BLUE;
//                     break;
//                 default : break;
//             }
//         }
//     }

// 	//计算距离表
//     vector<vector<int>> Dist_AXIS_Red(BOARD_SIZE,vector<int>(BOARD_SIZE,0));
// 	Distance_Init(board,Dist_AXIS_Red,true,true);

//     vector<vector<int>> Dist_Far_AXIS_Red(BOARD_SIZE,vector<int>(BOARD_SIZE,0));
// 	Distance_Init(board,Dist_Far_AXIS_Red,true,false);

//     vector<vector<int>> Dist_AXIS_Blue(BOARD_SIZE,vector<int>(BOARD_SIZE,0));
// 	Distance_Init(board,Dist_AXIS_Blue,false,true);

//     vector<vector<int>> Dist_Far_AXIS_Blue(BOARD_SIZE,vector<int>(BOARD_SIZE,0));
// 	Distance_Init(board,Dist_Far_AXIS_Blue,false,false);

// 	//计算连通
//     vector<vector<bool>> visit(BOARD_SIZE,vector<bool>(BOARD_SIZE,false));
//     BFS_Init(board,visit,true,CG_Red,Dist_AXIS_Red,Dist_Far_AXIS_Red);
//     BFS_Init(board,visit,false,CG_Blue,Dist_AXIS_Blue,Dist_Far_AXIS_Blue);


//     return;
// }