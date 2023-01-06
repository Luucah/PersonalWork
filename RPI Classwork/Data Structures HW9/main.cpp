#include <fstream>
#include <iostream>
#include <map>

#include "Movie.h"
#include "Table.h"

using namespace std;

int main() {
  int init_size = 100;
  float init_occ = .5;
  string command;
  Table hash_table(init_size, init_occ);
  while (cin >> command) {
    if (command == "table_size") {  // this command is pretty simple
      cin >> init_size;
      hash_table.setMaxSize(init_size);
    }

    else if (command == "occupancy") {  // same here
      cin >> init_occ;
      hash_table.setOccupancy(init_occ);
    }

    else if (command == "movies") {  // this is the one that makes the table
      string in_file;
      cin >> in_file;
      ifstream in_str(in_file);

      if (!in_str.good()) {
        std::cerr << "Can't open " << in_file << " to read.\n";
        exit(1);
      }
      vector<int> tmp_filter;
      for (int i = 0; i < 6; ++i) tmp_filter.push_back(1);

      while (!in_str.eof()) {
        Movie* tmp_movie = new Movie(in_str);
        MoviePtr tmp_movie_ptr =
            MoviePtr(tmp_movie);  // constructor does the input work
        hash_table.permute_filters_to_insert(tmp_movie_ptr);
      }
    }

    else if (command == "actors") {
      string in_file;
      cin >> in_file;
      ifstream in_str(in_file);
      if (!in_str.good()) {
        std::cerr << "Can't open " << in_file << " to read.\n";
        exit(1);
      }
      map<string, string> actors;
      string id, name;
      while (in_str >> id >> name) {
        actors[id] = name;
      }
      hash_table.setActors(actors);
    }

    else if (command == "query") {
      vector<int> tmp_filter;
      Movie tmp_movie(cin, tmp_filter);

      hash_table.printQuery(tmp_movie, tmp_filter);
    } else
      return 0;
  }
  return 0;
}