#include <iostream>
#include <queue>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <istream>
#include "HashTable.h"

using namespace std;

//Helper to find all n word chunks in a file
vector<string> findChunks(string fName, int size){
	
	 //Open file
    ifstream inFile;
    inFile.open(fName);
    string word;
	 
    vector<string> chunks;
    queue<string> current;
    int count = 0;

    //Keep reading in words for the chunks
    while(getline(inFile,word,' ')){
        count++;
        string alNumWord = "";

		  //Only get alphanumeric characters
        for(int i = 0; i < word.length(); i++) {
            if (isalnum(word[i])) {
                alNumWord+= word[i];
            }
        }

	     //Add word to queue of chunk
        current.push(alNumWord);
        if(count >= size){
            string toAdd = "";
            for(int i = 0; i<size; i++){
                string temp = current.front();
                current.pop();
                toAdd += temp;
                current.push(temp);
            }

            chunks.push_back(toAdd);
            current.pop();
            count--;
        }
    }
	 inFile.close();
    return chunks;
}

//Helper to put files from directory in a vector
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

int main(int argc, char *argv[]) {

	 //Get arguments from command line
    string directory = argv[1];
    vector<string> files;
    getdir(directory, files);
    int chunkLength = atoi(argv[2]);
    int min = atoi(argv[3]);

	 //Remove two . .. files
    files.erase(files.begin());
    files.erase(files.begin());

    //Create Hash object
    Hash hash (files.size());

	 //Go through all files are get chunks
    vector<string> chunks;
    for(int i = 0; i<files.size(); i++){
        
        chunks = findChunks(directory +files[i], chunkLength);
        for(int j=0; j<chunks.size();j++){
				//Add to hash
            hash.add(chunks[j], i, files[i]);
        }
    }

    hash.printCheaters(min, files.size(), files);
   
}
