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

#include "Func.h"
using namespace std;


int dx[6] = {0,-1,-1,0,1,1}; //方向
int dy[6] = {-1,0,1,1,0,-1};



MCTtree::MCTtree(vector<vector<int>> Board,MCTtree* Parent) : 
board(Board),parent(Parent),Dist(4,vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,-1))){
    value = 0;
    ni = 0;
    ucb = INT_MAX;
}


int PVMWS(vector<vector<int>>& board,vector<vector<vector<int>>>& Dist,bool isRed,int alpha,int beta,int depth){
    int i,value,best;
    //
    if(depth >= MAX_DEPTH){
        return board_evaluate(board,Dist);
    }

    int lim = BRANCH;
    vector<pair<int,int>> moves = generate_move(board,Dist,isRed,lim);
    if(moves.size() == 0){
        return board_evaluate(board,Dist);
    }

    make_move(board,moves[0],isRed);
    best = -PVMWS(board,Dist,isRed,-beta,-alpha,depth + 1);
    undo_move(board,moves[0]);

    alpha = best;

    for(int i = 1;i < moves.size();i++){
        make_move(board,moves[i],isRed);
        value = -PVMWS(board,Dist,isRed,-alpha - MWS,-alpha,depth + 1);
        if(value > best){
            if(value > alpha && value < beta){
                best = -PVMWS(board,Dist,isRed,-beta,-value,depth + 1);
            }
        }
        undo_move(board,moves[i]);
        alpha = max(alpha,best);
        if(best > beta)
            break;
    }
    return best;
}

void make_move(vector<vector<int>>& board,pair<int,int> position,bool isRed){
    board[position.first][position.second] = isRed ? 1 : -1;
    return;
}

void undo_move(vector<vector<int>>& board,pair<int,int> position){
    board[position.first][position.second] = 0;
    return;
}

vector<pair<int,int>> generate_move(const vector<vector<int>>& board,const vector<vector<vector<int>>>& Dist,bool isRed,int lim){   
    priority_queue<pair<int,pair<int,int>>> pq;
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(board[i][j] == 0){
                int value = Dist[0][i][j] + Dist[1][i][j] - Dist[2][i][j] - Dist[3][i][j];
                if(!isRed)
                    value = -value;
                pq.push({value,{i,j}});
            }
        }
    }

    vector<pair<int,int>> avail_position;
    while(!pq.empty() && lim > 0){
        avail_position.push_back(pq.top().second);
        pq.pop();
        lim--;
    }
    return avail_position;
}

void Double_Distance(const vector<vector<int>>& board,vector<vector<int>>& dist,bool isRed,bool isAXIS){
    queue<pair<int,int>> BFS_queue;
    vector<vector<int>> visit(BOARD_SIZE,vector<int>(BOARD_SIZE,0));
    int key = isRed ? 1 : -1;
    if(isRed){
        int start_line = isAXIS ? 0 : BOARD_SIZE - 1;
        for(int i = 0;i < BOARD_SIZE;i++){
            if(board[start_line][i] == key){
                dist[start_line][i] = 0;
                visit[start_line][i] = 2;             
            }
            else if(board[start_line][i] == 0){
                dist[start_line][i] = 1;
                visit[start_line][i] = 2;             
            }
        }
        for(int i = 0;i < BOARD_SIZE;i++){
            vector<pair<int,int>> neighbour = get_neighbour(board,visit,isRed,{start_line,i});
            int len = neighbour.size();
            for(int j = 0;j < len;j++){
                if(visit[neighbour[j].first][neighbour[j].second] == 0){
                    visit[neighbour[j].first][neighbour[j].second]++;
                    dist[neighbour[j].first][neighbour[j].second] = dist[start_line][i] + 1;
                }
                else if(visit[neighbour[j].first][neighbour[j].second] == 1){
                    visit[neighbour[j].first][neighbour[j].second]++;
                    dist[neighbour[j].first][neighbour[j].second] = max(dist[neighbour[j].first][neighbour[j].second],dist[start_line][i] + 1);
                    BFS_queue.push({neighbour[j].first,neighbour[j].second});
                }
            }
        }
    }
    else{    
        int start_col = isAXIS ? 0 : BOARD_SIZE - 1;
        for(int i = 0;i < BOARD_SIZE;i++){
            if(board[i][start_col] == key){
                dist[i][start_col] = 0;
                visit[i][start_col] = 2;             
            }
            else if(board[i][start_col] == 0){
                dist[i][start_col] = 1;
                visit[i][start_col] = 2;             
            }
        }
        for(int i = 0;i < BOARD_SIZE;i++){
            vector<pair<int,int>> neighbour = get_neighbour(board,visit,isRed,{i,start_col});
            int len = neighbour.size();
            for(int j = 0;j < len;j++){
                if(visit[neighbour[j].first][neighbour[j].second] == 0){
                    visit[neighbour[j].first][neighbour[j].second]++;
                    dist[neighbour[j].first][neighbour[j].second] = dist[i][start_col] + 1;
                }
                else if(visit[neighbour[j].first][neighbour[j].second] == 1){
                    visit[neighbour[j].first][neighbour[j].second]++;
                    dist[neighbour[j].first][neighbour[j].second] = max(dist[neighbour[j].first][neighbour[j].second],dist[i][start_col] + 1);
                    BFS_queue.push({neighbour[j].first,neighbour[j].second});
                }
            }
        }
    }

    while(!BFS_queue.empty()){
        auto current_position = BFS_queue.front();
        BFS_queue.pop();
        vector<pair<int,int>> neighbour = get_neighbour(board,visit,isRed,current_position);
        int len = neighbour.size();
        for(int i = 0;i < len;i++){
            if(visit[neighbour[i].first][neighbour[i].second] == 0){
                visit[neighbour[i].first][neighbour[i].second]++;
                dist[neighbour[i].first][neighbour[i].second] = dist[current_position.first][current_position.second] + 1;
            }
            else if(visit[neighbour[i].first][neighbour[i].second] == 1){
                visit[neighbour[i].first][neighbour[i].second]++;
                dist[neighbour[i].first][neighbour[i].second] = max(dist[neighbour[i].first][neighbour[i].second],dist[current_position.first][current_position.second] + 1);
                BFS_queue.push({neighbour[i].first,neighbour[i].second});
            }
        }
    }
    return;
}

vector<pair<int,int>> get_neighbour(const vector<vector<int>>& board,const vector<vector<int>>& visit,bool isRed,pair<int,int> position){
    vector<pair<int,int>> neighbour;
    int key = isRed ? 1 : -1;
    vector<vector<bool>> vis(BOARD_SIZE,vector<bool>(BOARD_SIZE,false));
    queue<pair<int,int>> q;
    q.push(position);
    while(!q.empty()){
        auto current_position = q.front();
        q.pop();
        for(int i = 0;i < 6;i++){
            pair<int,int> temp_position = {current_position.first + dx[i],current_position.second + dy[i]};
            if(!inBoard(temp_position) || visit[temp_position.first][temp_position.second] > 1 || vis[temp_position.first][temp_position.second])
                continue;
            if(board[temp_position.first][temp_position.second] == 0){
                neighbour.push_back(temp_position);
                vis[temp_position.first][temp_position.second] = true;
            }
            else if(board[temp_position.first][temp_position.second] == key){
                q.push(temp_position);
                vis[temp_position.first][temp_position.second] = true;
                neighbour.push_back(temp_position);
            }
        }
    }
    return neighbour;
}

bool inBoard(pair<int,int> position){
    return position.first >= 0 && position.first < BOARD_SIZE && position.second >= 0 && position.second < BOARD_SIZE;
}

void FileSet(vector<vector<int>> &Board){
	FILE* fp = fopen("D:\\data\\vscode\\hex\\board.txt","r");
	for(int i = 0;i < BOARD_SIZE;i++){
		for(int j = 0;j < BOARD_SIZE;j++){
			fscanf(fp,"%d",&Board[i][j]);
		}
	}
	fclose(fp);
    return;
}

int board_evaluate(const vector<vector<int>>& board,vector<vector<vector<int>>>& Dist){
    return 0;
}