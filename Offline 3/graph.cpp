#include<bits/stdc++.h>
#define INF 1000000
#define MAXITR 500

/*
The idea got from the article: 
    https://mpc.zib.de/archive/2016/3/Santis2016_Article_ANonmonotoneGRASP.pdf
*/
using namespace std;

int random_number(int s, int e){
    // Create a random number engine and seed it with the current time
    random_device rd;
    mt19937 gen(rd());

    // Create a uniform distribution for random integers
    uniform_int_distribution<> dis(s, e);
    return dis(gen);
}


class Graph{

public:
    int nodes, edges;       // numbers of nodes and edges
    vector<vector<int>>adjMat;   //adjMat[u] = { pair<v, w> , pair<x, w> }

    int maxU, maxV, maxW = INT_MIN; // for the maximum edge

    Graph(){}
    Graph(int n, int m){
        nodes = n;
        edges = m;
        adjMat.resize(nodes,vector<int>(nodes, INF ));
    }
    void addEdge(int u, int v, int w){
        // adjMat[u-1].push_back(make_pair(v-1,w));
        // adjMat[v-1].push_back(make_pair(u-1,w));   // since an undirected graph
        adjMat[u-1][v-1] = w;
        adjMat[v-1][u-1] = w;
    }
    void printGraph(){
        cout<<"Nodes: "<<nodes << " Edges: "<<edges<<endl<<"\n";
        for(int i = 0; i<adjMat.size(); i++){
            for(int j = 0; j<adjMat.size(); j++){
                cout<<adjMat[i][j]<<"\t";
            }
            cout<<endl;
        }
    }
    
    void readFromFile(string name){
        ifstream readFile(name);

        string line;
        getline(readFile, line);    // read the first line

        string word;
        string words[3];
        int i = 0;
        stringstream ss(line);
        while(ss>>word){
            words[i++] = word;
        }

        // set nodes, edges and the adjMat size
        nodes = stoi(words[0]);
        edges = stoi(words[1]);
        adjMat.resize(nodes,vector<int>(nodes, INF ));

        for(int j = 0; j<edges; j++){
            i = 0;
            getline(readFile, line);
            stringstream ss(line);
            while(ss>>word){
                words[i++] = word;
            }
            addEdge(stoi(words[0]), stoi(words[1]), stoi(words[2]));
            
            if(stoi(words[2]) > maxW){
                maxU = stoi(words[0]);
                maxV = stoi(words[1]);
                maxW = stoi(words[2]);
            }
        }

        readFile.close();

    }

};

class ProblemInstance{   

public:
    Graph graph;
    
    ProblemInstance(Graph &graph){
        this->graph = graph;
    }
    
    int greedyFunction(int v, vector<int>&solution){

        return max(calculateSigma_S(v, solution), calculateSigma_T(v, solution));
    }

    int greedyValue(vector<int>&solution){
        int s_S = 0;
        for(int i = 1; i<=graph.nodes; i++){
            for(int j = i+1; j<= graph.nodes;j++){
                if((solution[i-1] != solution[j-1]) && (graph.adjMat[i-1][j-1] != INF)){ // they vertices are in the different cut
                    s_S += graph.adjMat[i-1][j-1];
                }
            }
        }
        return s_S;
        
    }

    pair<int, int> calculateWminMax(vector<int>&solution){
        int minVal = INT_MAX;
        int maxVal = INT_MIN;

        for(int i = 1; i<= graph.nodes; i++){
            if(solution[i-1] == -1){
                int sigma_S = calculateSigma_S(i, solution);
                int sigma_T = calculateSigma_T(i, solution);
                int min_sigma = min(sigma_S, sigma_T);
                int max_sigma = max(sigma_S, sigma_T);
                minVal = minVal > min_sigma ? min_sigma : minVal;
                maxVal = maxVal < max_sigma ? max_sigma : maxVal;
            } 
        } 
        return make_pair(maxVal, minVal);
    }

    int calculateSigma_S(int v, vector<int>&solution){
        int sigma_S = 0;
        for(int i = 1; i<= graph.nodes; i++){
            if(solution[i-1] == 1 && graph.adjMat[i-1][v-1] != INF)
                sigma_S += graph.adjMat[i-1][v-1];
        }
        return sigma_S;
    }
    
    int calculateSigma_T(int v, vector<int>&solution){
        int sigma_T = 0;
        for(int i = 1; i<= graph.nodes; i++){
            if(solution[i-1] == 0 && graph.adjMat[i-1][v-1] != INF)
                sigma_T += graph.adjMat[i-1][v-1];
        }
        return sigma_T;
    }
    
    
    int getVertices(){
        return graph.nodes;
    }

    
};

class Solve{
    
public:
    
    vector<int> calculateRCL(ProblemInstance &problemInstance, vector<int>& solution, double alpha){
        
        pair<int, int>maxMin = problemInstance.calculateWminMax(solution);
        int wmax = maxMin.first;
        int wmin = maxMin.second;

        double mu = wmin + alpha * (wmax - wmin);

        int vertices = problemInstance.getVertices();
        vector<int>RCL;
        
        for(int i = 1; i<= vertices; i++){
            if(solution[i-1] == -1){
                int greedy_val = problemInstance.greedyFunction(i, solution);
                if( greedy_val >= mu){
                    RCL.push_back(i);
                }
            }
            
        }
        return RCL;
    }

    void findRandomSolution(ProblemInstance &problemInstance, vector<int>&solution){
       int vertices = problemInstance.getVertices();
       for(int i=1; i<= vertices; i++){
            solution[i-1] = random_number(0,1);
       } 
    }
    
    void findGreedySolution(ProblemInstance &problemInstance, vector<int>&solution){
        int vertices = problemInstance.getVertices();
        solution[problemInstance.graph.maxU - 1] = random_number(0,1);
        solution[problemInstance.graph.maxV - 1] = 1 - solution[problemInstance.graph.maxU - 1];

        for(int i = 1; i<= vertices; i++){
            if(i == problemInstance.graph.maxU || i == problemInstance.graph.maxV){
                continue;
            }
            int x = problemInstance.calculateSigma_S(i, solution);
            int y = problemInstance.calculateSigma_T(i, solution);

            if(x > y){
                solution[i-1] = 1;  // push to S
            }
            else if(x < y ){
                solution[i-1] = 0;  // push to T

            }
            else{
                solution[i-1] = random_number(0, 1);
            }

        }
        
    }
    
    void findInitialSolution(ProblemInstance &problemInstance, vector<int>&solution, double alpha){
        int vertices = problemInstance.getVertices();
        
        // randomly select 50% of the nodes and put them in S or T
        for(int i = 0; i<(vertices >> 2); i++){
            int indx = random_number(1, vertices);    // randomly select a vertex
            solution[indx-1] = random_number(0,1);   // put the randomly selected vertex in set S 
        }
       
        // while there is a unassigned vertex available iterate the loop
        while(count(solution.begin(), solution.end(), -1) > 0){
            vector<int>RCL = calculateRCL(problemInstance, solution, alpha);
            
            if(RCL.empty()){
                break;
            }
            int batchSize = random_number(1, RCL.size()) ;
            int prevVal = random_number(0,1);
            while(batchSize){
                int selectedIndex = random_number(0,RCL.size() -1);
                int selectedVertex = RCL[selectedIndex];
                solution[selectedVertex-1] = (prevVal == 0) ? 1 : 0;
                batchSize-- ;
                RCL.erase(RCL.begin()+selectedIndex);
            } 
        }
    }

    int localSearch(ProblemInstance &problemInstance, vector<int>&solution){
        vector<int>currentSolution = solution;
        int currentValue = problemInstance.greedyValue(currentSolution);
        int vertices = problemInstance.getVertices();
        bool improved = true;

        // for(int itr = 0; itr < 100; itr++)
        while(improved){
            improved = false;
            // int srcVertex = random_number(1,vertices);  // randomly select a vertex to find its neighbors
            
            for(int i = 1; i <= vertices; i++ ){
                vector<int>newSolution = currentSolution;
                newSolution[i-1] = 1 - currentSolution[i-1];  // if currentsoln = 0 ,new soln = 1
                int newValue = problemInstance.greedyValue(newSolution);

                if(newValue > currentValue){
                    currentSolution = newSolution;
                    currentValue = newValue;
                    improved = true;
                    break;
                }
            }

        }
        solution = currentSolution;
        return currentValue;
    }
    
    int localSearch2(ProblemInstance &problemInstance, vector<int>&solution){
        int count = 0;
        bool change = true;
        while(change){
            count ++;
            change = false;
            for(int i = 1; i<=problemInstance.getVertices(); i++){
                
                if(solution[i-1] == 1){     //v belongs to S
                    int del = problemInstance.calculateSigma_S(i, solution) - problemInstance.calculateSigma_T(i, solution);
                    if(del > 0){
                        solution[i-1] = 1 - solution[i-1];
                        change = true; 
                        break;
                    }
                }
                if(solution[i-1] == 0){
                    int del = problemInstance.calculateSigma_T(i, solution) - problemInstance.calculateSigma_S(i, solution);
                    if(del > 0){
                        solution[i-1] = 1 - solution[i-1];
                        change = true;
                        break; 
                    }
                }                
            }
        }

        return problemInstance.greedyValue(solution);
    }
    
    /* GRASP method
    for each itr: 
        find initial solution using semi greedy method
        Local search using the solution
        find the best val from this and update best val
    */
    
    void graspAlgorithm(ProblemInstance &problemInstance, double alpha){
        
        int vertices = problemInstance.getVertices();
        vector<int>solution(vertices, -1);      // solution[i] =-1, 0, 1 -> the vertex in V, T or S respectively
        vector<int>bestSoln(vertices, -1);
        int bestVal = INT_MIN;                  // best val
        int sameCount = 0;

        for(int i = 0 ; i<= MAXITR ; i++){
            findInitialSolution(problemInstance, solution, alpha);
            int searchVal = localSearch2(problemInstance, solution);

            if(searchVal > bestVal ){
                bestVal = searchVal;
                bestSoln = vector<int>(solution);
                sameCount = 0;
            }

            solution = vector<int>(vertices, -1);
            sameCount ++; 
            // if(sameCount == 10){        // If the value of 10 iterations are the same then, 
            //                             // break the iteration phase
            //     break;
            // }

            cout<<i+1 << " : val = "<<bestVal<<endl;

        }

        cout<<"The best solution is : "<<bestVal<<endl;
        // cout<<"The solution set is : \n";
        // for(auto x: bestSoln){
        //     cout<<x<<" ";
        // }
        cout<<endl;
    }
    
    void constructSolution(ProblemInstance &problemInstance){
        int vertices = problemInstance.getVertices();
        
        vector<int>solution(vertices, -1);
        findRandomSolution(problemInstance, solution);
        cout<<" Random Solution: "<<problemInstance.greedyValue(solution)<<endl;
        
        solution = vector<int>(vertices, -1);
        findGreedySolution(problemInstance, solution);
        cout<<" Greedy Solution: "<<problemInstance.greedyValue(solution)<<endl;

        solution = vector<int>(vertices, -1);
        findInitialSolution(problemInstance, solution, 0.6);
        cout<<" SemiGreedy Solution: "<< problemInstance.greedyValue(solution)<<endl;
        
    }
};

int main(){
    Graph graph;
    graph.readFromFile("random_graph.txt");

    ProblemInstance problemInstance(graph);
    vector<int>s(graph.nodes, -1);

    // s[0] = s[1] = 1;
    // s[2] = s[4] = 0;

    // (problemInstance.calculateSigma_S(3, s) > problemInstance.calculateSigma_T(3, s) ) ? s[3] = 1 : s[3] = 0;
    
    // cout<<problemInstance.greedyValue(s)<<endl;

    Solve solve;
    solve.graspAlgorithm(problemInstance, 0.8);
    // solve.constructSolution(problemInstance);

    
}