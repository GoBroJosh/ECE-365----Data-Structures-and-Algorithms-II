/* Joshua Go
DSA II - Prof. Sable
Programming Assignment #1 */
#include "hash.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <string.h>
/*Credit to Will Chen for introducing me to regex for parsing*/
#include <regex>

void spell(std::string in, std::string out);
void parse(std::string l);
bool hasNumbers(std::string word);

hashTable dictionary(50000);
std::vector<std::string> word;
std::regex re("\[-'[:alnum:]]+");

std::string inputPath;
std::ifstream inFile;

std::string outputPath;
std::ofstream outFile;

std::string inputline;

int main(){
  std::string dictPath;
  std::fstream dictFile;
  std::string dictline;

  std::cout << "Enter name of dictionary: ";
  std::cin >> dictPath;

  clock_t begin = clock();

  /* Opening the dictionary and creating dictionary hash table */

  dictFile.open(dictPath.c_str());
  if (!dictFile) {
    std::cout << "Error: could not open " << dictPath << std::endl;
    exit(0);
  }
  while (!dictFile.eof()){
    std::getline(dictFile, dictline);
    for (int i = 0; i < dictline.length(); i++){
      dictline[i] = std::tolower(dictline[i]);
    }
    dictionary.insert(dictline);
  }
  dictFile.close();
  clock_t end = clock();
  double elapsed_secs = ((double)(end-begin)) / CLOCKS_PER_SEC;
  std::cout << "Total time (in seconds): " << elapsed_secs << std::endl;

  begin = clock();
  /* Call spell function */
  spell(inputPath,outputPath);
  end = clock();
  elapsed_secs = ((double)(end-begin)) / CLOCKS_PER_SEC;
  std::cout << "Total time (in seconds): " << elapsed_secs << std::endl;
}

//Handles all input output and spell checks
void spell(std::string inputPath, std::string out){

  int linenum = 0; //keeping track of current line #
  //Input File
  std::cout << "Enter name of input file: ";
  std::cin >> inputPath;
  inFile.open(inputPath.c_str());
  if(!inFile) {
    std::cout << "Error: could not open " << inputPath << std::endl;
    exit(0);
  }
  //Output File
  std::cout << "Enter name of output file: ";
  std::cin >> outputPath;
  outFile.open(outputPath);
  if(!outFile) {
    std::cout << "Error: could not open " << outputPath << std::endl;
    exit(0);
  }

  while(!inFile.eof()){
    std::getline(inFile, inputline);
    linenum++;
    parse(inputline);

    for (int i = 0; i < word.size(); i++){
      if(!hasNumbers(word[i])){
        if (word[i].length()<=20 && !dictionary.contains(word[i])) {
          outFile << "Unknown word at line " << linenum << ": " << word[i] << std::endl;
        } else if (word[i].length() > 20){
          outFile << "Long word at line " << linenum << ", starts: " << word[i].substr(0, 20) << std::endl;
        }
      }
    }
    word.clear();
  }
  inFile.close();
  outFile.close();
}

//Parse function
void parse(std::string l){
  for (int i = 0; i < l.length(); i++){
    l[i] = std::tolower(l[i]);
  }
  std::sregex_token_iterator begin(l.begin(), l.end(), re), end;
  std::copy(begin, end, std::back_inserter(word));
}

//Does the word contain a number?
bool hasNumbers(std::string word) {
        for (int i = 0; i < word.length(); i++){
          if(isdigit(word[i]))
                  return true;
        }
        return false;
}
