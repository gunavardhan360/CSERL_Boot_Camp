#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

long long int fib(int n) {
    if (n <= 1)
        return n;
    return fib(n-1) + fib(n-2);
}

vector<string> split(const string &s, char delim) {
  vector<string> elems;

  stringstream ss(s);
  string item;

  while (getline(ss, item, delim)) {
    if (!item.empty())
      elems.push_back(item);
  }

  return elems;
}

string findnreplace(string body, string find, string replace){
    string newbody = "";
    vector<string> lines = split(body, '\n'), words;
    for(int i=0; i < lines.size(); i++){
        words = split(lines[i], ' ');
        for(int j =0; j < words.size(); j++){
            if(words[j].compare(find) == 0)
                newbody += replace + " ";
            else
                newbody += words[j] + " ";
        }
        newbody += "\n";
    }
    return newbody;
}

string nameparser(string body, string name){
    return findnreplace(body, "{{name}}", name);
}

string fibparser(string body, int n){
    string temp = findnreplace(body, "{{fib}}", to_string(fib(n)));
    return findnreplace(temp, "{{n}}", to_string(n));
}