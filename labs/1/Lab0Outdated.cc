#include <iostream>
#include <stdlib.h>
using namespace std;

int main(){
	int iSize;
	int jSize;
	int a;
	int b;
	int c;
	int t;
	cout << "What do you want i to be? " << endl;
	cin >> iSize;
	cout << "What do you want j to be? " << endl;
	cin >> jSize;
	cout << "What do you want a to be? " << endl;
	cin >> a;
	cout << "What do you want b to be? " << endl;
	cin >> b;
	cout << "What do you want c to be? " << endl;
	cin >> c;
	cout << "What do you want t to be? " << endl;
	cin >> t;
	
	int x[iSize+2][jSize+2];
	int y[iSize][jSize];
	

	for (int i = 0; i < iSize+2; i++){
		for (int j = 0; j < jSize+2; j++){
			x[i][j] = rand() % 2;
		}
	}
	
	for (int i = 1; i < iSize+1; i++){
		for (int j = 1; j < jSize+1; j++){
			y[i-1][j-1] = a * (x[i-1][j-1] + x[i-1][j+1] + x[i+1][j-1] + x[i+1][j+1]) +
						  b * (x[i-1][j+0] + x[i+1][j+0] + x[i+0][j-1] + x[i+0][j+1]) +
						  c * (x[i+0][j+0]);
		
		}
	}
}
