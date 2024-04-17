#include <bits/stdc++.h>

#include "Func.h"
using namespace std;

const int BOARD_SIZE = 11;
const int MAX_DISTANCE = 100;
const int MAX_POTENTIAL = MAX_DISTANCE * MAX_DISTANCE;
const int M = 11;

class Position;
class ConnectedGraph;
class HeuristicTree;

map<vector<vector<int>>,HeuristicTree*> Map;
enum Player{red,blue};
int dx[6];
int dy[6];
Player dist_color[4] = {red,red,blue,blue};
bool dist_if_AXIS[4] = {true,false,true,false};

vector<vector<int>> board(BOARD_SIZE,vector<int>(BOARD_SIZE,0));
Player myColor;
int RedValue;
clock_t start;

class Position{
    public:
        int x;
        int y;
        int value;
        Position(){}
        Position(int X,int Y) : x(X),y(Y),value(0){}
        Position(int X,int Y,int Value) : x(X),y(Y),value(Value){}
        Position(const Position& pos) : x(pos.x),y(pos.y),value(pos.value){}
};

class ConnectedGraph{
    public:
    map<Position,int> Connect_Point;
    map<int,int> BoardMap;
	vector<bool> AXIS;
	vector<bool> Far_AXIS;

	ConnectedGraph(){}
    ConnectedGraph(const ConnectedGraph& C) :
    Connect_Point(C.Connect_Point),BoardMap(C.BoardMap),AXIS(C.AXIS),Far_AXIS(C.Far_AXIS){}
};

class HeuristicTree{
    public:
        vector<vector<int>> board;
        HeuristicTree* parent;
        vector<vector<vector<int>>> Dist;
        vector<vector<int>> MoveBadness;
        vector<HeuristicTree*> children;
        vector<Position> avail_pos;
        Position pos;
        ConnectedGraph Graph[2];
        int min_Potential[2];
        int min_MoveBadness;
        int depth;
        bool expand;

        HeuristicTree() : Dist(4,vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,MAX_DISTANCE))),pos(Position(-1,-1,0)) {
            min_Potential[0] = min_Potential[1] = MAX_POTENTIAL;
            expand = false;
        }
        HeuristicTree(const HeuristicTree& HTree);
};

HeuristicTree::HeuristicTree(const HeuristicTree& HTree) :
board(HTree.board),Dist(HTree.Dist),MoveBadness(HTree.MoveBadness),pos(HTree.pos) {
    min_Potential[0] = HTree.min_Potential[0];
    min_Potential[1] = HTree.min_Potential[1];
    min_MoveBadness = HTree.min_MoveBadness;
    expand = true;
}

bool inBoard(Position pos){
    return pos.x >= 0 && pos.x < BOARD_SIZE && pos.y >= 0 && pos.y < BOARD_SIZE;
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

Player judgeRed(const vector<vector<int>>& board){
    RedValue = board[1][2];
    myColor = RedValue == 1 ? red : blue;
    return myColor == red ? blue : red;
}

void make_move(vector<vector<int>>& board,Position pos,Player color){
    board[pos.x][pos.y] = color == red ? RedValue : -RedValue;
    return;
}

void undo_move(vector<vector<int>>& board,Position pos){
    board[pos.x][pos.y] = 0;
    return;
}

bool cmp(const Position&a,const Position& b){
    return a.value < b.value;
}

bool judgeWin(HeuristicTree* t,Player color){
    int flag = color == red ? 0 : 1;
    int len = t->Graph[flag].AXIS.size();
    for(int i = 0;i < len;i++)
        if(t->Graph[flag].AXIS[i] && t->Graph[flag].Far_AXIS[i])
            return true;
    return false;
}

vector<Position> get_neighbour(const vector<vector<int>>& board,const vector<vector<int>>& dist,const vector<vector<int>>& visit,Player color,Position pos){
    vector<Position> neighbour;
    if(dist[pos.x][pos.y] >= MAX_DISTANCE)
        return neighbour;
    int value = color == red ? RedValue : -RedValue;
    vector<vector<bool>> haveBFS(BOARD_SIZE,vector<bool>(BOARD_SIZE,false));
    queue<Position> q;
    q.push(pos);
    while(!q.empty()){
        auto current_pos = q.front();
        q.pop();
        for(int i = 0;i < 6;i++){
            Position temp_pos(current_pos.x + dx[i],current_pos.y + dy[i]);
            if(!inBoard(temp_pos) || visit[temp_pos.x][temp_pos.y] > 1 || haveBFS[temp_pos.x][temp_pos.y])
                continue;

            if(board[temp_pos.x][temp_pos.y] == value){
                q.push(temp_pos);
                haveBFS[temp_pos.x][temp_pos.y] = true;
                neighbour.push_back(temp_pos);
            }
            else if(board[temp_pos.x][temp_pos.y] == 0){
                neighbour.push_back(temp_pos);
                haveBFS[temp_pos.x][temp_pos.y] = true;
            }
        }
    }
    return neighbour;
}

void Double_Distance(const vector<vector<int>>& board,vector<vector<int>>& dist,Player color,bool isAXIS){
    queue<Position> BFS_queue;
    vector<vector<int>> visit(BOARD_SIZE,vector<int>(BOARD_SIZE,0));
    int value = color == red ? RedValue : -RedValue;
    if(color == red){
        int start_line = isAXIS ? 0 : BOARD_SIZE - 1;
        for(int i = 0;i < BOARD_SIZE;i++){
            visit[start_line][i] = 2; 
            if(board[start_line][i] == value){
                dist[start_line][i] = 0;
                BFS_queue.push(Position(start_line,i));          
            }
            else if(board[start_line][i] == 0){
                dist[start_line][i] = 1;
                BFS_queue.push(Position(start_line,i));            
            }
            else{
                dist[start_line][i] = MAX_DISTANCE;            
            }
        }
    }
    else{    
        int start_col = isAXIS ? 0 : BOARD_SIZE - 1;
        for(int i = 0;i < BOARD_SIZE;i++){
            visit[i][start_col] = 2;
            if(board[i][start_col] == value){
                dist[i][start_col] = 0;
                BFS_queue.push(Position(i,start_col));            
            }
            else if(board[i][start_col] == 0){
                dist[i][start_col] = 1;
                BFS_queue.push(Position(i,start_col));            
            }
            else{
                dist[i][start_col] = MAX_DISTANCE; 
            }
        }
    }
    while(!BFS_queue.empty()){
        auto current_pos = BFS_queue.front();
        BFS_queue.pop();
        vector<Position> neighbour = get_neighbour(board,dist,visit,color,current_pos);
        int len = neighbour.size();
        for(int i = 0;i < len;i++){
            int addDistance = board[neighbour[i].x][neighbour[i].y] == value ? 0 : 1;;
            if(visit[neighbour[i].x][neighbour[i].y] == 0){
                visit[neighbour[i].x][neighbour[i].y]++;
                dist[neighbour[i].x][neighbour[i].y] = dist[neighbour[i].x][neighbour[i].y] + addDistance;
            }
            else if(visit[neighbour[i].x][neighbour[i].y] == 1){
                visit[neighbour[i].x][neighbour[i].y]++;
                dist[neighbour[i].x][neighbour[i].y] = max(dist[neighbour[i].x][neighbour[i].y],dist[current_pos.x][current_pos.y] + addDistance);
                BFS_queue.push(Position(neighbour[i]));
            }
        }
    }
    return;
}

void BFS_Connected(const vector<vector<int>>& board,Player color,vector<vector<bool>> &visit,Position pos,ConnectedGraph& CG){
    int key = color == red ? RedValue : -RedValue;
    int index = CG.AXIS.size();
    queue<Position> q;
    q.push(pos);
    bool AXIS = false,Far_AXIS = false;
    CG.BoardMap.insert({index,index});
    while(!q.empty()){
        Position point = q.front();
        q.pop();
        visit[point.x][point.y] = true;
		CG.Connect_Point.insert({point,index});
        if(color == red){
            if(point.x == 0)
                AXIS = true;
            else if(point.x == BOARD_SIZE - 1)
                Far_AXIS = true;
        }
        else{
            if(point.y == 0)
                AXIS = true;
            else if(point.y == BOARD_SIZE - 1)
                Far_AXIS = true;                
        }
        for(int i = 0;i < 6;i++){
            Position next_point(point.x + dx[i],point.y + dy[i]);
            if(inBoard(next_point) && board[next_point.x][next_point.y] == key && visit[next_point.x][next_point.y] == false){
                q.push(next_point);
                visit[next_point.x][next_point.y] = true;
            }
        }
    }
    CG.AXIS.push_back(AXIS);
    CG.Far_AXIS.push_back(Far_AXIS);
    return;
}

void BFS_Init(const vector<vector<int>>& board,Player color,ConnectedGraph& CG){
    int key = color == red ? RedValue : -RedValue;
    vector<vector<bool>> vis(BOARD_SIZE,vector<bool>(BOARD_SIZE,false));
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(board[i][j] == key && vis[i][j] == false){
                BFS_Connected(board,color,vis,Position(i,j),CG);
            }
        }
    }
    return;
}

void Connected_Unit(HeuristicTree* t,Position pos,Player color){
    bool hasUnit = false;
    int index;
    int flag = color == red ? 0 : 1;
    int value = color == red ? RedValue : -RedValue;
    for(int i = 0;i < 6;i++){
        Position temp_pos(pos.x + dx[i],pos.y + dy[i]);
        if(!inBoard(temp_pos) || t->board[temp_pos.x][temp_pos.y] != value)
            continue;
        if(!hasUnit){
            hasUnit = true;
            auto it = t->Graph[flag].BoardMap.find((t->Graph[flag].Connect_Point.find(temp_pos))->second);
            index = it->second;
            t->Graph[flag].Connect_Point.insert({temp_pos,index});
            if(temp_pos.x == 0)
                t->Graph[flag].AXIS[index] = true;
            else if(temp_pos.x == BOARD_SIZE - 1)
                t->Graph[flag].Far_AXIS[index] = true; 
        }
        else{
            auto it = t->Graph[flag].BoardMap.find((t->Graph[flag].Connect_Point.find(temp_pos))->second);
            int temp = it->second;
            it->second = index;
            t->Graph[flag].AXIS[index] = t->Graph[flag].AXIS[index] || t->Graph[flag].AXIS[temp];
            t->Graph[flag].Far_AXIS[index] = t->Graph[flag].Far_AXIS[index] || t->Graph[flag].Far_AXIS[temp];
        }
        return;
    }
    if(!hasUnit){
        index = t->Graph[flag].AXIS.size();
        t->Graph[flag].Connect_Point.insert({pos,index});
        t->Graph[flag].BoardMap.insert({index,index});
        if(color == red){
            t->Graph[flag].AXIS.push_back(pos.x == 0 ? true : false);
            t->Graph[flag].Far_AXIS.push_back(pos.x == BOARD_SIZE - 1 ? true : false);
        }
        else{
            t->Graph[flag].AXIS.push_back(pos.y == 0 ? true : false);
            t->Graph[flag].Far_AXIS.push_back(pos.y == BOARD_SIZE - 1 ? true : false);
        }
    }
    return;
}


int evaluate(HeuristicTree* t,Player color){
    int p[2] = {INT_MAX,INT_MAX};
    int a[2] = {0,0};
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(t->board[i][j] == 0){
                int temp[2] = {t->Dist[0][i][j] + t->Dist[1][i][j],t->Dist[2][i][j] + t->Dist[3][i][j]};
                for(int k = 0;k < 2;k++){
                    if(p[k] > temp[k]){
                        p[k] = temp[k];
                        a[k] = 0;
                    }
                    else if(p[k] == temp[k])
                        a[k]++;
                }
            }

        }
    }
    int value;
    if(color == red)
        value = M * (p[0] - p[1]) - (a[0] - a[1]);
    else
        value = M * (p[1] - p[0]) - (a[1] - a[0]);
    return value;
}

HeuristicTree* CreateNode(HeuristicTree* Parent,Position pos,Player color){
    if(Parent != nullptr){
        make_move(Parent->board,pos,color);
        auto Map_it = Map.find(Parent->board);
        if(Map_it != Map.end()){
            HeuristicTree* t = new HeuristicTree(*Map_it->second);
            return t;
        }
    }
    HeuristicTree* t;
    if(Parent != nullptr)
        t = new HeuristicTree(*Parent);
    else{
        t = new HeuristicTree;
        t->board = board;
    }
    for(int i = 0;i < 4;i++)
        Double_Distance(t->board,t->Dist[i],dist_color[i],dist_if_AXIS[i]);
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            int p[2] = {t->Dist[0][i][j] + t->Dist[1][i][j],t->Dist[2][i][j] + t->Dist[3][i][j]};
            t->min_Potential[0] = min(t->min_Potential[0],p[0]); 
            t->min_Potential[1] = min(t->min_Potential[1],p[1]);
            t->MoveBadness[i][j] = p[0] + p[1];
            t->min_MoveBadness = min(t->min_MoveBadness,t->MoveBadness[i][j]);
            t->avail_pos.push_back(Position(i,j,t->MoveBadness[i][j]));
        }
    }
    sort(t->avail_pos.begin(),t->avail_pos.end(),cmp);
    Connected_Unit(t,pos,color);
    if(Parent != nullptr)
        undo_move(Parent->board,pos);
    return t;
}


int DFS(HeuristicTree*t,Player color,int alpha,int beta,int TreeDepth,int& DFS_Depth){
    if((double)(clock() - start) / CLOCKS_PER_SEC > 0.97)
        return 0;
    if(judgeWin(t,color)){
        return evaluate(t,color);
    }
    int best_value = INT_MIN;
    Player next_color = color == red ? blue : red;
    int len = t->avail_pos.size();
    for(int i = 0;i < len && t->MoveBadness[t->avail_pos[i].x][t->avail_pos[i].y] - t->min_MoveBadness < 200;i++){
        t->children.push_back(CreateNode(t,t->avail_pos[i],next_color));
        if(next_color == myColor){
            alpha = max(alpha,DFS(t->children[i],next_color,-beta,-alpha,TreeDepth + 1,++DFS_Depth));
        }
        else{
            beta = min(beta,-DFS(t,next_color,-beta,-alpha,TreeDepth + 1,++DFS_Depth));
        }
        if(alpha > beta)
            break;
    }
    return best_value;
}

Position SearchInit(Position pos,Player color){
    HeuristicTree* root = CreateNode(nullptr,pos,color);
    while((double)(clock() - start) / CLOCKS_PER_SEC < 0.9){

    }
}

void solve(){
    start = clock();
    int x,y,n;
    BotSet(board,n,x,y);
    if(x == -1 && y == -1){
        cout << 1 << ' ' << 2 << endl;
        return;
    }
    Position pre_pos(x,y);
    Player pcolor = judgeRed(board);
    Position best_pos = SearchInit(pre_pos,pcolor);
    cout << best_pos.x << ' ' << best_pos.y << endl;
    return;
}

int main(){
    solve();
    return 0;
}
