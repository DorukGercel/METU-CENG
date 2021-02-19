#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <cassert>
#include "the3.h"

using namespace std;

int n = 5;
double* scores = 0;

int edge[6][6] = {{0, 0, 0, 3, 0, 2},
                  {0, 0, 3, 0, 2, 0},
                  {0, 3, 0, 0, 0, 0},
                  {3, 0, 0, 0, 0, 4},
                  {0, 2, 0, 0, 0, 0},
                  {2, 0, 0, 4, 0, 0}};

int edge1[5][5] = {{0, 0, 2, 5, 3},
                  {0, 0, 0, 2, 3},
                  {2, 0, 0, 1, 0},
                  {5, 2, 1, 0, 4},
                  {3, 3, 0, 4, 0},
                  };

int edge2[5][5] = {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            };

int ** edgeList; 

void initializer(){
    edgeList = new int*[n];
    for(int i = 0; i < n; i++){
        edgeList[i] = edge2[i];
    }
}

void printEdgeList(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << *(*(edgeList + i)+j) << " ";
        }
        cout << endl;
    }
}

int main()
{
  initializer();
  Important (n, edgeList, scores);
  return 0;
}
