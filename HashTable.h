//
// Created by Evan Chang on 2019-05-01.
//

#ifndef CHEATERS_HASHTABLE_H
#define CHEATERS_HASHTABLE_H

#include <cstdlib>  // Provides size_t
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

const int TABLE_SIZE = 202343;

class Hash {

//Public contructor, destructor, and add/print methods
public:
    Hash(int numFiles);

    ~Hash();

    void add(string chunk, int curFileNum, string fName);

    void printCheaters(int minimum, int size, vector<string> files);

//Private nodes for hash table and printing, table for keys, and 2D array for collisions
private:

    struct Node {
        string file;
        int fileNum;
        Node *next;
    };

    struct printNode {
        int num;
        string f1;
        string f2;
    };

    int** collisions;
    int numFiles;
    struct Node table[TABLE_SIZE];

	 //Private helpers
    int hashHelper(string chunk);
    bool lessThan(const printNode& a, const printNode& b);
};

Hash::Hash(int numFiles) {
    //Create the 2D array of collisions
    collisions = new int*[numFiles];
    for(int i = 0; i < numFiles; i++){
        collisions[i] = new int[numFiles];
        for(int j = 0; j< numFiles; j++){
            collisions[i][j] = 0;
        }
    }


	 //Create the table of keys
    for(int i = 0; i< TABLE_SIZE; i++){
        table[i];
        table[i].fileNum = -1;
        table[i].file = "";
        table[i].next = NULL;
    }
    this->numFiles = numFiles;
}

Hash::~Hash() {
    for(int i = 0; i < numFiles; i++)
    {
        delete collisions[i];
    }

    delete[] collisions;
}

//Compare two structs for printing
bool Hash::lessThan(const printNode& a, const printNode& b){
    return (a.num < b.num);
}

//Determine index for hashing
int Hash::hashHelper(string chunk) {

    unsigned long long total = 0;
    for(int i =0; i< chunk.length(); i++){
        total += chunk[i] * pow(37,i%5);
    }
    return (int)(total % TABLE_SIZE);
}

//Add a struct to the hash
void Hash::add(string chunk, int curFileNum, string fName) {


    int index = hashHelper(chunk);
    Node* current = &table[index];
   
	 //If there is no chunk already there, add it to hash 
    if(current->fileNum == -1){
        table[index].file = fName;
        table[index].fileNum = curFileNum;
        return;
    }
	 //Case where there is only one collision
    if(current->next == NULL){
        collisions[current->fileNum][curFileNum]++;
        table[index].next = new Node{fName, curFileNum, NULL};
        return;
    }
	 //Reach end of linked list
    while(current->next != NULL){
        collisions[current->fileNum][curFileNum]++;
        current = current->next;
    }
    collisions[current->fileNum][curFileNum]++;
    current->next = new Node{fName, curFileNum, NULL};

}

//Print all collisions over a certain number
void Hash::printCheaters(int minimum, int size, vector<string> files) {

	 //Add all collisions to a vector with structs
    vector<printNode> results;
    cout<<endl<<"Collisions:"<<endl;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(collisions[i][j]>=minimum && j>i){
                results.push_back({collisions[i][j], files[i], files[j]});
            }
        }
    }

    //Sort structs
    for(int i=0; i<results.size(); i++){
        for(int j = 0; j<results.size()-1; j++){

            if(lessThan(results[j], results[j+1])){

                printNode temp = {results[j].num, results[j].f1, results[j].f2};
                results[j].num = results[j+1].num;
                results[j].f1 = results[j+1].f1;
                results[j].f2 = results[j+1].f2;
                results[j+1].num = temp.num;
                results[j+1].f1 = temp.f1;
                results[j+1].f2 = temp.f2;
            }
        }
    }
	 //Print them all
    for(int i = 0; i<results.size(); i++){
        cout<<results[i].num << ": " << results[i].f1 << ", " << results[i].f2 << endl;
    }
}



#endif //CHEATERS_HASHTABLE_H
