#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>

#include "Movie.h"
#include "Table.h"

using namespace std;

typedef unsigned long QUERY;  // all queries will be hashed to an unsigned long

Table::Table(int sz, float occ) : max_size(sz), occupancy(occ), size_(0) {
  // just initialize all of the query values to something the hash function cant
  // return
  table.resize(sz);
  for (int i = 0; i < sz; ++i) {
    table[i].first = 0;
  }
}

Table* Table::copyTable(const Table& tb) {
  size_ = tb.size_;
  max_size = tb.max_size;
  occupancy = tb.occupancy;
  table.resize(max_size);
  // all of the types used to store data are stl types which support deep
  // copying in their assignment operator/copy constructor, so theoretically this
  // line just works
  table = tb.table;
  return this;
}

unsigned long Table::DJB2Hash(const string& str) const {
  // adapted from http://www.cse.yorku.ca/~oz/hash.html
  unsigned long hash = 5381;
  for (char c : str) {
    hash = (hash << 5) + hash + c; /* hash * 33 + c */
  }
  return hash;
}

void Table::insert(MoviePtr movie, const vector<int>& filter) {
  string query_str = "";
  // a zero from the filter will be interpreted as missing information
  // while a one is seen as known info, so it will be added to the query string
  // append all "known" information to query_str, then hash query_str
  assert(filter.size() == 6);
  for (int i = 0; i < filter.size(); ++i) {
    if (filter[i] == 1) {
      for (int j = 0; j < movie->m_data[i].size(); ++j) {
        query_str += movie->m_data[i][j];
      }
    }
  }

  // q is the actual query after being hashed
  // index is where in the main vector it will be placed
  QUERY q = DJB2Hash(query_str);
  int index = q % max_size;
  int count = 0;

  // check the table for that query value, and see if it matches the movie's
  // information
  while (count <= max_size) {
    // we do a bit of circling back to the beginning if necessary
    if (index == max_size) index = 0;
    // if that spot in the table is unoccupied
    if (table[index].first == 0) {  // im sort of just assuming that the hsah
                                    // cant output 0, im screwed if it does
      table[index].first = q;
      table[index].second.push_back(movie);
      ++size_;
      break;
    } else if (table[index].first != q) {  // if the queries dont match
      ++index;
      continue;
    } else if (table[index].first == q) {
      table[index].second.push_back(movie);
      ++size_;
      break;
    }
  }

  // because for some reason i need to print out every movie in the case of no
  // info
  if (sum_vec(filter) == 6) full_list.push_back(movie);

  // calculate occupancy and determine if resizing/rehashing is necessary
  int tmp_occ = size_ / max_size;
  if (tmp_occ > occupancy) resize(max_size * 2);
}

void Table::resize(int new_size) {
  Table new_table(new_size, occupancy);
  list<MoviePtr> movies;
  /*g o over the entire table, and if a movie we havent seen before shows up,
  add it to the new table and add it to the list of movies we have seen. */
  for (pair<QUERY, list<MoviePtr> > p : table) {
    if (p.first != 0) {
      for (MoviePtr m : p.second) {
        if (find(movies.begin(), movies.end(), m) == movies.end()) {
          movies.push_back(m);
          new_table.permute_filters_to_insert(m);
        }
      }
    }
  }
  max_size = new_size;
  copyTable(new_table);
}

void Table::printQuery(const Movie& movie, const vector<int>& filter) {
  string query_str = "";
  // a zero from the filter will be interpreted as missing information
  // while a one is seen as known info, so it will be added to the query string
  // append all "known" information to query_str, then hash query_str
  assert(filter.size() == 6);
  for (int i = 0; i < filter.size(); ++i) {
    if (filter[i] == 1) {
      for (int j = 0; j < movie.m_data[i].size(); ++j) {
        query_str += movie.m_data[i][j];
      }
    }
  }

  // q is the actual query after being hashed
  // index is where in the main vector it will be placed
  QUERY q = DJB2Hash(query_str);
  int index = q % max_size;
  list<MoviePtr> movies;

  int sum = sum_vec(filter);

  if (sum == 0) {
    movies = movieSearch(q, index, true);
    movies.pop_back();  // repair a consistently damaged list structure
  } else
    movies = movieSearch(q, index, false);

  printMovies(movies);
}

const list<MoviePtr>& Table::movieSearch(QUERY q, int index, bool full) const {
  if (full) return full_list;
  int count = 0;
  while (count <= max_size) {
    // circling back to the beginning if necessary
    if (index == max_size) index = 0;
    if (table[index].first == 0)
      ++index;
    else if (table[index].first != q)
      ++index;
    else
      return table[index].second;
    ++count;
  }
  return empty_list;
}

void Table::permute_filters(int pos, std::vector<int> filter, MoviePtr movie) {
  if (pos == 6) {
    insert(movie, filter);
    return;
  }

  filter.push_back(0);
  std::vector<int> filter_new = filter;
  filter_new.back() = 1;
  permute_filters(pos + 1, filter_new, movie);
  permute_filters(pos + 1, filter, movie);
}

void Table::permute_filters_to_insert(MoviePtr movie) {
  permute_filters(0, std::vector<int>(), movie);
}

// just a cute little sum function
int Table::sum_vec(const vector<int>& vec) const {
  int sum = 0;
  for (int i : vec) {
    sum += i;
  }
  return sum;
}

void Table::printMovies(const list<MoviePtr>& movies) {
  if (movies.size() == 0) {
    cout << "No results for query.\n";
    return;
  }

  std::cout << "Printing " << movies.size() << " result(s):\n";
  for (MoviePtr m : movies) {
    cout << m->m_data[0][0] << endl
         << m->m_data[1][0] << endl
         << m->m_data[2][0] << endl;
    cout << m->m_data[3].size();
    for (string str : m->m_data[3]) {
      cout << ' ' + str;
    }
    cout << endl;
    cout << m->m_data[4].size();
    for (int i = 0; i < m->m_data[4].size(); ++i) {
      cout << ' ' + actors[m->m_data[4][i]] + " (" + m->m_data[5][i] + ")";
    }
    cout << endl;
  }
}