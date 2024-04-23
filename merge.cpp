#include <bits/stdc++.h>

using namespace std;

#define Ladder_Err INT_MAX
const int BOARD_SIZE = 11;
const int MAX_DISTANCE = 100;
const int MAX_DEPTH = 2;
const int MAX_POTENTIAL = MAX_DISTANCE * MAX_DISTANCE;
const int M = 2;
const int WINDOW = 1;
int DFS_Depth = 1;
class Position;
class ConnectedGraph;
class HeuristicTree;

map<vector<vector<int>>,HeuristicTree*> Map;
enum Player{red,blue};
int dx[6] = {0,-1,-1,0,1,1};
int dy[6] = {-1,0,1,1,0,-1};
Player dist_color[4] = {red,red,blue,blue};
bool dist_if_AXIS[4] = {true,false,true,false};

vector<vector<int>> board(BOARD_SIZE,vector<int>(BOARD_SIZE,0));
Player myColor;
int RedValue;
clock_t start;
bool runTimeOut = false;

class Position{
    public:
        int x;
        int y;
        int value;
        Position(){}
        Position(int X,int Y) : x(X),y(Y),value(0){}
        Position(int X,int Y,int Value) : x(X),y(Y),value(Value){}
        Position(const Position& pos) : x(pos.x),y(pos.y),value(pos.value){}
        bool operator<(const Position& a) const{
            return this->value < a.value;
        }
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

        HeuristicTree() :
        Dist(4,vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,MAX_DISTANCE))),
        MoveBadness(vector<vector<int>>(BOARD_SIZE,vector<int>(BOARD_SIZE,0))),
        pos(Position(-1,-1,0)){
            parent = nullptr;
            min_Potential[0] = min_Potential[1] = MAX_POTENTIAL;
            min_MoveBadness = 2 * MAX_POTENTIAL;
            expand = false;
        }
        HeuristicTree(const HeuristicTree& HTree);
};

HeuristicTree::HeuristicTree(const HeuristicTree& HTree) :
board(HTree.board),Dist(HTree.Dist),MoveBadness(HTree.MoveBadness),pos(HTree.pos),Graph{HTree.Graph[0],HTree.Graph[1]}{
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

bool cmp(const Position& a,const Position& b){
    return a.value < b.value;
    // if(a.value < b.value)
    //     return true;
    // else if(a.value > b.value)
    //     return false;
    // double d1 = (pow(a.x - 5,2) + pow(a.y - 5,2));
    // double d2 = (pow(b.x - 5,2) + pow(b.y - 5,2));
    // return d1 < d2;
}

bool judgeWin(HeuristicTree* t,Player color){
    int flag = color == red ? 0 : 1;
    int len = t->Graph[flag].AXIS.size();
    for(int i = 0;i < len;i++)
        if(t->Graph[flag].AXIS[i] && t->Graph[flag].Far_AXIS[i])
            return true;
    return false;
}

Position judgeEdge(int x,int y){
    Position pos = Position(-1,-1);
    int k[2][2];
    bool flag;
    bool match = false;
    if(myColor == red){
        k[0][0] = 4,k[0][1] = 5,k[1][0] = 1,k[1][1] = 2;
        if(x == 0)
            flag = false,match = true;
        else if(x == BOARD_SIZE -1)
            flag = true,match = true;
    }
    else{
        k[0][0] = 2,k[0][1] = 3,k[1][0] = 0,k[1][1] = 5;
        if(y == 0)
            flag = false,match = true;
        else if(y == BOARD_SIZE -1)
            flag = true,match = true;
    }
    if(match){
        for(int i = 0;i < 2;i++){
            Position temp = Position(x + dx[k[(int)(flag)][i]],y + dy[k[(int)(flag)][i]]);
            if(inBoard(Position(temp)) && board[temp.x][temp.y] == ((myColor == red) ? RedValue : -RedValue)){
                Position p = Position(temp.x + dx[k[(int)(!flag)][i]],temp.y + dy[k[(int)(!flag)][i]]);
                if(inBoard(p) && board[p.x][p.y] == 0)
                    return p;
            }
        }
    }

    // match = false;
    // if(myColor == red){
    //     if(y == 2){
            
    //     }
    // }

END:
    return pos;
}

int judgeLadder(const vector<vector<int>>& board,Position pos,Player color){

    vector<vector<vector<int>>> dir{
        {{1,2},{5,4}},
        {{0,5},{2,3}}
    }; //color AXIS en
    int nei[2][2] = {{0,3},{1,4}};
    int nnei[2][2][2] = {
        {{2,4},{1,5}},
        {{5,3},{0,2}}
    };
    int value = color == red ? RedValue : -RedValue;
    for(int i = 0;i < 2;i++){
        Position neighbour = Position(pos.x + dx[nei[color][i]],pos.y + dy[nei[color][i]]);
        if(inBoard(neighbour) && board[neighbour.x][neighbour.y] == value){
            for(int j = 0;j < 2;j++){
                if(inBoard(Position(neighbour.x + dx[dir[color][j][0]],neighbour.y + dy[dir[color][j][0]]))
                && board[neighbour.x + dx[dir[color][j][0]]][neighbour.y + dy[dir[color][j][0]]] == -value
                && inBoard(Position(neighbour.x + dx[dir[color][j][1]],neighbour.y + dy[dir[color][j][1]]))
                && board[neighbour.x + dx[dir[color][j][1]]][neighbour.y + dy[dir[color][j][1]]] == -value){
                    Position temp1 = Position(pos.x + dx[nnei[color][i][j]],pos.y + dy[nnei[color][i][j]]);
                    Position temp2 = Position(pos.x + dx[nei[color][i ^ 1]],pos.y + dy[nei[color][i ^ 1]]);
                    while(inBoard(temp1) && inBoard(temp2)){
                        if(board[temp1.x][temp1.y] == -value || board[temp2.x][temp2.y] == -value)
                            return Ladder_Err;
                        if(board[temp1.x][temp1.y] == value || board[temp2.x][temp2.y] == value)
                            return true;
                        temp1 = Position(temp1.x + dx[nei[color][i ^ 1]],temp1.y + dy[nei[color][i ^ 1]]);
                        temp2 = Position(temp2.x + dx[nei[color][i ^ 1]],temp2.y + dy[nei[color][i ^ 1]]);
                    }
                    return Ladder_Err;
                }
            }
        }
    }


    return true;
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
                dist[neighbour[i].x][neighbour[i].y] = dist[current_pos.x][current_pos.y] + addDistance;
            }
            else if(visit[neighbour[i].x][neighbour[i].y] == 1){
                visit[neighbour[i].x][neighbour[i].y]++;
                dist[neighbour[i].x][neighbour[i].y] = max(dist[neighbour[i].x][neighbour[i].y],dist[current_pos.x][current_pos.y] + addDistance);
                BFS_queue.push(Position(neighbour[i]));
            }
        }
    }
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(visit[i][j] < 2)
                dist[i][j] = MAX_DISTANCE;
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
    int a[2] = {0,0};
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            if(t->board[i][j] == 0){
                int temp[2] = {t->Dist[0][i][j] + t->Dist[1][i][j],t->Dist[2][i][j] + t->Dist[3][i][j]};
                for(int k = 0;k < 2;k++){
                    if(t->min_Potential[k] == temp[k])
                        a[k]++;
                }
            }
        }
    }
    int value;
    if(color == red)
        value = M * (t->min_Potential[0] - t->min_Potential[1]) - (a[0] - a[1]);
    else
        value = M * (t->min_Potential[1] - t->min_Potential[0]) - (a[1] - a[0]);
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
    if(Parent != nullptr){
        t = new HeuristicTree(*Parent);
        t->parent = Parent;
        Connected_Unit(t,pos,color);
    }
    else{
        t = new HeuristicTree;
        t->board = board;
        BFS_Init(t->board,red,t->Graph[0]);
        BFS_Init(t->board,blue,t->Graph[1]);
    }
    for(int i = 0;i < 4;i++)
        Double_Distance(t->board,t->Dist[i],dist_color[i],dist_if_AXIS[i]);
    // t->pos.value = evaluate(t,color);
    for(int i = 0;i < BOARD_SIZE;i++){
        for(int j = 0;j < BOARD_SIZE;j++){
            int p[2] = {t->Dist[0][i][j] + t->Dist[1][i][j],t->Dist[2][i][j] + t->Dist[3][i][j]};
            t->min_Potential[0] = min(t->min_Potential[0],p[0]); 
            t->min_Potential[1] = min(t->min_Potential[1],p[1]);
            t->MoveBadness[i][j] = p[0] + p[1];
            t->min_MoveBadness = min(t->min_MoveBadness,t->MoveBadness[i][j]);
            if(t->board[i][j] == 0)
                t->avail_pos.push_back(Position(i,j,p[0] + p[1]));
        }
    }
    sort(t->avail_pos.begin(),t->avail_pos.end(),cmp);
    
    if(Parent != nullptr)
        undo_move(Parent->board,pos);
    return t;
}

int DFS(HeuristicTree*t,Player color,int TreeDepth,int& DFS_Depth){
    // if(runTimeOut || (double)(clock() - start) / CLOCKS_PER_SEC > 0.95){
    //     runTimeOut = true;
    //     return 0;
    // }
    if(judgeWin(t,color)){
        return INT_MIN;
    }
    if(TreeDepth == MAX_DEPTH || t->avail_pos.size() == 0){
        return evaluate(t,color);
    }
    Player next_color = color == red ? blue : red;
    t->children.push_back(CreateNode(t,t->avail_pos[0],next_color));
    int best_value = DFS(t->children[0],next_color,TreeDepth + 1,++DFS_Depth);
    int best_i = 0;
    int value;
    int len = min((int)t->avail_pos.size(),8);
    for(int i = 1;i < len;i++){
        t->children.push_back(CreateNode(t,t->avail_pos[i],next_color));
        value = DFS(t->children[i],next_color,TreeDepth + 1,++DFS_Depth);
        if(color == myColor){
            if(value > best_value){
                best_value = value;
                best_i = i;
            }
        }
        else{
            if(value < best_value){
                best_value = value;
                best_i = i;
            }
        }
    }
    // make_move(t->board,t->avail_pos[best_i],next_color);
    // int v = evaluate(t,color);
    // undo_move(t->board,t->avail_pos[best_i]);
    return best_value;
}


Position SearchInit(Position pos,Player color){ 
    HeuristicTree* root = CreateNode(nullptr,pos,color);
    Player next_color = color == red ? blue : red;
    Position best_pos = root->avail_pos[0];
    int value,best_value = INT_MAX;
    // cout << (double)(clock() - start) / CLOCKS_PER_SEC << endl;
    #ifdef _BOTZONE_ONLINE
    while((double)(clock() - start) / CLOCKS_PER_SEC < 0.95){
    #endif
        int len = min((int)root->avail_pos.size(),8);
        int index = -1;
        for(int i = 0;i < len;i++){
            int v = judgeLadder(root->board,root->avail_pos[i],next_color);
            if(v == Ladder_Err)
                continue;
            index++;
            root->children.push_back(CreateNode(root,root->avail_pos[i],next_color));
            // value = DFS(root->children[index],next_color,1,DFS_Depth);
            if(judgeWin(root->children[index],next_color)){
                best_pos = root->avail_pos[i];
                return best_pos;
            }
            value = evaluate(root->children[index],next_color) + v;
            if(value < best_value){
                best_value = value;
                best_pos = root->avail_pos[i];
            }
        }
    #ifdef _BOTZONE_ONLINE
    }
    #endif
    // cout << (double)(clock() - start) / CLOCKS_PER_SEC << endl;
    return best_pos;
}

void solve(){
    #ifndef _BOTZONE_ONLINE
    freopen("D:\\data\\vscode\\hex\\board.txt","r",stdin);
    #endif
    start = clock();
    int x,y,n;
    BotSet(board,n,x,y);
    if(x == -1 && y == -1){
        cout << 1 << ' ' << 2 << endl;
        return;
    }
    if(x == 1 && y == 2){
        cout << 7 << ' ' << 3 << endl;
        return;        
    }
    if(n == 2 && myColor == red){
        // if(x == 7 && y == 3){
        //     cout << 6 << ' ' << 5 << endl;
        //     return;
        // }
        if(x > 5 && y < 5 && !(x == 8 && y == 1)){
            cout << x - 1 << ' ' << y + 2 << endl;
            return;
        }
        else if(y >= 5 && board[7][3] == 0){
            cout << 7 << ' ' << 3 << endl;
            return;
        }
    }
    Position pre_pos(x,y);
    Player pcolor = judgeRed(board);
    Position best_pos = judgeEdge(x,y);
    if(best_pos.x != -1){
        cout << best_pos.x << ' ' << best_pos.y << endl;
        return;
    }
    best_pos = SearchInit(pre_pos,pcolor);
    cout << best_pos.x << ' ' << best_pos.y << endl;
    return;
} 

int main(){
    solve();
    return 0;
}
