#ifndef _HELPERS
#define _HELPERS

#include<iostream>
#include<string>
#include<vector>
#include<set>
#include<map>
#include<fstream>
#include<filesystem>
#include<cmath>

using namespace std;

class Table{
public:
    vector<string>attributes = {"buying", "maint","doors", "persons","lug_boot","safety","goal"};
    map<string, set<string>>attributeValues; // for mapping of the values of the attributes
    vector<vector<string>>table_data;       // used to create the table for the decision tree
                                            // as used in the book

    void printTable(){
        cout<<"The data table is as follows, rows = "<< table_data.size()<<"\n";
        for(auto x: attributes){
            cout<<x<<"\t";
        }
        cout<<endl;
        for(int i = 0; i<table_data.size(); i++){
            for(auto x: table_data[i]){
                cout<<x<<"\t";
            }
            cout<<endl;
        }  
    }

    void printAttributes(){
        for(auto x: attributes){
            cout<<x<<" :";
            for(auto y: attributeValues[x]){
                cout<<y<<" ";
            }
            cout<<endl;
        }
    }
};

class FileOperations{
    ofstream filewriter;
    ifstream filereader;
    Table table;
public:
    void readFile(string fileName){
        filereader.open(fileName);
        if(! filereader.is_open()){
            cout<<"error in file opening\n";
            return;
        }
        string s;
        while(getline(filereader, s)){
            vector<string>tablerow;
            string w = "";
            int i = 0;
            for(auto x: s){
               if(x == ','){
                    tablerow.push_back(w);
                    table.attributeValues[table.attributes[i]].insert(w); // list of values for the attributes
                    i++;
                    w = "";
                }else{
                    w += x;
                }
            }
            tablerow.push_back(w);  // extracted all the values of the attributes
            table.attributeValues[table.attributes[i]].insert(w);
            table.table_data.push_back(tablerow);     // push the row to the table
        }
        filereader.close();

        // table.printAttributes();
    }

    /**
     * \param resultList The list of success count per iteration
     * \param testEntries no of test data per iteration
     * \param totalEntries total no of entries in the dataset
    */
    void writeFile(vector<int>& resultList, int testEntries, int totalEntries){
        ofstream write("result.html");
        if(!write.is_open()){
            cout<<"error in opening file\n";
            return ;
        }
        int totalSuccess = 0;
        for(auto x: resultList){
            totalSuccess += x;
        }
        double meanSuccess = totalSuccess / (double) resultList.size();

        double squareSum = 0.0;
        for(auto x: resultList){
            squareSum += (x - meanSuccess) * (x - meanSuccess);
        }
        double stdDeviation = sqrt(squareSum / resultList.size());

        write<<"<!DOCTYPE html>\n<html>\n<head></head>\n<body>\n";
        write<<"<table border=\"1\">\n";
        write<<"<tr>\n<th>Itr no.</th>\n<th>TestSetSize</th>\n<th>Success</th>\n<th>Percentage</th>\n<th>MeanSuccess</th>\n<th>StdDeviation</th>\n</tr>";

        for(int i = 0; i<resultList.size(); i++){
            if(i == 0){
                write<<"<tr>\n<td>"<<(i+1)<<"</td>\n<td>"<<testEntries<<"</td>\n<td>"<<resultList[i]<<"</td>\n<td>"<<(resultList[i]*100.00)/testEntries<<"</td>\n<td rowspan=\""<<resultList.size()<<"\" align=\"center\">"<<meanSuccess<<"</td>\n<td rowspan=\""<<resultList.size()<<"\" align=\"center\">"<<stdDeviation<<"</td>\n</tr>\n";
            }
            else{
                write<<"<tr>\n<td>"<<(i+1)<<"</td>\n<td>"<<testEntries<<"</td>\n<td>"<<resultList[i]<<"</td>\n<td>"<<(resultList[i]*100.00)/testEntries<<"</td>\n</tr>\n";
            }
        }

        write<<"</table>\n</body>\n</html>\n";

        write.close();
    }

    Table getTable(){
        return table;
    }
};
 
class TreeNode{
public:
    bool isLeaf; 
    string name, attributeValue, parentName;
    vector<TreeNode*>children;

    TreeNode(){
        isLeaf = false;
        name = "";
        parentName="";
    }

};

#endif