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


const int dx[6] = {0,-1,-1,0,1,1}; //方向
const int dy[6] = {-1,0,1,1,0,-1};
const bool Dist_isRed[4] = {true,true,false,false};
const bool Dist_isAXIS[4] = {true,false,true,false};

int RedValue;
enum Player{red,blue};
Player player;
int N;//总共模拟了多少次
map<vector<vector<int>>,MCTtree*> BoardMap; //换位表

MCTtree::MCTtree(vector<vector<int>>& Board,MCTtree* Parent) : 
parent(Parent),board(Board),Dist(4,vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,MAX_DISTANCE))){
    value = 0;
    ni = 0;
    ucb = INT_MAX;
    decision = {-1,-1};
}

MCTtree::MCTtree(MCTtree* Parent) :
parent(Parent),board(Parent->board),Dist(4,vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,MAX_DISTANCE))){
    value = 0;
    ni = 0;
    ucb = INT_MAX;
    decision = {-1,-1};
}

void MCTtree::CalculateUCB() {
	this->ucb = value / ni + C * sqrt(log(N) / ni);
    return;
}

void make_move(vector<vector<int>>& board,pair<int,int> position,Player isRed){
    board[position.first][position.second] = isRed == red ? RedValue : -RedValue;
    return;
}

void undo_move(vector<vector<int>>& board,pair<int,int> position){
    board[position.first][position.second] = 0;
    return;
}

vector<pair<int,int>> generate_move(const MCTtree* t,Player isRed,int lim){   
    priority_queue<pair<int,pair<int,int>>,vector<pair<int,pair<int,int>>>,greater<pair<int,pair<int,int>>>> pq;
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(t->board[i][j] == 0){
                int value = 0;
                value = t->Dist[0][i][j] + t->Dist[1][i][j] - t->Dist[2][i][j] - t->Dist[3][i][j];
                if((player == red && isRed == blue) || (player == blue && isRed == red)){
                    value = -value;
                }
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
    int key = isRed ? RedValue : -RedValue;
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

        // for(int i = 0;i < BOARD_SIZE - 1;i++){
        //     if(dist[start_line][i + 1] == key){
        //         dist[start_line][i] = MAX_DISTANCE;
        //     }
        // }
        // if(dist[start_line][BOARD_SIZE - 2] == key)
        //     dist[start_line][BOARD_SIZE - 1] = MAX_DISTANCE;

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

        // for(int i = 0;i < BOARD_SIZE - 1;i++){
        //     if(dist[i + 1][start_col] == key){
        //         dist[i][start_col] = MAX_DISTANCE;
        //     }
        // }
        // if(dist[BOARD_SIZE - 2][start_col] == key)
        //     dist[BOARD_SIZE - 1][start_col] = MAX_DISTANCE;

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

    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(visit[i][j] != 2){
                dist[i][j] = MAX_DISTANCE;
            }
        }
    }
    return;
}

vector<pair<int,int>> get_neighbour(const vector<vector<int>>& board,const vector<vector<int>>& visit,bool isRed,pair<int,int> position){
    vector<pair<int,int>> neighbour;
    int key = isRed ? RedValue : -RedValue;
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

void FileSet(vector<vector<int>> &Board,int& x,int& y){
	freopen("D:\\data\\vscode\\hex\\board.txt","r",stdin);
	for(int i = 0;i < BOARD_SIZE;i++){
		for(int j = 0;j < BOARD_SIZE;j++){
			cin >> Board[i][j];
		}
	}
    cin >> x >> y;
    return;
}

void judgeRed(const vector<vector<int>>& board,Player& Begin,int x,int y){
    RedValue = board[1][2];
    player = RedValue == 1 ? red : blue;
    Begin = board[x][y] == RedValue ? red : blue;
    return;
}

double board_evaluate(const vector<vector<int>>& board,const vector<vector<vector<int>>>& Dist){
    priority_queue<int> Red,Blue;
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(board[i][j] == 0){
                Red.push(Dist[0][i][j] + Dist[1][i][j]);
                Blue.push(Dist[2][i][j] + Dist[3][i][j]);
            }
        }
    }
    while(!Red.empty() && !Blue.empty()){
        if(Red.top() < Blue.top()){
            return player == red ? 1 : 0;
        }
        else if(Red.top() > Red.top()){
            return player == red ? 0 : 1;
        }
        Red.pop();
        Blue.pop();
    }
    return 0.5;
}

bool judgeWin(const vector<vector<int>>& board,Player isRed){
    queue<pair<int,int>> q;
    vector<vector<bool>> vis(BOARD_SIZE,vector<bool>(BOARD_SIZE,false));
    if(isRed == red){
        for(int i = 0;i < BOARD_SIZE;i++){
            if(board[0][i] == RedValue){
                q.push({0,i});
                vis[0][i] = true;
            }
        }
    }
    else{
        for(int i = 0;i < BOARD_SIZE;i++){
            if(board[i][0] == RedValue){
                q.push({i,0});
                vis[i][0] = true;
            }
        }
    }

    int a = 0;

    while(!q.empty()){
        pair<int,int> pos = q.front();
        q.pop();
        if(pos.first == BOARD_SIZE - 1 && isRed == red)
            return true;
        else if(pos.second == BOARD_SIZE - 1 && isRed == blue)
            return true;
        for(int i = 0;i < 6;i++){
            int tempX = pos.first + dx[i];
            int tempY = pos.second + dy[i];
            if(inBoard({tempX,tempY}) && board[tempX][tempY] == RedValue && !vis[tempX][tempY]){
                q.push({tempX,tempY});
                vis[tempX][tempY] = true;
            }
        }
    }
    return false;
}

MCTtree* Node_Init(MCTtree* parent,pair<int,int> pos,Player isRed){
    make_move(parent->board,pos,isRed);
    auto it = BoardMap.find(parent->board);
    if(it != BoardMap.end()){
        MCTtree* t = new MCTtree(*(it->second));
        t->parent = parent;
        t->decision = pos;
        undo_move(parent->board,pos);
        return t;
    }
    MCTtree* t = new MCTtree(parent);
    for(int i = 0;i < 4;i++){
        Double_Distance(t->board,t->Dist[i],Dist_isRed[i],Dist_isAXIS[i]);
    }
    BoardMap.insert({t->board,t});
    t->decision = pos;
    undo_move(parent->board,pos);
    return t;
}

void backSpread(MCTtree* t,double value){
    while(!t){
        t->value += value;
        t->ni++;
        t = t->parent;
    }
    return;
}

double Expand(MCTtree* t,double alpha,double beta,int depth,Player isRed){
    if(judgeWin(t->board,isRed)){
        double value = (player == isRed) ? 1 : 0;
        backSpread(t,value);
        t->CalculateUCB();
        return t->ucb;
    }

    if(depth == MAX_DEPTH){
        backSpread(t,board_evaluate(t->board,t->Dist));
        t->CalculateUCB();
        return t->ucb;
    }

    Player nextPlay = isRed == red ? blue : red;
    vector<pair<int,int>> avail_pos = generate_move(t,nextPlay,BRANCH);
    int len = avail_pos.size();
    if(isRed == blue){
        for(int i = 0;i < len;i++){
            t->children.push_back(Node_Init(t,avail_pos[i],nextPlay));
            N++;
            alpha = max(alpha,Expand(t->children[i],alpha,beta,depth + 1,nextPlay));
            if(alpha >= beta){
                break;
            }
        }
        t->ni++;
        t->value += alpha;

        return alpha;
    }
    else{
        for(int i = 0;i < len;i++){
            t->children.push_back(Node_Init(t,avail_pos[i],nextPlay));
            beta = min(beta,Expand(t->children[i],alpha,beta,depth + 1,nextPlay));
            if(alpha >= beta){
                break;
            }
        }
        t->ni++;
        t->value += beta;

        return beta;
    }
}

void solve(){
    int x, y, n;
    vector<vector<int>> board(BOARD_SIZE,vector<int>(BOARD_SIZE,0));
    FileSet(board,x,y);

    if(x == -1 && y == -1){
        int new_x = 1;
        int new_y = 2;
        cout << new_x << ' ' << new_y << endl;
        return;
	}

	clock_t start = clock();

    Player Begin;
    judgeRed(board,Begin,x,y);
    MCTtree* root = new MCTtree(board,nullptr);
    for(int i = 0;i < 4;i++){
        Double_Distance(root->board,root->Dist[i],Dist_isRed[i],Dist_isAXIS[i]);
    }
    Expand(root,INT_MIN,INT_MAX,0,Begin);
    pair<int,int> pos;
    double value = 0;
    for(auto i : root->children){
        if(i->ni == 0)
            continue;
        i->CalculateUCB();
        if(i->ucb > value){
            value = i->ucb;
            pos = i->decision;
        }
    }

    clock_t end = clock();
	cout << pos.first << ' ' << pos.second << endl;
    cout << double(end - start) << "ms" << endl;

    return;
}



// int PVMWS(vector<vector<int>>& board,vector<vector<vector<int>>>& Dist,bool isRed,int alpha,int beta,int depth){
//     int i,value,best;
//     //
//     if(depth >= MAX_DEPTH){
//         return board_evaluate(board,Dist);
//     }

//     int lim = BRANCH;
//     vector<pair<int,int>> moves = generate_move(board,Dist,isRed,lim);
//     if(moves.size() == 0){
//         return board_evaluate(board,Dist);
//     }

//     make_move(board,moves[0],isRed);
//     best = -PVMWS(board,Dist,isRed,-beta,-alpha,depth + 1);
//     undo_move(board,moves[0]);

//     alpha = best;

//     for(int i = 1;i < moves.size();i++){
//         make_move(board,moves[i],isRed);
//         value = -PVMWS(board,Dist,isRed,-alpha - MWS,-alpha,depth + 1);
//         if(value > best){
//             if(value > alpha && value < beta){
//                 best = -PVMWS(board,Dist,isRed,-beta,-value,depth + 1);
//             }
//         }
//         undo_move(board,moves[i]);
//         alpha = max(alpha,best);
//         if(best > beta)
//             break;
//     }
//     return best;
// }