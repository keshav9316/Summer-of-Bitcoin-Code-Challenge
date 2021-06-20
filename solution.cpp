#include<bits/stdc++.h>
typedef long long ll;
using namespace std;

// global variables
string input = "mempool.csv";
set<string> validTransactionId; 
int totalBlockWeight = 4000000; 

// transaction class
class TransactionInfo {
    public:
        string tx_id;
        int fee;
        int weight;
        vector<string> parents;
};

pair<string, TransactionInfo*> addTransactionInfo(vector<string>& trxInfo){
    auto trx = new TransactionInfo(); // trx is the new TransactionInfo object
    trx->tx_id = trxInfo[0]; // trx term stands for transaction
    trx->fee = stoi(trxInfo[1]);
    trx->weight = stoi(trxInfo[2]);
    vector<string> pid;
    for (int i = 3; i < trxInfo.size(); i++){
        pid.push_back(trxInfo[i]);
    }
    trx->parents = pid;
    return {trxInfo[0], trx};
}

// read the csv file : O(1)
void readCSV(string filename, unordered_map<string, TransactionInfo*>& trx){
    ifstream fin(input);
    vector<string> trxDetails;
    string temp,line,word;
    getline(fin,line);
    while(getline(fin,line)){
        trxDetails.clear();
        stringstream s(line);
        while(getline(s,word,',')){
            trxDetails.push_back(word);
        }
        pair<string,TransactionInfo*> p = addTransactionInfo(trxDetails);
        trx[p.first] = p.second;
    }
    fin.close();

    cout << "Total Transactions Read: " << trx.size() << endl;
}

// outputs the valid transactionIDs in a new text file
void getOutput(vector<string>& output, string filename){
    ofstream myfile(filename);
    for(auto s:output){
        myfile << s << endl;
    }
    myfile.close();
}

// return true if transaction is valid
bool isValidTransaction(TransactionInfo* trx,set<string>& validTransactionId){
    //If all the parents of trx are present in validTransactionId then it is valid
    for(auto parent : trx->parents){
        if(validTransactionId.find(parent) == validTransactionId.end())
            return false;
    }
    return true;
}

// main function
int main(){
    unordered_map<string, TransactionInfo *> trx;

    //read CSV file
    readCSV(input, trx);
    
    set<pair<float, TransactionInfo *>, greater<pair<float, TransactionInfo *>>> transactionSet; //  descending order for max fee/weight ratio
    set<string> includedTransactionSet; // include all transactions included in block
    vector<string> output; // maintains order of output

    // inserting {fee/weight, transactionInfo object} in ordered set
    for(auto i:trx){
        transactionSet.insert({(float)i.second->fee / (float)i.second->weight, i.second});
    }

    int currentBlockWeight = 0;
    int totalFee = 0;
    while(!transactionSet.empty() && (currentBlockWeight < totalBlockWeight)){
        bool found = false;
        for(auto i = transactionSet.begin(); i != transactionSet.end(); i++){
            TransactionInfo* currentTransaction = (*i).second;
            int currFee = currentTransaction->fee;
            int currWeight = currentTransaction->weight;
            if(isValidTransaction(currentTransaction, validTransactionId) && currentBlockWeight + currWeight <= totalBlockWeight){
                currentBlockWeight += currWeight;
                includedTransactionSet.insert(currentTransaction->tx_id);
                output.push_back(currentTransaction->tx_id);
                totalFee += currFee;
                transactionSet.erase(i);
                 found = true;
                break;
            }
        }

         if(!found){
             break;
         }
         
    }
    //output
    cout << "Number of transaction in final block: " << includedTransactionSet.size() << endl;
    cout << "Total fee in current block: " << totalFee << endl;
    cout << "Total weight in current block: " << currentBlockWeight << endl;
    getOutput(output, "block.txt");

    return 0;
}
