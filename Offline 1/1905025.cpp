#include<bits/stdc++.h>
#include "Merge_Sort.h"
using namespace std;

struct comp;

class Grid{
public:
    Grid* parent;
    int f, g, h, p_moves;    // f = g + h
    vector<vector<int>> grid;
    vector<vector<int>>des_grid;
    vector<int>arr;
    
    Grid(){
        parent = nullptr;
        f = 0, g = 0, h = 0, p_moves = 0;
    }

    void print_grid(){
        cout<<grid.size()<<endl;
        for(int i = 0;i<grid.size();i++){
            for(int j = 0;j<grid[i].size(); j++){
                if(grid[i][j] == 0){
                    cout<<'*'<<" ";
                }
                else{
                    cout<<grid[i][j]<<" ";
                }
            }
            cout<<endl;
        }
        cout<<endl;
    }

    int find_hamming_distance(){
        int n = grid.size();
        int ret = 0;
        for(int i = 0; i<n; i++){
            for(int j = 0;j<n; j++){
                if(grid[i][j] != 0 && grid[i][j] != (i*n + (j+1)))
                {    
                    ret++;
                }
            }
        }
        return ret;
    }

    int find_manhattan_distance(){
        int n = grid.size();
        int m_d = 0;

        for(int i = 0;i<n; i++){
            for(int j = 0;j<n; j++){
                int x = grid[i][j];
                if(x != 0){
                    if(x%n){
                        m_d += abs(x/n - i) + abs(x%n - (j+1)); 
                    }
                    else{
                        m_d += abs(x/n - (i+1)) + abs(n - (j+1));
                    }
                }
            }
        }

        return m_d;

    }

    // operator overloading for checking the equality of two grids
    bool operator == (Grid a){
        int n = grid.size();
        for(int i = 0; i<n; i++ ){
            for(int j = 0; j<n; j++){
                if(grid[i][j] != a.grid[i][j])
                    return false;
            }
        }
        return true;
    }

};

// compare for minimum priority calculation
struct comp{
    bool operator()(Grid& a, Grid& b){
        return a.f > b.f ;
    }
};

void construct_path(Grid s){
    Grid *temp = &s;
    stack<Grid*> st;
    st.push(temp);
    while(temp->parent != nullptr){
        temp = temp->parent;
        st.push(temp);
    }
    while(!st.empty()){
        st.top()->print_grid();
        st.pop();
    }

}

// returns the blank positon in the grid
pair<int,int> blank_position(const Grid& a){
    int n = a.grid.size();
    for(int i = 0;i<n; i++){
        for(int j = 0;j<n; j++){
            if(a.grid[i][j] == 0){
                return make_pair(i, j);
            }
        }
    }
    return make_pair(-1,-1);
}

// check if the grid is already in the set
bool inset(Grid x, vector<Grid>c_set){
    int n = c_set.size();
    for(int i = 0;i<n;i++){
        if(x == c_set[i]){
            return true;
        }
    }
    return false;
}

// checks the equality of two grids
bool equal(vector<vector<int>> a, vector<vector<int>>b){
    int n = a.size();
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            if(a[i][j] != b[i][j]){
                return false;
            }
        }
    }
    return true;
}

// we need to create new grids by moving the blankspace and then push it to the queue
void add_to_queue(const Grid current, int o_x, int o_y, int n_x, int n_y, priority_queue<Grid, vector<Grid>, comp>& pq,vector<Grid>&c_set, int mode ){
    Grid new_grid = current;
    swap(new_grid.grid[n_x][n_y], new_grid.grid[o_x][o_y]); // swap the blank space

    new_grid.g += 1;
    new_grid.p_moves++;
    if(mode == 0){
        new_grid.f = new_grid.g + new_grid.find_hamming_distance();
    }else{
        new_grid.f = new_grid.g + new_grid.find_manhattan_distance();
    }

    if((current.parent != nullptr) && equal(current.parent->grid, new_grid.grid)){
        // if child is same as parent of current grid's parent then do not push
        return;
    }
    if(!inset(new_grid, c_set)){
        // if new grid is not in the closed set then push it
        Grid *temp = new Grid();
        *temp = current;
        new_grid.parent = temp;
        pq.push(new_grid);
        
        /* testing purpose*/
        /*
            cout<<"pushed to the pq: "<<endl;
            cout<<"priority: "<<new_grid.f<<endl;
            new_grid.print_grid();

            cout<<"priority queue size= "<<pq.size()<<endl;
        */
    }
    
}

// mode = 0 then hamming , mode = 1 then manhattan
bool astar( Grid& start, Grid& goal, int mode){

    if(mode){
        cout<<"****** Manhattan method*********\n"<<endl;
    }else{
        cout<<"****** Hamming method********\n"<<endl;
    }

    priority_queue<Grid, vector<Grid>, comp> pq; // open set (min priority queue)
    vector<Grid> c_set; // closed set
    int n = start.grid.size();

    pq.push(start);
    while(!pq.empty()){
        Grid current = pq.top();

        /*testing purpose
        
            cout<<"current grid: "<<endl;
            current.print_grid();
        */

        c_set.push_back(pq.top());
        pq.pop();

        if(current == goal){
            cout<<"goal grid reached\ntotal no of moves needed = "<<current.p_moves<<"\n"<<endl;
            construct_path(current);
            return true;

            // construct_path(current);
            // return true;
        }
        

        // add the neighbours to the priority queue
        pair<int,int>ij = blank_position(current);  // find the index of the blank
        int i = ij.first, j = ij.second;
        
        if(i-1 >= 0){   // the row is 1, 2 ..
            add_to_queue(current, i, j, i-1, j, pq, c_set, mode);
            
        }
        if(i+1 < n){    // row is 0 1 ... n-2
            add_to_queue(current, i, j, i+1, j, pq, c_set, mode);
        }
        if(j-1 >= 0){
            add_to_queue(current, i, j, i, j-1,pq, c_set, mode);
        }
        if(j+1 < n){
            add_to_queue(current, i, j, i, j+1,pq, c_set, mode);
        }
    }
    return false;
}

bool solvable(Grid& init){

    /*  
        when n is odd, an n-by-n board is solvable 
        if and only if its number of inversions is even.
    */

   /*
        when n is even, an n-by-n board is solvable 
        if and only if the number of inversions plus the row of the blank square is odd(0 indexed).
   */

    int n = init.grid.size();
    int m = init.arr.size();

    int inv_pair = merge_sort(init.arr, 0, m-1);
    cout<<"\nInversion pair no: "<<inv_pair<<endl;

    pair<int,int>ij = blank_position(init);

    if(((n%2) && !(inv_pair%2)) || (!(n%2) && ((inv_pair + ij.first)%2))){
        return true;
    }
    return false;

}

int main(){
    Grid init;
    Grid des;
    int k;
    cin>>k;

    // create the inital grid
    init.grid = vector<vector<int>>(k);
    for(int i = 0; i<k; i++){
        init.grid[i] = vector<int>(k);
        for(int j = 0;j<k; j++){
            string s;
            cin>>s;
            if(s =="*" || s == "0"){
                init.grid[i][j] = 0;
            }else{
                init.grid[i][j] = stoi(s);
                init.arr.push_back(stoi(s));
            }
            
        }
    }

    // create the goal grid
    des.grid = vector<vector<int>>(k);
    for(int i = 0;i<k; i++){
        des.grid[i] = vector<int>(k);
        for(int j = 0; j<k; j++){
            if(((i*k) + (j+1)) != (k*k))
                des.grid[i][j] = (i*k) + (j+1);
            else{
                des.grid[i][j] = 0;
            }
        }
    }

    if(! solvable(init)){
        cout<<"Not solvable"<<endl;
    }else{
        cout<<"solvable"<<endl;
        astar(init,des, 1); // 1 for manhattan method 0 for hamming method
    }


    





}

