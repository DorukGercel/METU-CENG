#include "the3.h"

// You  can define extra functions here
using namespace std;

/*
void printVector(std::vector<int> array){

    std::cout << "size: " << array.size() << std::endl;
    for (unsigned i = 0; i < array.size(); ++i){
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

void printList(int** array, int n){
    for (unsigned i = 0; i < n; ++i){
       for(unsigned j = 0; j < n; ++j){
        std::cout << array[i][j] << " ";
       }
       cout << endl;
    }
    std::cout << std::endl;
}
*/

bool isInVector(int value, vector<int> v){
   int size = v.size();
   for(int i = 0; i < size; i++){
      if(v[i] == value){
         return true;
      }
   }
   return false;
}

void placeNeighbours(int value, int size, vector<int>& traversed, vector<int>& not_traversed, int**& edgeList){
   for(int i = 0; i < size; i++){
      int edge = edgeList[value][i];
      if(edge > 0 && !isInVector(i, traversed)){
         not_traversed.push_back(i);
      }
   }
}

vector<int>* componentMaker(int size, int**& edgeList, vector<int>& traversed, vector<int>& not_traversed){
   vector<int>* component = new vector<int>;
   while(!not_traversed.empty()){
      int value = not_traversed.back();
      not_traversed.pop_back();
      if(!isInVector(value, traversed)){
         traversed.push_back(value);
         placeNeighbours(value, size, traversed, not_traversed, edgeList);
         component->push_back(value);
      }
   }
   return component;
}

int** modifiedFloydWharshall(int size, int**&edgeList){
   int distance = 0;
   int** newList = new int*[size];

   for(int i = 0; i < size; i++){
      newList[i] = new int[size];
      for(int j = 0; j < size; j++){
         if((distance = edgeList[i][j]) == 0 && i != j){
            distance = __INT_MAX__;
         }
         newList[i][j] = distance;
      }
   }
   
   for(int k = 0; k < size; k++){
      for(int i = 0; i < size; i++){
         for(int j = 0; j < size; j++){
            if(newList[i][k] != __INT_MAX__ && newList[k][j] != __INT_MAX__){
               if(newList[i][j] > newList[i][k] + newList[k][j]){
                  newList[i][j] = newList[i][k] + newList[k][j];
               }
            }
         }
      }
   }
   return newList;
}

double* scoresCalculator(int** newList, int size){
   double* score = new double[size];
   double point;
   for(int v = 0; v < size; v++){
      point = 0;
      for(int i = 0; i < size; i++){
         for(int j = 0; j < size; j++){
            int d_src_v = newList[i][v], d_v_dst = newList[v][j], d_src_dst = newList[i][j];
            if(d_src_v == __INT_MAX__ || d_v_dst == __INT_MAX__ || d_src_dst == __INT_MAX__){
               continue;
            }
            else{
               if(i == j){
                  d_src_dst = 1;
               }
               if(v == i || v == j){
                  d_src_v = 0;
                  d_v_dst = 0;
               }
               point += (double)(d_src_v + d_v_dst)/(double)d_src_dst; 
            }
         }
      }
      score[v] = point;
   }
   return score;
}

// INPUT :
//            n         : number of nodes in the graph
//            edgeList  : edges in the graph
//            scores    : importance scores
// return value : 
//                 number of disconnected components
int Important (int n, int**& edgeList, double*& scores)
{ 
   vector<int> traversed, not_traversed;
   vector<vector<int>*> components;
   int** fw_list;
   //vector<int**> components_edge_list;

   for(int i = 0; i < n; i++){
      if(!isInVector(i, traversed)){
         not_traversed.push_back(i);
         vector<int>* tmp = componentMaker(n, edgeList, traversed, not_traversed);
         components.push_back(tmp);
      }
   }

   fw_list = modifiedFloydWharshall(n, edgeList);
   
   /*
   printList(fw_list, n);
   cout << endl;
   */
   scores = scoresCalculator(fw_list, n);
   /*
   for(int i = 0; i < n; i++){
      cout << scores[i] << endl;
   }
   cout << components.size() << endl;
   */
   return components.size(); 
}