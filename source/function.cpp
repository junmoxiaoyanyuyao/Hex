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

Player player;
int N;//总共模拟了多少次
map<vector<vector<int>>,MCTtree*> BoardMap; //换位表

MCTtree::MCTtree(vector<vector<int>>& Board,MCTtree* Parent) : 
parent(Parent),board(Board),Dist(4,vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,-1))){
    value = 0;
    ni = 0;
    ucb = INT_MAX;
    decision = {-1,-1};
    hasExpand = false;
    RedGraph = new ConnectedGraph();
    BlueGraph = new ConnectedGraph();
}

MCTtree::MCTtree(MCTtree* Parent) :
parent(Parent),board(Parent->board),Dist(4,vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,-1))),
RedGraph(Parent->RedGraph),BlueGraph(Parent->BlueGraph){
    value = 0;
    ni = 0;
    ucb = INT_MAX;
    decision = {-1,-1};
    hasExpand = false;
}

MCTtree::MCTtree(const MCTtree& tree,MCTtree* Parent) :
parent(Parent),board(tree.board),Dist(4,vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,-1))),
RedGraph(tree.RedGraph),BlueGraph(tree.BlueGraph) {
    value = tree.value;
    ni = tree.ni;
    ucb = tree.ucb;
    decision = tree.decision;
    hasExpand = true;
}

ConnectedGraph::ConnectedGraph(){}

ConnectedGraph::ConnectedGraph(const ConnectedGraph* C) :
ConnectedPoint(C->ConnectedPoint),GraphPosition(C->GraphPosition),AXIS(C->AXIS),Far_AXIS(C->Far_AXIS){}

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
    bool minus = isRed == red ? false : true;
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(t->board[i][j] == 0){
                int value = 0;
                int Rvalue = t->Dist[0][i][j] + t->Dist[1][i][j];
                int Bvalue = t->Dist[2][i][j] + t->Dist[3][i][j];
                value =  Rvalue - Bvalue;
                if(minus){
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

        for(int i = 0;i < BOARD_SIZE;i++){
            if(board[start_line][i] == key){
                if(inBoard({start_line,i - 1}))
                    dist[start_line][i - 1] = MAX_DISTANCE;
                if(inBoard({start_line,i + 1}))
                    dist[start_line][i + 1] = MAX_DISTANCE;
            }
        }

        for(int i = 0;i < BOARD_SIZE;i++){
            vector<pair<int,int>> neighbour = get_neighbour(board,dist,visit,isRed,{start_line,i});
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
            if(board[i][start_col] == key){
                if(inBoard({i - 1,start_col}))
                    dist[i - 1][start_col] = MAX_DISTANCE;
                if(inBoard({i + 1,start_col}))
                    dist[i + 1][start_col] = MAX_DISTANCE;
            }
        }

        for(int i = 0;i < BOARD_SIZE;i++){
            vector<pair<int,int>> neighbour = get_neighbour(board,dist,visit,isRed,{i,start_col});
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
        vector<pair<int,int>> neighbour = get_neighbour(board,dist,visit,isRed,current_position);
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

vector<pair<int,int>> get_neighbour(const vector<vector<int>>& board,const vector<vector<int>>& dist,const vector<vector<int>>& visit,bool isRed,pair<int,int> position){
    vector<pair<int,int>> neighbour;
    if(dist[position.first][position.second] >= MAX_DISTANCE)
        return neighbour;
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
            if(board[temp_position.first][temp_position.second] == 0 && dist[temp_position.first][temp_position.second] < MAX_DISTANCE){
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

void BotSet(vector<vector<int>> &board,int& n,int& x,int& y){
	cin >> n;
	for (int i = 0; i < n - 1; i++) {
		cin >> x >> y; if (x != -1) board[x][y] = -1;	//对方
		cin >> x >> y; if (x != -1) board[x][y] = 1;	//我方
	}
	cin >> x >> y;
	if (x != -1) board[x][y] = -1;	//对方
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

// bool judgeWin(const vector<vector<int>>& board,Player isRed){
//     queue<pair<int,int>> q;
//     vector<vector<bool>> vis(BOARD_SIZE,vector<bool>(BOARD_SIZE,false));
//     if(isRed == red){
//         for(int i = 0;i < BOARD_SIZE;i++){
//             if(board[0][i] == RedValue){
//                 q.push({0,i});
//                 vis[0][i] = true;
//             }
//         }
//     }
//     else{
//         for(int i = 0;i < BOARD_SIZE;i++){
//             if(board[i][0] == RedValue){
//                 q.push({i,0});
//                 vis[i][0] = true;
//             }
//         }
//     }

//     int a = 0;

//     while(!q.empty()){
//         pair<int,int> pos = q.front();
//         q.pop();
//         if(pos.first == BOARD_SIZE - 1 && isRed == red)
//             return true;
//         else if(pos.second == BOARD_SIZE - 1 && isRed == blue)
//             return true;
//         for(int i = 0;i < 6;i++){
//             int tempX = pos.first + dx[i];
//             int tempY = pos.second + dy[i];
//             if(inBoard({tempX,tempY}) && board[tempX][tempY] == RedValue && !vis[tempX][tempY]){
//                 q.push({tempX,tempY});
//                 vis[tempX][tempY] = true;
//             }
//         }
//     }
//     return false;
// }

bool judgeWin(MCTtree* t,Player isRed){
    if(isRed == red){
        int len = t->RedGraph->AXIS.size();
        for(int i = 0;i < len;i++){
            if(t->RedGraph->AXIS[i] && t->RedGraph->Far_AXIS[i])
                return true;
        }
        return false;
    }
    int len = t->BlueGraph->AXIS.size();
    for(int i = 0;i < len;i++){
        if(t->BlueGraph->AXIS[i] && t->BlueGraph->Far_AXIS[i])
            return true;
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
    Connected_Unit(t,pos,isRed);
    BoardMap.insert({t->board,t});
    t->hasExpand = false;
    t->decision = pos;
    undo_move(parent->board,pos);
    return t;
}

void backSpread(MCTtree* t,double value){
    while(t){
        t->value += value;
        t->ni++;
        t = t->parent;
    }
    return;
}

// double Expand(MCTtree* t,double alpha,double beta,int depth,Player isRed){
//     if(judgeWin(t,isRed)){
//         double value = (player == isRed) ? 1 : 0;
//         backSpread(t,value);
//         t->CalculateUCB();
//         return t->ucb;
//     }

//     if(depth == MAX_DEPTH){
//         backSpread(t,board_evaluate(t->board,t->Dist));
//         t->CalculateUCB();
//         return t->ucb;
//     }

//     Player nextPlay = isRed == red ? blue : red;
//     vector<pair<int,int>> avail_pos = generate_move(t,nextPlay,BRANCH);
//     int len = avail_pos.size();
//     if(isRed == blue){
//         for(int i = 0;i < len;i++){
//             t->children.push_back(Node_Init(t,avail_pos[i],nextPlay));
//             N++;
//             if(t->children[i]->hasExpand){
//                 alpha = max(alpha,t->children[i]->ucb);
//             }
//             else
//                 alpha = max(alpha,Expand(t->children[i],alpha,beta,depth + 1,nextPlay));
//             if(alpha >= beta){
//                 break;
//             }
//         }
//         t->ni++;
//         t->CalculateUCB();
//         alpha = max(alpha,t->ucb);
//         t->value += alpha;

//         return alpha;
//     }
//     else{
//         for(int i = 0;i < len;i++){
//             t->children.push_back(Node_Init(t,avail_pos[i],nextPlay));
//             beta = min(beta,Expand(t->children[i],alpha,beta,depth + 1,nextPlay));
//             if(alpha >= beta){
//                 break;
//             }
//         }
//         t->ni++;
//         t->value += beta;

//         return beta;
//     }
// }

void Expand(MCTtree* t,int depth,Player isRed){
    if(judgeWin(t,isRed)){
        double value = (player == isRed) ? 1 : 0;
        backSpread(t,value);
        t->CalculateUCB();
        return;
    }

    if(depth == MAX_DEPTH){
        backSpread(t,board_evaluate(t->board,t->Dist));
        t->CalculateUCB();
        return;
    }    

    Player nextPlay = isRed == red ? blue : red;
    vector<pair<int,int>> avail_pos = generate_move(t,nextPlay,BRANCH);
    int len = avail_pos.size();

    for(int i = 0;i < len;i++){
        t->children.push_back(Node_Init(t,avail_pos[i],nextPlay));
        N++;
        if(t->children[i]->hasExpand){
            continue;
        }
        Expand(t->children[i],depth + 1,nextPlay);      
        t->children[i]->hasExpand = true;      
    }

    return;

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
    BFS_Init(root->board,red,*(root->RedGraph));
    BFS_Init(root->board,blue,*(root->BlueGraph));    
    Expand(root,0,Begin);
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

void BFS_Connected(const vector<vector<int>>& board,Player isRed,vector<vector<bool>> &visit,int x,int y,ConnectedGraph& CG){
    int key = isRed == red ? RedValue : -RedValue;
    
    int NUM = CG.ConnectedPoint.size();
    vector<pair<int,int>> tempV;
    visit[x][y] = true;
    queue<pair<int,int>> q;
    q.push(make_pair(x,y));
    bool AXIS = false,Far_AXIS = false;

    while(!q.empty()){
        pair<int,int> point = q.front();
        q.pop();
		tempV.push_back(point);
        if(isRed == red){
            if(point.first == 0)
                AXIS = true;
            else if(point.first == BOARD_SIZE - 1)
                Far_AXIS = true;
        }
        else{
            if(point.second == 0)
                AXIS = true;
            else if(point.second == BOARD_SIZE - 1)
                Far_AXIS = true;                
        }
        for(int i = 0;i < 6;i++){
            int tempX = point.first + dx[i];
            int tempY = point.second + dy[i];
            if(inBoard({tempX,tempY}) && board[tempX][tempY] == key && visit[tempX][tempY] == false){
                pair<int,int> tempPoint = make_pair(tempX,tempY);
                q.push(tempPoint);
				visit[tempX][tempY] = true;
            }
        }
    }
	CG.ConnectedPoint.push_back(tempV);
    CG.AXIS.push_back(AXIS);
    CG.Far_AXIS.push_back(Far_AXIS);
    return;
}

void BFS_Init(const vector<vector<int>>& board,Player isRed,ConnectedGraph& CG){
    int key = isRed == red ? RedValue : -RedValue;
    vector<vector<bool>> vis(BOARD_SIZE,vector<bool>(BOARD_SIZE,false));
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(board[i][j] == key && vis[i][j] == false){
                BFS_Connected(board,isRed,vis,i,j,CG);
            }
        }
    }
    return;
}

void Connected_Unit(MCTtree* t,pair<int,int> pos,Player isRed){
    int key = isRed == red ? RedValue : -RedValue;
    bool hasUnit = false;
    int num;
    for(int i = 0;i < 6;i++){
        int tempX = pos.first + dx[i];
        int tempY = pos.second + dy[i];
        if(isRed == red){
            if(inBoard({tempX,tempY}) && t->board[tempX][tempY] == RedValue){
                auto it = t->RedGraph->GraphPosition.find({tempX,tempY});
                if(it != t->RedGraph->GraphPosition.end()){
                    if(!hasUnit){
                        t->RedGraph->GraphPosition.insert({{tempX,tempY},it->second});
                        t->RedGraph->ConnectedPoint[it->second].push_back({tempX,tempY});
                        hasUnit = true;
                        num = it->second;
                        if(tempX == 0)
                            t->RedGraph->AXIS[num] = true;
                        else if(tempX == BOARD_SIZE - 1)
                            t->RedGraph->Far_AXIS[num] = true;                        
                    }
                    else{
                        int len = t->RedGraph->ConnectedPoint[it->second].size();
                        map<pair<int,int>,int>::iterator iit;
                        for(int i = 0;i < len;i++){
                            pair<int,int> tempPos = t->RedGraph->ConnectedPoint[it->second][i];
                            iit = t->RedGraph->GraphPosition.find(tempPos);
                            iit->second = num;
                            t->RedGraph->ConnectedPoint[num].push_back(tempPos);
                        }
                        if(t->RedGraph->AXIS[iit->second]){
                            t->RedGraph->AXIS[num] = true;
                        }
                        if(t->RedGraph->Far_AXIS[iit->second]){
                            t->RedGraph->Far_AXIS[num] = true;
                        }                        
                    }
                }
            }
        }
        else{
            if(inBoard({tempX,tempY}) && t->board[tempX][tempY] == -RedValue){
                auto it = t->BlueGraph->GraphPosition.find({tempX,tempY});
                if(it != t->BlueGraph->GraphPosition.end()){
                    if(!hasUnit){
                        t->BlueGraph->GraphPosition.insert({{tempX,tempY},it->second});
                        t->BlueGraph->ConnectedPoint[it->second].push_back({tempX,tempY});
                        hasUnit = true;
                        num = it->second;
                        if(tempY == 0)
                            t->BlueGraph->AXIS[num] = true;
                        else if(tempY == BOARD_SIZE - 1)
                            t->BlueGraph->Far_AXIS[num] = true;
                    }
                    else{
                        int len = t->BlueGraph->ConnectedPoint[it->second].size();
                        map<pair<int,int>,int>::iterator iit;
                        for(int i = 0;i < len;i++){
                            pair<int,int> tempPos = t->BlueGraph->ConnectedPoint[it->second][i];
                            iit = t->BlueGraph->GraphPosition.find(tempPos);
                            iit->second = num;
                            t->BlueGraph->ConnectedPoint[num].push_back(tempPos);
                        }
                        if(t->BlueGraph->AXIS[iit->second]){
                            t->BlueGraph->AXIS[num] = true;
                        }
                        if(t->BlueGraph->Far_AXIS[iit->second]){
                            t->BlueGraph->Far_AXIS[num] = true;
                        }                                
                    }
                }
            }
        }
    }


    if(!hasUnit){
        if(isRed == red){
            vector<pair<int,int>> temp;
            temp.push_back(pos);
            int num = t->RedGraph->ConnectedPoint.size();
            t->RedGraph->ConnectedPoint.push_back(temp);
            t->RedGraph->GraphPosition.insert({pos,num});
        }
        else{
            vector<pair<int,int>> temp;
            temp.push_back(pos);
            int num = t->BlueGraph->ConnectedPoint.size();
            t->BlueGraph->ConnectedPoint.push_back(temp);
            t->BlueGraph->GraphPosition.insert({pos,num});            
        }
    }

    return;
}

