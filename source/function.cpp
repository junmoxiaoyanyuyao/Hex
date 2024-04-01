#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <queue>

#include "Func.h"
using namespace std;

const int SIZE = 15;

int dx[6] = {-1,-1,0,0,1,1};
int dy[6] = {};

class connectedGraph{
    public:
    vector<vector<pair<int,int>>> connectedPoint;
    unordered_map<pair<int,int>,int> graphPosition;
};

void solve(int n){

}

void BFS_Init_Connected(int board[SIZE][SIZE],int n,bool isRed,vector<vector<int>> &visit,connectedGraph& CG){
    int key = isRed ? 1 : -1;
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            if(visit[i][j] == 0){
                BFS_Connected(board,n,isRed,visit,i,j,CG);
            }
        }
    }
}

void BFS_Connected(int board[SIZE][SIZE],int n,bool isRed,vector<vector<int>> &visit,int x,int y,connectedGraph& CG){

    int key = isRed ? 1 : -1;
    int NUM = CG.connectedPoint.size();
    vector<pair<int,int>> tempV;
    tempV.push_back(make_pair(x,y));
    CG.connectedPoint.push_back(tempV);
    queue<pair<int,int>> q;
    q.push(make_pair(x,y));
    while(!q.empty()){
        pair<int,int> point = q.front();
        q.pop();
        for(int i = 0;i < 6;i++){
            int tempX = point.first + dx[i];
            int tempY = point.second + dy[i];
            if(inBoard(n,tempX,tempY) && board[tempX][tempY] == key && visit[tempX][tempY] == 0){
                pair<int,int> tempPoint = make_pair(tempX,tempY);
                q.push(tempPoint);
                visit[tempX][tempY] = 1;
                CG.connectedPoint[NUM].push_back(tempPoint);
                CG.graphPosition.insert({tempPoint,NUM});
            }
        }
    }
    return;
}

void Distence(int board[SIZE][SIZE],int n,bool isRed){
    int Dist[SIZE][SIZE];
    int key = isRed ? 1 : -1;
    if(isRed){
        for(int j = 0;j < n;j++){
            
        }
    }
}

bool inBoard(int n,int x,int y){
    return x >= 0 && x < n && y >= 0 && y < n;
}



