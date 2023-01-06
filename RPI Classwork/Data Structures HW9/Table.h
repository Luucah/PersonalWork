#ifndef __Table_
#define __Table_

#include "Movie.h"
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

class Movie;
class MoviePtr;

typedef unsigned long QUERY; //all queries will be hashed to an unsigned long

class Table{
    public:
    //constructors/destructor
    Table(int sz = 100, float occ = .5);
    Table(const Table& tb) {copyTable(tb);}
    Table* operator=(const Table& tb) {return copyTable(tb);}

    //Hash function :)
    unsigned long DJB2Hash(const string& key) const;

    //other stuff
    void insert(MoviePtr movie,const vector<int>& filter);
    void resize(int new_size);
    void printQuery(const Movie& movie, const vector<int>& filter);
    const list<MoviePtr>& movieSearch(QUERY q, int index, bool full) const;
    void permute_filters_to_insert(MoviePtr movie);
    void permute_filters(int pos, std::vector<int> filter, MoviePtr movie);
    void insertActor(const string& id,const string& name) {actors[id] = name;}
    void setMaxSize(int sz) {max_size = sz; resize(max_size);}
    void setOccupancy(int occ) {occupancy = occ;}
    void setActors(map<string, string>& act) {actors = act;}
    int sum_vec(const vector<int>& vec) const;
    void printMovies(const list<MoviePtr>& movies);

    private:
    //private member functions
    void destroyTable();
    Table* copyTable(const Table& tb);

    //representation
    float occupancy;
    int size_, max_size;
    vector<pair<QUERY, list<MoviePtr> > > table;
    list<MoviePtr> empty_list; //exists just to be returned in the case of an invalid search
    list<MoviePtr> full_list; //this is going to be every movie, I will just only use it for printing everything
    map<string, string> actors;
};
#endif