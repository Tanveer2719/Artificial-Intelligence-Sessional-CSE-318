
# A Report On “Decision tree learning”

Prepared by: Tanveer Rahman (1905025)


## Description

Decision tree learning is a supervised learning approach. Decision trees are among the most popular machine learning algorithms given their intelligibility and simplicity. The goal is to create a model that predicts the value of a target variable based on several input variables. 

In our given Offline we used 'Decision Tree Learning' for the 'Car Evaluation'. The dataSet for the car Evaluation is also provided here

## The Implementation of the Algorithm
This is a recursive algorithm to create the decision tree. The algorithm is as follows: 

```bash
decisionTreeLearning(examples, attributes, parent_examples, attributeValues)
    { 
        if examples.empty()
            node = plurality(parent_examples)
            return node

        if leaf node    
            node = examples[0][GOAL]
            return node

        if attributes are empty, 
            make the node a leaf and name it according to the majority of goals
            return node;
        
        
        node = attribute with max gain;

        for each value of the best attribute
            create new examples list (exs)
            // recursively call the decisionTreeLearning function 
            subtree = decisionTreeLearning(exs, tmpAttributes, examples, attributeValues);
            //make the returned subtree as the child of the current node
            
            node->children. = {node->children} U {subtree};
        
        return node;
```

The Best attribute is calculated according to the maximum 'Information Gain' among all the attributes. 

            INFORMATION GAIN = BASE ENTROPY - REMAINDER

Entropy calculation
```bash
double entropyCalculation(vector<vector<string>> &table_data){
        int tableSize = table_data.size(); // total entries
        
        auto countOfClass = getGoalValueCount(table_data);

        double entropy = 0.0;
        for(auto x: countOfClass){      // for each values of the attribute
            double probability =(x.second / (double)tableSize);

            entropy -= probability * log(probability)/log(2);
        }
        return entropy;
    }
```

Remainder calculation:
```bash
double remainderCalculation(vector<vector<string>>&examples, string attribute, int indexOfAttribute, map<string, set<string>>&attributeValues){
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
```

## TrainSet data and TestSet data
We Divided the data set randomly between training (80%) and testing (20%) sets. By using our algorithm we trained  a  decision  tree classifier from  the training data and  applied  it  on the  test data. 

how data is splitted ?
Used this for random splitting
                random_device r;
                seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
                mt19937 eng(seed);
```bash
void 
splitDataSet(vector<vector<string>>tableData,
                vector<vector<string>>&trainSet,vector<vector<string>>&testSet)
{
    vector<vector<string>> shuffledData = tableData;
    // shuffle the data set
    random_shuffle(shuffledData.begin(), shuffledData.end());
    
    int sz = 0.8*tableData.size();
    trainSet.assign(shuffledData.begin(), shuffledData.begin() + sz);
    testSet.assign(shuffledData.begin() + sz, shuffledData.end(), eng);
}
``` 

## Result
After 20 iterations of the random splitting of the data, training and testing of the data, the results are provided in the results folder.

