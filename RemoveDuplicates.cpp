#include <stdlib.h>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sqlite3.h>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{
//IP in/out txt file data
  vector<string> IpList;
  vector<string> SplitList;
  string line;
  ifstream in;
  ofstream out;

//database data
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  string sql;
  sqlite3_stmt *stmt;

//open db and txt files
  rc = sqlite3_open("aether.db", &db);
  in.open(argv[1]);
  out.open(argv[2]);

//Quick check to make sure that the database is open
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  }
  else
  {
    fprintf(stderr, "Opened database successfully\n");
  }

  if(in.is_open() && out.is_open())
  {
    cout << "Files opened successfully\n";
    while(getline(in, line))
    {
      IpList.push_back(line);
      cout << "Reading: " << line << endl;
    }
    if(IpList.size() > 0)
    {
      //Sort the list of IPs then remove duplicates with unique() and erase()
      sort(IpList.begin(), IpList.end());
      IpList.erase(unique(IpList.begin(), IpList.end()), IpList.end());

      cout << IpList.size() << " unique items." << endl;
      for(int i = 0; i < IpList.size(); i++)
      {
        SplitList.erase(SplitList.begin(), SplitList.end());
        cout << "Writing : " << IpList[i] << endl;
        sql = "INSERT or REPLACE INTO nodes(LastConnectedIP, LastConnectedPort, LastConnectedDate, LastSyncTimestamp) VALUES(?, ?, ?, ?)";
        split(SplitList , IpList[i], is_any_of(":"), token_compress_on);

        //cout << "Split got: "<< SplitList[0] << ":" << SplitList[1] << endl;
        if(sqlite3_prepare_v2(db, sql.c_str() , -1, &stmt, NULL) != SQLITE_OK)
        {
          cout << "  [ERROR] Sqlite prepare failure";
          return 1;
        }
        else{
          string Time = "2015-07-05 00:45:01.877416";
          sqlite3_bind_text(stmt, 1, (SplitList[0].c_str()), 16, NULL);
          sqlite3_bind_text(stmt, 2, (SplitList[1].c_str()), sizeof(SplitList[1]), NULL);
          sqlite3_bind_text(stmt, 3, Time.c_str(), 32, NULL);
          sqlite3_bind_text(stmt, 4, Time.c_str(), 32, NULL);
          while(sqlite3_step(stmt) == SQLITE_BUSY);
          cout << "Writing to DB: "<< SplitList[0] << ":" << SplitList[1] << endl;
        }
        out << "> " << IpList[i] << endl << endl;
      }
    }
    else
    {
      cout << ".txt files could not be opened\n";
      return 1;
    }

  }

//close files
  cout << "Closing files \n";
  in.close();
  out.close();
  sqlite3_close(db);
  return 0;
}
