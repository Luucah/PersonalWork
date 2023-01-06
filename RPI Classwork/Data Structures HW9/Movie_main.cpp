#include <fstream>
#include <iostream>

#include "Movie.h"
using namespace std;

Movie::Movie(ifstream& fs) {
  string title, year, runtime;
  fs >> title >> year >> runtime;

  // builds the movie itself
  vector<string> tmp_vec;
  tmp_vec.push_back(title);
  m_data.push_back(tmp_vec);  // title added
  tmp_vec.clear();
  tmp_vec.push_back(year);
  m_data.push_back(tmp_vec);  // year added
  tmp_vec.clear();
  tmp_vec.push_back(runtime);
  m_data.push_back(tmp_vec);  // runtime added
  tmp_vec.clear();

  // the ugly part is (probably) over
  int num;
  for (int i = 0; i < 3; ++i) {
    fs >> num;
    if (num != 0) {
      for (int j = 0; j < num; ++j) {
        string tmp_str;
        fs >> tmp_str;
        tmp_vec.push_back(tmp_str);
      }
      m_data.push_back(tmp_vec);
      tmp_vec.clear();
    }
  }
}

Movie::Movie(istream& ci, vector<int>& filter) {
  string title, year, runtime;
  ci >> title >> year >> runtime;

  // builds a filter for the movie.
  if (title == "?")
    filter.push_back(0);
  else
    filter.push_back(1);
  if (year == "?")
    filter.push_back(0);
  else
    filter.push_back(1);
  if (runtime == "?")
    filter.push_back(0);
  else
    filter.push_back(1);

  // builds the movie itself
  vector<string> tmp_vec;
  tmp_vec.push_back(title);
  m_data.push_back(tmp_vec);  // title added
  tmp_vec.clear();
  tmp_vec.push_back(year);
  m_data.push_back(tmp_vec);  // year added
  tmp_vec.clear();
  tmp_vec.push_back(runtime);
  m_data.push_back(tmp_vec);  // runtime added
  tmp_vec.clear();

  int num;
  for (int i = 0; i < 3; ++i) {
    ci >> num;
    if (num == 0)
      filter.push_back(0);
    else {
      filter.push_back(1);
      for (int j = 0; j < num; ++j) {
        string tmp_str;
        ci >> tmp_str;
        tmp_vec.push_back(tmp_str);
      }
    }
    m_data.push_back(tmp_vec);
    tmp_vec.clear();
  }
}

Movie::~Movie() { m_data.clear(); }