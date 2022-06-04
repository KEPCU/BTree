#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <climits>
using namespace std;

int main() {
  std::ofstream myFile("input.csv");
  int ll = INT_MIN;
  int ul = INT_MAX;
  for(int i = 0; i <= ul; i += 1000) {
    string cad = "";
    for(int j = i; j <= i+1000; j++) cad += to_string(j) + ",";
    myFile << cad+"\n";
  }
  myFile.close();
  return 0;
}


