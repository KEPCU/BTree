#include<iostream>
#include <string>
#include <fstream>
#include <climits>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>

using namespace std;
#define LineBreak 10


#define MinimumSize 2
#define MaximumSize 4

struct BTree {
  int Keys[MaximumSize + 1];
  BTree *Banchs[MaximumSize + 1];
  int KeysNumber;
};

BTree* Root;

BTree* Initialize(int key, BTree* branch) {
  BTree* newNode;
  newNode = (BTree*)malloc(sizeof(BTree));
  newNode->Keys[1] = key;
  newNode->KeysNumber = 1;
  newNode->Banchs[0] = Root;
  newNode->Banchs[1] = branch;
  return newNode;
}

void Search(int key, BTree* bTree, int height, int indexBranch, int* position) {
  if (bTree == NULL){ 
    cout<<"no found: "<< key<<endl;
    return;
  }
  if (key < bTree->Keys[1]) *position = 0;
  else {
    for (*position = bTree->KeysNumber; (key < bTree->Keys[*position] && *position > 1); (*position)--);
    if (key == bTree->Keys[*position]) {
      cout<<"found: "<<key<<" in ("<<height+1<<" x "<<indexBranch<<")"<<endl;
      return;
    }
  }
  height++;
  Search(key, bTree->Banchs[*position], height, *position, position);
  return;
}

void Print(BTree* bTree) {
  if (bTree != NULL) {
    int i;
    for (i = 0; i < bTree->KeysNumber; i++) {
      Print(bTree->Banchs[i]);
      cout<<bTree->Keys[i + 1]<<", ";
    }
    Print(bTree->Banchs[i]);
  }
}

void AddBranch(int key, int position, BTree* bTree, BTree* branch) {
  int i = bTree->KeysNumber;
  for ( ; i > position; i--) {
    bTree->Keys[i + 1] = bTree->Keys[i];
    bTree->Banchs[i + 1] = bTree->Banchs[i];
  }
  bTree->Keys[i + 1] = key;
  bTree->Banchs[i + 1] = branch;
  bTree->KeysNumber++;
}

void SplitBranch(int key, int* ptrKey, int position, BTree* branch, BTree** newBranch, BTree* bTree) {
  int minimumBranchs;
  if (position <= MinimumSize) minimumBranchs = MinimumSize;
  else minimumBranchs = MinimumSize + 1;

  *newBranch = (BTree*)malloc(sizeof(BTree));

  for(int i = minimumBranchs + 1; i <= MaximumSize; i++) {
    (*newBranch)->Keys[i - minimumBranchs] = bTree->Keys[i];
    (*newBranch)->Banchs[i - minimumBranchs] = bTree->Banchs[i];
  }

  bTree->KeysNumber = minimumBranchs;
  (*newBranch)->KeysNumber = MaximumSize - minimumBranchs;

  if (position > MinimumSize) AddBranch(key, position - minimumBranchs, *newBranch, branch);
  else AddBranch(key, position, bTree, branch); 
  
  *ptrKey = bTree->Keys[bTree->KeysNumber];
  (*newBranch)->Banchs[0] = bTree->Banchs[bTree->KeysNumber];
  bTree->KeysNumber--;
}

bool InsertKey(int key, int* ptrUpperKey, BTree* node, BTree** branch) {
  if (!node) {
    *ptrUpperKey = key;
    *branch = NULL;
    return true;
  }

  int position = 0;
  if (key >= node->Keys[1]) {
    position = node->KeysNumber;
    while(position > 1 && key < node->Keys[position]) position--;
    if (key == node->Keys[position]) {
      cout<<key<<" Repeated "<<endl;
      return false;
    }
  }

  if(InsertKey(key, ptrUpperKey, node->Banchs[position], branch)) {
    if(node->KeysNumber < MaximumSize) AddBranch(*ptrUpperKey, position, node, *branch);
    else {
      SplitBranch(*ptrUpperKey, ptrUpperKey, position, *branch, branch, node);
      return true;
    }
  }
  return false;
}

void Insert(int key) {
  BTree* branch;
  int  i;
  if(InsertKey(key, &i, Root, &branch)) Root = Initialize(i, branch);
}

void PrintTree(BTree* bTree, int height, int indexBranch) {
  height++;
  if (bTree != NULL) {
    cout<<"("<<height<<" x "<<indexBranch<<") ";
    for (int i = 0; i < bTree->KeysNumber; i++) cout<<" "<<bTree->Keys[i + 1]<<" ";
    cout<<endl;
    for (int i = 0; i <= bTree->KeysNumber; i++) PrintTree(bTree->Banchs[i],height,i);
  }
}

void RemoveKey(BTree* bTree, int position) {
  int i = position + 1;
  for(int i = position + 1; i <= bTree->KeysNumber; i++) {
    bTree->Keys[i - 1] = bTree->Keys[i];
    bTree->Banchs[i - 1] = bTree->Banchs[i];
  }
  bTree->KeysNumber--;
}

void Read(string fileName) {
  vector<vector<string>> content;
  vector<string> row;
  string line, word;
  fstream file(fileName, ios::in);
  if(file.is_open()) {
    while(getline(file, line)) {
      row.clear();
      stringstream str(line);
      while(getline(str, word, ',')) row.push_back(word);
      content.push_back(row);
    }
  }
  else {
    cout<<"!! READ ERROR"<<endl;
    return;
  }
  for(int i = 0; i < content.size(); i++) {
    for(int j = 0; j < content[i].size(); j++) {
      Insert(stoi(content[i][j]));
    }
  }
  cout<<"DATA ADDED !!"<<endl;
}

void GetString(BTree* bTree,fstream* ExportFile) {
  if (bTree != NULL) {
    int i;
    for (i = 0; i < bTree->KeysNumber; i++) {
      GetString(bTree->Banchs[i],&(*ExportFile));
      (*ExportFile)<<to_string(bTree->Keys[i + 1])+",";
    }
    GetString(bTree->Banchs[i],&(*ExportFile));
  }
}

void ExportSortedFile(BTree* bTree,string fileName) {
  if(bTree != NULL) {
    fstream ExportFile;
    ExportFile.open("FILESorted"+fileName, ios::out | ios::app);
    GetString(bTree,&ExportFile);
    ExportFile.close();
    cout<<"EXPORTED FILE !!"<<endl;
  }
}

int main() {
  int n, key;
  string option = "";
  string fileName = "";
  //string fileName = "input.csv";
  cout<<"Enter the file name: ";
  cin>>fileName;
  Read(fileName + ".csv");

  cout<<"————————————————————————————————"<<endl;
  cout<<"search numbers: ";
  cin>>n;
  for(int i = 0; i < n; i++) {
    cout<<"number (key): ";
    cin>>key;
    int j;
    Search(key,Root,-1,0,&j);
  }

  cout<<"————————————————————————————————"<<endl;
  ExportSortedFile(Root,fileName + ".csv");

  cout<<"————————————————————————————————"<<endl;
  cout<<"Print? (Y/N): ";
  cin>>option;
  if(option == "y" || option == "Y") Print(Root);
  cout<<endl;
  
  cout<<"————————————————————————————————"<<endl;
  cout<<"Print Tree? (Y/N): ";
  cin>>option;
  if(option == "y" || option == "Y") {
    cout<<"(Height x Branch)"<<endl;
    PrintTree(Root,-1,0);
  }

  
  cout<<"————————————————————————————————"<<endl;
  cout<<"number of removes: ";
  cin>>n;

  for(int i = 0; i < n; i++) {
    cout<<"number (key): ";
    cin>>key;
    int j;
    Search(key,Root,-1,0,&j);
    RemoveKey(Root,key);
    cout<<"Removing :"<<key<<endl;
    Search(key,Root,-1,0,&j);
  }
}
