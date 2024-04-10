#include <bits/stdc++.h>

using namespace std;

const int BOARD_SIZE = 11;
const int MAX_DISTANCE = 200; //最大距离
const int MWS = 1; //窗口区间
const int BRANCH = 8; //分支系数
const int MAX_DEPTH = 5; //最大搜索深度
const int C = 2;
enum Player{red,blue};
const int dx[6] = {0,-1,-1,0,1,1}; //方向
const int dy[6] = {-1,0,1,1,0,-1};
const bool Dist_isRed[4] = {true,true,false,false};
const bool Dist_isAXIS[4] = {true,false,true,false};
int RedValue;
Player player;
int N = 0;//总共模拟了多少次

class MCTtree;
class ConnectedGraph;
map<vector<vector<int>>,MCTtree*> BoardMap; //换位表
clock_t time_start,time_end;

class MCTtree{
public:
	MCTtree* parent;
	vector<MCTtree*> children;
	double value;
	int ni;//每个结点模拟了多少次
	double ucb;
    pair<int,int> decision;
	vector<vector<int>> board;

    vector<pair<int,int>> avail_pos;
    int cnt;
	bool hasExpand;
	ConnectedGraph* RedGraph;
	ConnectedGraph* BlueGraph;

	MCTtree(MCTtree* Parent);
	MCTtree(vector<vector<int>>& board);
	MCTtree(const MCTtree& tree,MCTtree* Parent);

	MCTtree* SelectMaxUCBNode();

	void CalculateUCB();
};

//连通分量
class ConnectedGraph{
    public:
    vector<vector<int>> Map;
	vector<bool> AXIS;
	vector<bool> Far_AXIS;

	ConnectedGraph();
    ConnectedGraph(const ConnectedGraph* C); //拷贝构造
};

ConnectedGraph::ConnectedGraph() : Map(vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,-1))){}

ConnectedGraph::ConnectedGraph(const ConnectedGraph* C) :
Map(C->Map),AXIS(C->AXIS),Far_AXIS(C->Far_AXIS){}

MCTtree::MCTtree(vector<vector<int>>& Board) : 
parent(nullptr),board(Board){
    value = 0;
    ni = 0;
    ucb = INT_MAX;
    decision = {-1,-1};
    cnt = 0;
    hasExpand = false;
    RedGraph = new ConnectedGraph();
    BlueGraph = new ConnectedGraph();
}

MCTtree::MCTtree(MCTtree* Parent) : 
parent(Parent),board(Parent->board),avail_pos(Parent->avail_pos){
    value = 0;
    ni = 0;
    ucb = INT_MAX;
    decision = {-1,-1};
    cnt = Parent->cnt - 1;
    hasExpand = false;
    RedGraph = new ConnectedGraph(Parent->RedGraph);
    BlueGraph = new ConnectedGraph(Parent->BlueGraph);
}

MCTtree::MCTtree(const MCTtree& tree,MCTtree* Parent) :
parent(Parent),board(tree.board),avail_pos(tree.avail_pos){
    value = tree.value;
    ni = tree.ni;
    ucb = tree.ucb;
    decision = tree.decision;
    cnt = tree.cnt;
    hasExpand = true;
    RedGraph = new ConnectedGraph(tree.RedGraph);
    BlueGraph = new ConnectedGraph(tree.BlueGraph);
}



void MCTtree::CalculateUCB() {
	this->ucb = value / ni;
    return;
}



bool inBoard(pair<int,int> position){
    return position.first >= 0 && position.first < BOARD_SIZE && position.second >= 0 && position.second < BOARD_SIZE;
}

void BotSet(vector<vector<int>> &board,int& n,int& x,int& y){
	cin >> n;
	for (int i = 0; i < n - 1; i++) {
		cin >> x >> y; if (x != -1) board[x][y] = -1;	//对方
		cin >> x >> y; if (x != -1) board[x][y] = 1;	//我方
	}
	cin >> x >> y;
	if (x != -1) board[x][y] = -1;	//对方
    return;
}

void FileSet(vector<vector<int>> &board,int& x,int& y){
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++)
            cin >> board[i][j];
    }
    cin >> x >> y;
    return;
}

void judgeRed(const vector<vector<int>>& board,Player& Begin,int x,int y){
    RedValue = board[1][2];
    player = RedValue == 1 ? red : blue;
    Begin = board[x][y] == RedValue ? blue : red;
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

void backSpread(MCTtree* t,double value){
    N++;
    while(t){
        t->value += value;
        t->ni++;
        t = t->parent;
    }
    return;
}

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

vector<pair<int,int>> generate_move(const vector<vector<int>>& board,const vector<vector<vector<int>>>& Dist,Player isRed,int lim){   
    vector<pair<int,pair<int,int>>> pq;
    bool minus = isRed == red ? false : true;
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(board[i][j] == 0){
                int value = 0;

                // if(Dist[0][i][j] >= MAX_DISTANCE)
                //     break;
                // if(Dist[1][i][j] >= MAX_DISTANCE)
                //     break;
                // if(Dist[2][i][j] >= MAX_DISTANCE)
                //     break;
                // if(Dist[3][i][j] >= MAX_DISTANCE)
                //     break;

            
                int Rvalue = pow(Dist[0][i][j],2) + pow(Dist[1][i][j],2);
                int Bvalue = pow(Dist[2][i][j],2) + pow(Dist[3][i][j],2);
                // int Rvalue = sqrt(Dist[0][i][j] * Dist[1][i][j]);
                // int Bvalue = sqrt(Dist[2][i][j] * Dist[3][i][j]);

                value =  Rvalue + Bvalue;
                // value = min(Rvalue,Bvalue);

                // if(minus){
                //     value = -value;
                // }

                // if(i == 10){
                //     int k = 0;
                // }

                // if(minus){
                //     value = pow(t->Dist[2][i][j],2) + pow(t->Dist[3][i][j],2);
                // }
                // else{
                //     value = pow(t->Dist[0][i][j],2) + pow(t->Dist[1][i][j],2);
                // }
                pq.push_back({value,{i,j}});
            }
        }
    }
    sort(pq.begin(),pq.end());
    vector<pair<int,int>> avail_position;
    int len = pq.size();
    int i = 0;
    while(i < len && lim > 0){
        avail_position.push_back(pq[i].second);
        i++;
        lim--;
    }
    return avail_position;
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
            else{
                dist[start_line][i] = MAX_DISTANCE;
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
            else{
                dist[i][start_col] = MAX_DISTANCE;
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
            if(visit[i][j] == 0){
                dist[i][j] = MAX_DISTANCE;
            }
        }
    }
    return;
}




// vector<pair<int,int>> get_neighbour(const vector<vector<int>>& board,const vector<vector<int>>& dist,const vector<vector<int>>& visit,bool isRed,pair<int,int> position){
//     vector<pair<int,int>> neighbour;
//     if(dist[position.first][position.second] >= MAX_DISTANCE)
//         return neighbour;
//     int key = isRed ? RedValue : -RedValue;
//     vector<vector<bool>> vis(BOARD_SIZE,vector<bool>(BOARD_SIZE,false));
//     queue<pair<int,int>> q;
//     q.push(position);
//     while(!q.empty()){
//         auto current_position = q.front();
//         q.pop();
//         for(int i = 0;i < 6;i++){
//             pair<int,int> temp_position = {current_position.first + dx[i],current_position.second + dy[i]};
//             if(!inBoard(temp_position) || visit[temp_position.first][temp_position.second] > 0 || vis[temp_position.first][temp_position.second])
//                 continue;
//             if(board[temp_position.first][temp_position.second] == 0 && dist[temp_position.first][temp_position.second] < MAX_DISTANCE){
//                 neighbour.push_back(temp_position);
//                 vis[temp_position.first][temp_position.second] = true;
//             }
//             else if(board[temp_position.first][temp_position.second] == key){
//                 q.push(temp_position);
//                 vis[temp_position.first][temp_position.second] = true;
//                 neighbour.push_back(temp_position);
//             }
//         }
//     }
//     return neighbour;
// }

// void Double_Distance(const vector<vector<int>>& board,vector<vector<int>>& dist,bool isRed,bool isAXIS){
//     queue<pair<int,int>> BFS_queue;
//     vector<vector<int>> visit(BOARD_SIZE,vector<int>(BOARD_SIZE,0));
//     int key = isRed ? RedValue : -RedValue;
//     if(isRed){
//         int start_line = isAXIS ? 0 : BOARD_SIZE - 1;
//         for(int i = 0;i < BOARD_SIZE;i++){
//             if(board[start_line][i] == key){
//                 dist[start_line][i] = 0;
//                 visit[start_line][i] = 1;            
//             }
//             else if(board[start_line][i] == 0){
//                 dist[start_line][i] = 1;
//                 visit[start_line][i] = 1;             
//             }
//             else{
//                 dist[start_line][i] = MAX_DISTANCE;
//                 visit[start_line][i] = 1;             
//             }
//         }

//         for(int i = 0;i < BOARD_SIZE;i++){
//             if(board[start_line][i] == key){
//                 if(inBoard({start_line,i - 1}))
//                     dist[start_line][i - 1] = MAX_DISTANCE;
//                 if(inBoard({start_line,i + 1}))
//                     dist[start_line][i + 1] = MAX_DISTANCE;
//             }
//         }

//         for(int i = 0;i < BOARD_SIZE;i++){
//             vector<pair<int,int>> neighbour = get_neighbour(board,dist,visit,isRed,{start_line,i});
//             int len = neighbour.size();
//             for(int j = 0;j < len;j++){
//                 if(visit[neighbour[j].first][neighbour[j].second] == 0){
//                     visit[neighbour[j].first][neighbour[j].second]++;
//                     dist[neighbour[j].first][neighbour[j].second] = dist[start_line][i] + 1;
//                     BFS_queue.push({neighbour[j].first,neighbour[j].second});
//                 }
//             }
//         }
//     }
//     else{    
//         int start_col = isAXIS ? 0 : BOARD_SIZE - 1;
//         for(int i = 0;i < BOARD_SIZE;i++){
//             if(board[i][start_col] == key){
//                 dist[i][start_col] = 0;
//                 visit[i][start_col] = 1;             
//             }
//             else if(board[i][start_col] == 0){
//                 dist[i][start_col] = 1;
//                 visit[i][start_col] = 1;             
//             }
//             else{
//                 dist[i][start_col] = MAX_DISTANCE;
//                 visit[i][start_col] = 1;  
//             }
//         }

//         for(int i = 0;i < BOARD_SIZE;i++){
//             if(board[i][start_col] == key){
//                 if(inBoard({i - 1,start_col}))
//                     dist[i - 1][start_col] = MAX_DISTANCE;
//                 if(inBoard({i + 1,start_col}))
//                     dist[i + 1][start_col] = MAX_DISTANCE;
//             }
//         }

//         for(int i = 0;i < BOARD_SIZE;i++){
//             vector<pair<int,int>> neighbour = get_neighbour(board,dist,visit,isRed,{i,start_col});
//             int len = neighbour.size();
//             for(int j = 0;j < len;j++){
//                 if(visit[neighbour[j].first][neighbour[j].second] == 0){
//                     visit[neighbour[j].first][neighbour[j].second]++;
//                     dist[neighbour[j].first][neighbour[j].second] = dist[i][start_col] + 1;
//                     BFS_queue.push({neighbour[j].first,neighbour[j].second});
//                 }
//             }
//         }
//     }

//     while(!BFS_queue.empty()){
//         auto current_position = BFS_queue.front();
//         BFS_queue.pop();
//         vector<pair<int,int>> neighbour = get_neighbour(board,dist,visit,isRed,current_position);
//         int len = neighbour.size();
//         for(int i = 0;i < len;i++){
//             if(visit[neighbour[i].first][neighbour[i].second] == 0){
//                 visit[neighbour[i].first][neighbour[i].second]++;
//                 dist[neighbour[i].first][neighbour[i].second] = dist[current_position.first][current_position.second] + 1;
//                 BFS_queue.push({neighbour[i].first,neighbour[i].second});
//             }
//         }
//     }

//     for(int i = 0;i < BOARD_SIZE;i++){
//         for(int j = 0;j < BOARD_SIZE;j++){
//             if(visit[i][j] == 0){
//                 dist[i][j] = MAX_DISTANCE;
//             }
//         }
//     }
//     return;
// }


double board_evaluate(MCTtree* t){
    vector<vector<vector<int>>> Dist(4,vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,-1)));
    for(int i = 0;i < 4;i++){
        Double_Distance(t->board,Dist[i],Dist_isRed[i],Dist_isAXIS[i]);
    }
    priority_queue<int,vector<int>,greater<int>> Red,Blue;
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(t->board[i][j] == 0){
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

    // srand(time(0));
    // int value = rand() % 2;
    // return (double)value;

    // return 0.5;
}

void BFS_Connected(const vector<vector<int>>& board,Player isRed,vector<vector<bool>> &visit,int x,int y,ConnectedGraph& CG){
    int key = isRed == red ? RedValue : -RedValue;
    
    int NUM = CG.AXIS.size();
    queue<pair<int,int>> q;
    q.push(make_pair(x,y));
    bool AXIS = false,Far_AXIS = false;

    while(!q.empty()){
        pair<int,int> point = q.front();
        q.pop();
        visit[point.first][point.second] = true;
		CG.Map[point.first][point.second] = NUM;

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
                q.push({tempX,tempY});
				visit[tempX][tempY] = true;
                CG.Map[tempX][tempY] = NUM;
            }
        }
    }

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
    bool hasUnit = false;
    int num;
    for(int i = 0;i < 6;i++){
        int tempX = pos.first + dx[i];
        int tempY = pos.second + dy[i];

        if(!inBoard({tempX,tempY}))
            continue;

        if(isRed == red && t->board[tempX][tempY] == RedValue){
            if(!hasUnit){
                hasUnit = true;
                num = t->RedGraph->Map[tempX][tempY];
                t->RedGraph->Map[pos.first][pos.second] = num;
                if(pos.first == 0)
                    t->RedGraph->AXIS[num] = true;
                else if(pos.second == BOARD_SIZE - 1)
                    t->RedGraph->Far_AXIS[num] = true; 
            }
            else{
                int temp = t->RedGraph->Map[tempX][tempY];
                for(int n = 0;n < BOARD_SIZE;n++){
                    for(int m = 0;m < BOARD_SIZE;m++){
                        if(t->RedGraph->Map[n][m] == temp)
                            t->RedGraph->Map[n][m] = num;
                    }
                }
                if(t->RedGraph->AXIS[temp]){
                    t->RedGraph->AXIS[num] = true;
                }
                if(t->RedGraph->Far_AXIS[temp]){
                    t->RedGraph->Far_AXIS[num] = true;
                }
            }
        }
        else if(isRed == blue && t->board[tempX][tempY] == -RedValue){
            if(!hasUnit){
                hasUnit = true;
                num = t->BlueGraph->Map[tempX][tempY];
                t->BlueGraph->Map[pos.first][pos.second] = num;
                if(pos.second == 0)
                    t->BlueGraph->AXIS[num] = true;
                else if(pos.second == BOARD_SIZE - 1)
                    t->BlueGraph->Far_AXIS[num] = true; 
            }
            else{
                int temp = t->BlueGraph->Map[tempX][tempY];
                for(int n = 0;n < BOARD_SIZE;n++){
                    for(int m = 0;m < BOARD_SIZE;m++){
                        if(t->BlueGraph->Map[n][m] == temp)
                            t->BlueGraph->Map[n][m] = num;
                    }
                }
                if(t->BlueGraph->AXIS[temp]){
                    t->BlueGraph->AXIS[num] = true;
                }
                if(t->BlueGraph->Far_AXIS[temp]){
                    t->BlueGraph->Far_AXIS[num] = true;
                }
            }
        }
    }

    if(!hasUnit){
        if(isRed == red){
            num = t->RedGraph->AXIS.size();
            t->RedGraph->Map[pos.first][pos.second] = num;
            if(pos.first == 0)
                t->RedGraph->AXIS.push_back(true);
            else
                t->RedGraph->AXIS.push_back(false);
            if(pos.first == BOARD_SIZE - 1)
                t->RedGraph->Far_AXIS.push_back(true);
            else
                t->RedGraph->Far_AXIS.push_back(false);
        }
        else{
            num = t->BlueGraph->AXIS.size();
            t->BlueGraph->Map[pos.first][pos.second] = num;
            if(pos.second == 0)
                t->BlueGraph->AXIS.push_back(true);
            else
                t->BlueGraph->AXIS.push_back(false);
            if(pos.second == BOARD_SIZE - 1)
                t->BlueGraph->Far_AXIS.push_back(true);
            else
                t->BlueGraph->Far_AXIS.push_back(false);           
        }
    }

    return;
}

void get_avail_pos(MCTtree* root){
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(root->board[i][j] == 0){
                root->cnt++;
                root->avail_pos.push_back({i,j});
            }
        }
    }
    return;
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
    Connected_Unit(t,pos,isRed);
    BoardMap.insert({t->board,t});
    t->decision = pos;

    vector<pair<int,int>>::iterator pos_begin = t->avail_pos.begin();
    auto pos_end = t->avail_pos.end();
    for( ;pos_begin != pos_end;pos_begin++){
        if(*pos_begin == pos)
            break;
    }
    swap(*pos_begin,t->avail_pos[t->cnt]);

    undo_move(parent->board,pos);
    return t;
}

void boardPrint(const vector<vector<int>>& board){
    for(int i = 0; i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            cout << board[i][j] << '\t';
        }
        cout << endl;
    }
    cout << endl;
    return;
}

void Expand(MCTtree* t,Player isRed,int depth){
    if(judgeWin(t,isRed)){
        double value = (player == isRed) ? 1 : 0;
        backSpread(t,value);
        // t->CalculateUCB();
        return;
    }

    if(t->hasExpand){
        double value = t->value / t->ni;
        backSpread(t,value);
        return;
    }

    if(depth == MAX_DEPTH){
        double value = board_evaluate(t);
        backSpread(t,value);
        return;
    }

    int branch = t->cnt / 4;
    Player nextPlay = isRed == red ? blue : red;
    for(int i = 0;i < branch;i++){
        srand(time(0));
        int rand_pos = rand() % t->cnt;
        t->children.push_back(Node_Init(t,t->avail_pos[rand_pos],nextPlay));

        Expand(t->children[i],nextPlay,depth + 1);
        t->hasExpand = true;
    }
    return;
}

void solve(){
    // freopen("D:\\data\\vscode\\hex\\board.txt","r",stdin);
    int x, y, n;
    vector<vector<int>> board(BOARD_SIZE,vector<int>(BOARD_SIZE,0));
    BotSet(board,n,x,y);
    //FileSet(board,x,y);

    if(x == -1 && y == -1){
        int new_x = 1;
        int new_y = 2;
        cout << new_x << ' ' << new_y << endl;
        return;
	}

    Player Begin;
    judgeRed(board,Begin,x,y);
    time_start = clock();
    
    MCTtree* root = new MCTtree(board);

    vector<vector<vector<int>>> Dist(4,vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,-1)));
    for(int i = 0;i < 4;i++){
        Double_Distance(board,Dist[i],Dist_isRed[i],Dist_isAXIS[i]);
    }
    BFS_Init(root->board,red,*(root->RedGraph));
    BFS_Init(root->board,blue,*(root->BlueGraph)); 
    get_avail_pos(root);

    vector<pair<int,int>> avail_pos = generate_move(board,Dist,Begin,BRANCH);

    if(n < 5){
        cout << avail_pos[0].first << ' ' << avail_pos[0].second << endl;
        return;
    }

    for(int i = 0;i < avail_pos.size();i++){
        root->children.push_back(Node_Init(root,avail_pos[i],Begin));
        Expand(root->children[i],Begin,1);
    }

    time_end = clock();
    // cout << (double)time_end - time_start << endl; 


    pair<int,int> pos = root->children[0]->decision;
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
    cout << pos.first << ' ' << pos.second << endl;
    return;
}

int main(){
    solve();
	return 0;
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