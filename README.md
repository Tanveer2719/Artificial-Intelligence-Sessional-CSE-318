# Artificial-Intelligence-Sessional-CSE-318
The offlines of the AI sessional are uploaded here. 

## Offline-3


# A Report On “Solving the Max-cut problem by GRASP”

Prepared by: Tanveer Rahman (1905025)


## Description

In our given offline We have tried to solve the Max-cut problem using GRASP
algorithm, Which is an iteration based algorithm. The Max-cut problem is a “APX hard” problem i.e. there is no ‘polynomial-time approximation scheme’ for this problem.

## The Implementation of the Algorithm


```bash
  void graspAlgorithm(ProblemInstance &problemInstance, double alpha, vector<int>&ret){
        
        int vertices = problemInstance.getVertices();
        vector<int>solution(vertices, -1);      // solution[i] =-1, 0, 1 -> the vertex in V, T or S respectively
        vector<int>bestSoln(vertices, -1);
        int bestVal = INT_MIN;                  // best val
        int sameCount = 0;
        double avgLocalSearchItr = 0,avgLocalSearchVal = 0;


        for(int i = 1 ; i<= MAXITR ; i++){
            findInitialSolution(problemInstance, solution, alpha);
            pair<int, int> searchVal = localSearch(problemInstance, solution);
            
            avgLocalSearchItr = (avgLocalSearchItr * (i-1) + searchVal.first) / i;
            avgLocalSearchVal = (avgLocalSearchVal * (i-1) + searchVal.second) / i;
            
            if(searchVal.second > bestVal ){
                bestVal = searchVal.second;
                bestSoln = vector<int>(solution);
                sameCount = 0;
            }

            solution = vector<int>(vertices, -1);
            sameCount ++; 

            cout<<i<< " : val = "<<bestVal<<endl;
        }
    }
```

Here for each iteration of the outer 'for' loop, I have created a new semi-greedy solution from the given graph and then, tried to improve the solution value using the localSearch(), and updated the 'bestVal' variable for the better solution.



## The Implementation of the localSearch
```bash
pair<int, int> localSearch(ProblemInstance &problemInstance, vector<int>&solution){
        int count = 0;
        bool change = true;
        int index = 1;          // this is used for a little optimization, inside local search
                                // the for() loop will start from index and it will be updated
                                // inside the for loop
        while(change){
            count ++;
            change = false;
            for(int i = index; i<=problemInstance.getVertices(); i++){
                
                if(solution[i-1] == 1){     //v belongs to S
                    int del = problemInstance.calculateSigma_S(i, solution) - problemInstance.calculateSigma_T(i, solution);
                    if(del > 0){
                        solution[i-1] = 1 - solution[i-1];
                        change = true;
                        index = i; 
                        break;
                    }
                }
                if(solution[i-1] == 0){
                    int del = problemInstance.calculateSigma_T(i, solution) - problemInstance.calculateSigma_S(i, solution);
                    if(del > 0){
                        solution[i-1] = 1 - solution[i-1];
                        change = true;
                        index = i;
                        break; 
                    }
                }                
            }
        }

        return make_pair(count, problemInstance.greedyValue(solution));
    }
```
We returned the no of iterations needed by the search to move to the local maximum and the greedyValue of the partition obtained from the localsearch 

## The initial Solution Formulation
```bash
// semigreedy method
    void findInitialSolution(ProblemInstance &problemInstance, vector<int>&solution, double alpha){
        int vertices = problemInstance.getVertices();
        
        // randomly some of the nodes and put them in S or T
        for(int i = 0; i<(vertices >> 3); i++){
            int indx = random_number(1, vertices);    // randomly select a vertex
            solution[indx-1] = random_number(0,1);   // put the randomly selected vertex in set S 
        }

        if((vertices >> 3 ) == 0 ){
            int indx = random_number(1, vertices);
            solution[indx-1] = random_number(0,1);
        }
       
        // while there is a unassigned vertex available iterate the loop
        while(count(solution.begin(), solution.end(), -1) > 0){
            priority_queue<pair<int,int>>RCL = calculateRCL(problemInstance, solution, alpha);
            if(RCL.empty()){
                break;
            }
            int batchSize = random_number(1, RCL.size());
            while(batchSize){
                pair<int,int>top = RCL.top();
                RCL.pop();
                int selectedVertex = top.second;
                int sigma_S = problemInstance.calculateSigma_S(top.second, solution);
                int sigma_T = problemInstance.calculateSigma_T(top.second, solution);
                solution[selectedVertex-1] =sigma_T > sigma_S ? 1 : 0;
                batchSize-- ;
            } 
        }
    }
    
```
I have used the Semigreedy method for finding the initial solution. A good point to look the loop
```bash
for(int i = 0; i<(vertices >> 3); i++){
            int indx = random_number(1, vertices);    // randomly select a vertex
            solution[indx-1] = random_number(0,1);   // put the randomly selected vertex in set S 
        }

```
Here I have randomly assigned some of the vertices to one of the two partitions to make the solution finding a little easier. 


## Conclusion
From the provided '.html' file we can see the comparisn of our implementation. We can easily see that the result of the grasp has improved all the randomized, greedy
and semi-greedy solutions. In fact, in most of the cases the GRASP resulted about
90% of the benchmark solution.

Since, GRASP is a randomized algorithm, for another run of the program, the values
may vary and they can be better or worse for some graphs. Again, if the number of
iterations of the grasp is increased, then there is a chance of further improvement of
the result, because then we could reach the global maxima, by overcoming the local
maxima during the local search and get a better result.

So we can say that our experiment with the grasp algorithm to find the max- cut of a
graph is a success.




