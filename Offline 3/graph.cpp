#include<bits/stdc++.h>
#define INF 1000000
#define MAXITR 100

/*
The idea got from the article: 
    https://mpc.zib.de/archive/2016/3/Santis2016_Article_ANonmonotoneGRASP.pdf
*/
using namespace std;

list<pair<int,int>> benchMarks({
    {1,12078},{11,627},{12,621},{13,645}, {14 ,3187}, {15,3169}, {16,3172}, {34,1541}, {35, 8000}, {43, 7027}, {44, 7022}, {45, 7020}, {48,6000}, {49, 6000}, {50, 5988}
    });


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
    
    pair<int, int> readFromFile(string name){
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

        return make_pair(nodes, edges);

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

    //random
    // no calculation, just assign each vertex randomly
    void findRandomSolution(ProblemInstance &problemInstance, vector<int>&solution){
       int vertices = problemInstance.getVertices();
       for(int i=1; i<= vertices; i++){
            solution[i-1] = random_number(0,1);
       } 
    }
    
    // greedy method
    // if there is no edge between two nodes then, they are 
    // randomly partitioned
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
    
    // semigreedy method
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
    
    /* GRASP method
    for each itr: 
        find initial solution using semi greedy method
        Local search using the solution
        find the best val from this and update best val
    */
    
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

            cout<<i+1 << " : val = "<<bestVal<<endl;

        }

        ret.push_back(avgLocalSearchItr);
        ret.push_back(avgLocalSearchVal);
        ret.push_back(MAXITR);
        ret.push_back(bestVal);
    }
    
    vector<int> constructSolution(ProblemInstance &problemInstance){
        int vertices = problemInstance.getVertices();
        vector<int>ret;
        vector<int>solution(vertices, -1);

        findRandomSolution(problemInstance, solution);

        ret.push_back(problemInstance.greedyValue(solution));
        
        solution = vector<int>(vertices, -1);
        findGreedySolution(problemInstance, solution);
        ret.push_back(problemInstance.greedyValue(solution));

        solution = vector<int>(vertices, -1);
        findInitialSolution(problemInstance, solution, 0.6);
        ret.push_back(problemInstance.greedyValue(solution));

        graspAlgorithm(problemInstance, 0.7, ret);
        

        return ret;
    }
};

int main(){
    // Graph graph;
    // graph.readFromFile("random_graph.txt");

    // ProblemInstance problemInstance(graph);
    // vector<int>s(graph.nodes, -1);

    // // s[0] = s[1] = 1;
    // // s[2] = s[4] = 0;

    // // (problemInstance.calculateSigma_S(3, s) > problemInstance.calculateSigma_T(3, s) ) ? s[3] = 1 : s[3] = 0;
    
    // // cout<<problemInstance.greedyValue(s)<<endl;

    // Solve solve;
    // solve.graspAlgorithm(problemInstance, 0.8);
    // solve.constructSolution(problemInstance);

    for (auto x: benchMarks){
        Graph graph;
        string s = "set1/g"+to_string(x.first) + ".rud";
        
        pair<int , int> nodeEdge = graph.readFromFile(s);
        cout<<"For graph "<<x.first<<" nodes: "<<nodeEdge.first<<" edges: "<<nodeEdge.second<<endl;

        ProblemInstance problemInstance(graph);
        Solve solve;
        vector<int> soln = solve.constructSolution(problemInstance);

        for(auto y: soln){
            cout<<y<< " ";
        }
        cout<<endl;        
    }

    
}