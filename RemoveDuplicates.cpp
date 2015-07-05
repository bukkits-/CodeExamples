#include <stdlib.h>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define OUTPATH "IP_Strings.txt"
using namespace std;

int main(int argc, char* argv[])
{
  vector<string> IpList;
  string line;
  ifstream in;
  ofstream out;
  in.open(argv[1]);
  out.open(argv[2]);

  if(in.is_open() && out.is_open())
  {
    while(getline(in, line))
    {
      IpList.push_back(line);
      cout << "Reading: " << line << endl;
    }
    if(IpList.size() >= 1)
    {
      sort(IpList.begin(), IpList.end());
      IpList.erase(unique(IpList.begin(), IpList.end()), IpList.end());
      for(int i; i < IpList.size(); i++)
      {
        cout << "Writing : " << IpList[i] << endl;
        out << IpList[i] << endl;
      }
    }

  }
  in.close();
  out.close();
  return 0;
}
