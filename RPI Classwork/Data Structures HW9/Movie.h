#ifndef __Movie_
#define __Movie_

#include "Table.h"
#include <fstream>
#include <vector>
#include <string>
#include<iostream>

class Movie{
    public:
    Movie(std::ifstream& fs); //regular constructor
    Movie(std::istream& ci, std::vector<int>& filter);//filter constructor lmao
    Movie(std::vector<std::vector<std::string> > new_movie) : m_data(new_movie) {}
    ~Movie();

    //I know the order my info is pushed in, so its not a big deal 
    //that push_back is implemented like this
    void push_back(const std::vector<std::string>& vec) {m_data.push_back(vec);}
    friend class Table;
    friend class MoviePtr;
    private:
    std::vector<std::vector<std::string> > m_data;
};

class MoviePtr {
    public: 
        MoviePtr(Movie* _m) {
            m = _m;
            count = new int;
            *count = 1;
        }
        MoviePtr(const MoviePtr& o) {
            m = o.m;
            count = o.count;
            (*count)++;
        }
        ~MoviePtr() {
            (*count)--;
            if (*count == 0) delete m;
        }
        Movie* operator->() { return m; }
        bool operator==(const MoviePtr& rhs) const {
            return (m->m_data == rhs.m->m_data && *count == *rhs.count);
        }
    private:
        Movie* m;
        int * count;
};

#endif