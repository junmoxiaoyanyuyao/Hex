#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>

#include "Func.h"
using namespace std;

const int SIZE = 15;
int board[SIZE][SIZE];//����1���Է�-1���հ�0

void FileSet(int &n,int &x,int &y);

int main()
{
	int x, y, n;
	//�ָ�Ŀǰ��������Ϣ

	//cin >> n;
	// for (int i = 0; i < n - 1; i++) {
	// 	cin >> x >> y; if (x != -1) board[x][y] = -1;	//�Է�
	// 	cin >> x >> y; if (x != -1) board[x][y] = 1;	//�ҷ�
	// }
	// cin >> x >> y;
	// if (x != -1) board[x][y] = -1;	//�Է�

	FileSet(n,x,y);
	
	//��ʱboard[][]��洢�ľ��ǵ�ǰ���̵�����������Ϣ,x��y����ǶԷ����һ���µ���

	/************************************************************************************/
	/***********�����������Ĵ��룬���߽�������������ӵ�λ�ã�����new_x��new_y��****************/
	//�����Ϊ������Ե�ʾ�����룬��ɾ��
	int avail_x[SIZE*SIZE], avail_y[SIZE*SIZE]; //�Ϸ�λ�ñ�
	int cnt = 0;
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			if (board[i][j] == 0) {
				avail_x[cnt] = i;
				avail_y[cnt] = j;
				cnt++;
			}
	srand(time(0));
	int rand_pos = rand() % cnt;
	int new_x = avail_x[rand_pos];
	int new_y = avail_y[rand_pos];
	/***********���Ϸ������Ĵ��룬���߽�������������ӵ�λ�ã�����new_x��new_y��****************/
	/************************************************************************************/

	// ��ƽ̨������߽��
	cout << new_x << ' ' << new_y << endl;
	return 0;
}

void FileSet(int &n,int &x,int &y){
    //Ĭ��n = 11
	FILE* fp = fopen("./board.txt","r");
	fscanf(fp,"%d",&n);
	for(int i = 0;i < n;i++){
		for(int j = 0;j < n;j++){
			fscanf(fp,"%d",board[i][j]);
		}
		int temp;
		fscanf(fp,"%d",&temp);
	}
	fscanf(fp,"%d",&x);
	fscanf(fp,"%d",&y);
	fclose(fp);
    return;
}