#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <cassert>
#include "the3.h"

void printVector(std::vector<int>& array){

    std::cout << "size: " << array.size() << std::endl;
    for (unsigned i = 0; i < array.size(); ++i){
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}




bool TestTask(int test_case)
{
    
    // time variables
    clock_t begin;
    clock_t end;
    double duration;
    int expectedTime = 500;
    int true_answer;
    int solution;
    int n;
    if ((begin = clock ()) == -1) 
        std::cerr << "clock err" << std::endl;

    switch (test_case)
    {
        case 0: {
            /*int adjacencyMatrix[5][5] = {
                {0, 0, 2, 5, 3},
                {0, 0, 0, 2, 3},
                {2, 0, 0, 1, 0},
                {5, 2, 1, 0, 4},
                {3, 3, 0, 4, 0}
            };*/
            int adjacencyMatrix[5][5] = {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            };

            n = 5;
            int** edgeList = (int **)malloc(n * sizeof(int *));
            double* scores = (double *) malloc(sizeof(double) * n);

            for (int i = 0; i < n; i++)
            {
                edgeList[i] = (int *)malloc(n * sizeof(int));
                for (int j = 0; j < n; j++)
                {
                    edgeList[i][j] = adjacencyMatrix[i][j];
                }
                scores[i] = 0;
            }

            true_answer = 1;
            solution = Important (n, edgeList, scores);
            std::cout << "SCORES" <<  std::endl;
            for (int i = 0; i < n; i++)
            {
                std::cout << scores[i] << " ";
            }
                std::cout <<  std::endl;
            std::cout << "SCORES" <<  std::endl;


            break;
            /*END-CASE END-CASE END-CASE*/
        }

        case (1): {
            int adjacencyMatrix[6][6] = {
                {0, 0, 0, 3, 0, 2},
                {0, 0, 3, 0, 2, 0},
                {0, 3, 0, 0, 0, 0},
                {3, 0, 0, 0, 0, 4},
                {0, 2, 0, 0, 0, 0},
                {2, 0, 0, 4, 0, 0}
            };

            n = 6;
            int** edgeList = (int **)malloc(n * sizeof(int *));
            double* scores = (double *) malloc(sizeof(double) * n);

            for (int i = 0; i < n; i++)
            {
                edgeList[i] = (int *)malloc(n * sizeof(int));
                for (int j = 0; j < n; j++)
                {
                    edgeList[i][j] = adjacencyMatrix[i][j];
                }
                scores[i] = 0;
            }

            true_answer = 2;
            solution = Important (n, edgeList, scores);
            std::cout << "SCORES" <<  std::endl;
            for (int i = 0; i < n; i++)
            {
                std::cout << scores[i] << " ";
            }
                std::cout <<  std::endl;
            std::cout << "SCORES" <<  std::endl;
            break;
            /*END-CASE END-CASE END-CASE*/
        }
    }
    
    // generate solution

    if ((end = clock ()) == -1) 
        std::cerr << "clock err" << std::endl;
      
    duration = ((double)end-begin) / CLOCKS_PER_SEC * 1000000;
  
    std::cout << "Elapsed time  " << duration << " vs " << expectedTime << " microsecs" << std::endl;
    
    std::cout << "Solution is: " << solution << ", True answer: " << true_answer;

    return (solution == true_answer);
}





int main()
{
    srandom(time(0));       // no need to initialize second time
    int test = 0;
    while (test++ < 2)
    {
        if (TestTask(test - 1))
            std::cout << "PASSED TASK" << std::endl << std::endl;
        else
            std::cout << "FAILED TASK" << std::endl << std::endl;
    }

  return 0;
}