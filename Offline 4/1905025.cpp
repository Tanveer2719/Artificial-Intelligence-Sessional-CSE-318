#include<random>
#include<algorithm>
#include "Helpers.h"

random_device r;
seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
mt19937 eng(seed);

enum enums{
    BUYING = 0,
    MAINT = 1,
    DOORS = 2,
    PERSONS = 3,
    LUG_BOOT = 4,
    SAFETY = 5,
    GOAL = 6
};

map<string, enums> mappings = {
    {"BUYING",BUYING},
    {"MAINT",MAINT},
    {"DOORS",DOORS},
    {"PERSONS",PERSONS},
    {"LUG_BOOT",LUG_BOOT},
    {"SAFETY",SAFETY},
    {"GOAL",GOAL}
};

string toUpper(string name){
    string x = "";
    for(auto c: name){
        if(c>='a' && c<='z')
            x+= toupper(c);
        else    
            x+= c;
    }
    return x;
}

class DecisionTree{

public:
    TreeNode* parent;

    DecisionTree(){
        parent = new TreeNode();
    }

    vector<vector<string>> getSubTableForaValue(vector<vector<string>> examples, int attributeIndex, string value){
        vector<vector<string>> subTable;
        for(auto row: examples){
            if(row[attributeIndex] == value){
                subTable.push_back(row);
            }
        }
        return subTable;
    }
    
    /**
     * \param table_data a table containg rows of the examples
     * 
    */
    double entropyCalculation(vector<vector<string>> table_data){
        int tableSize = table_data.size(); // total entries
        
        auto countOfClass = getGoalValueCount(table_data);

        double entropy = 0.0;
        for(auto x: countOfClass){      // for each values of the attribute
            double probability =(x.second / (double)tableSize);

            entropy -= probability * log(probability)/log(2);
        }
        return entropy;
    }

    /**
     * \param table The table information
     * \param indexOfAttribute the attribute for which we want to calculate the remainder
    */
    double remainderCalculation(vector<vector<string>> examples, string attribute, int indexOfAttribute, map<string, set<string>> attributeValues){
        // for each values of the attribute
                // create a sublist of rows having that value
                // remainder += (sublist.size() / totalsize) * entropy(sublist, GOAL)
        
        double remainder = 0.0;
        for(auto value: attributeValues[attribute]){
            auto subTable = getSubTableForaValue(examples, indexOfAttribute, value);
            remainder += (subTable.size() /(double)examples.size()) * entropyCalculation(subTable);
        } 
        return remainder;
    }

    int getAttribute(vector<vector<string>> examples, vector<string>attributes, map<string, set<string>>attributeValues){
        double initialEntropy = entropyCalculation(examples);

        double gain = 0.0;
        int attributeIndex = 0;
        // for each of the attributes except the goal
            // calculate gain = initalEntropy - remainder(attribute)
        for(int i = 0; i<attributes.size()-1; i++){    // -1 because no need to consider the goal
            double x = initialEntropy - remainderCalculation(examples, attributes[i], mappings[toUpper(attributes[i])], attributeValues);
            // cout<<" Information Gain for "<<attributes[i]<<": "<<x<<endl;
            if(x >= gain){
                gain = x;
                attributeIndex = i;
            }        
        }
        return attributeIndex; 
    }

    map<string, int> getGoalValueCount(vector<vector<string>> examples){
        map<string, int>countPlurality;
        for(auto x: examples){
            countPlurality[x[GOAL]]++;
        }
        return countPlurality;
    }

    string plurality(vector<vector<string>>examples){
        string s= "";
        int mx = 0;
        auto countPlurality = getGoalValueCount(examples);
        for(auto x: countPlurality){
            if(x.second > mx){
                mx = x.second;
                s = x.first;
            } 
        }
        return s;

    }

    bool checkLeaf(vector<vector<string>> examples){
            // check if the goal attribute value of all the rows same or not
        bool leaf = true;
        for(int i = 1; i<examples.size(); i++){
            if(examples[i][GOAL] != examples[0][GOAL]){
                leaf = false;
                break;
            }
        }
        return leaf;
    }

    /**
     * \param examples table with rows for each test
     * \param attributes list of attributes
     * \param parent_examples the example set of the parent node
     * \param attributeValues value list for each attribute
    */
    TreeNode* 
    decisionTreeLearning(vector<vector<string>> examples, vector<string> attributes, 
                        vector<vector<string>> parent_examples, 
                        map<string, set<string>> attributeValues)
    { 
        TreeNode* node = new TreeNode();

        if(examples.empty()){
            node->name = plurality(parent_examples);
            node->isLeaf = true;
            return node;
        }
        else if(checkLeaf(examples)){
            node->isLeaf = true;
            node->name = examples[0][GOAL];
            return node;
        }
        
        // if attributes are empty, 
                // make the node a leaf and name it according to the majority
        else if(attributes.size() == 0){
            node->isLeaf = true;
            node->name = plurality(examples);
            return node;
        }
        else{
            // find the attribute with max gain
            int bestAttrIndx = getAttribute(examples, attributes, attributeValues);
            string bestAttributeName = attributes[bestAttrIndx];
            node->name = bestAttributeName;

            // for each value of the best attribute
                // create new examples
                // recursively call the decisionTreeLearning function
                // make the returned subtree as the child of the current node
            
            vector<string>tmpAttributes = attributes;
            tmpAttributes.erase(tmpAttributes.begin()+bestAttrIndx);  // erase the attribute
            for(auto x: attributeValues[bestAttributeName]){
                
                auto subTable = getSubTableForaValue(examples, mappings[toUpper(bestAttributeName)], x);
                TreeNode* subtree = decisionTreeLearning(subTable, tmpAttributes, examples, attributeValues);
                
                subtree->attributeValue = x;
                subtree->parentName = node->name;
                node->children.push_back(subtree);
            }
            return node;
        }  
    }

    /**
     * \param table The table with all the information
     * \param trainSet set to construct the decision tree
    */
    void createDecisionTree(Table &table, vector<vector<string>>trainSet){
        vector<vector<string>> parentExamples = {};
        parent = decisionTreeLearning(trainSet, table.attributes, parentExamples, table.attributeValues);
    }

    string testTree(TreeNode* parent, vector<string>row){
        if(parent->isLeaf){
            return parent->name;
        }

        string attribute = parent->name;
        for(auto x : parent->children){
            if(x->attributeValue == row[mappings[toUpper(attribute)]]) {
                return testTree(x, row);
            }
        }
        return "";
    }

    int testDecisionTree(vector<vector<string>>testSet){
        int successCount = 0;
        for(auto x: testSet){
            string goal = testTree(parent, x);
            // cout<<goal<<endl;
            if(goal == x[GOAL]){
                successCount++;
            }
        }
        return successCount;
    }

    void dfs(TreeNode *parent, bool flag = false){
        if(flag){
            cout<<"label: "<<parent->name<<" "<<" parentName: "<<parent->parentName<<" attribute name: "<<parent->attributeValue<<endl;
        }
        for(auto x: parent->children){
            dfs(x, flag);
        }

        if(!flag){
            parent->children.clear();
            delete(parent);
        }
    }

    void printDecisionTree(){
        dfs(parent, true);
    }

    void freeTree(){
        dfs(parent);
    }
};

void 
splitDataSet(vector<vector<string>>tableData,
                vector<vector<string>> &trainSet,vector<vector<string>> &testSet)
{
    vector<vector<string>> shuffledData = tableData;
    // shuffle the data set
    shuffle(shuffledData.begin(), shuffledData.end(),eng);
    
    int sz = 0.8*tableData.size();
    trainSet.assign(shuffledData.begin(), shuffledData.begin() + sz);
    testSet.assign(shuffledData.begin() + sz, shuffledData.end());
}

int main(){
    FileOperations fileOperations;
    fileOperations.readFile("car evaluation dataset/car.data");
    Table table = fileOperations.getTable();
    vector<vector<string>> trainSet, testSet;

    vector<int>successList;
    for(int i = 0; i<20; i++){
        trainSet.clear(), testSet.clear();
        splitDataSet(table.table_data, trainSet, testSet);
        DecisionTree dTree;
        dTree.createDecisionTree(table, trainSet);
        successList.push_back(dTree.testDecisionTree(testSet));
        dTree.freeTree();
    }

    fileOperations.writeFile(successList, testSet.size(), table.table_data.size());
    
}