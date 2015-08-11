#ifndef GROUP_H
#define GROUP_H

#include <vector>

template<typename T>
class Group
{
private:
    std::vector<T> collection;
    
public:
    
    Group()  { ; }
    ~Group() { ; }
    
    void clear()   { collection.clear(); }
    
    void add(T t) { collection.push_back(t); }
    
    void remove(int index) { collection.erase(collection.begin() + index); }
    
    T    &get(int index)   { return collection[index]; } 
    
    int getSize()          { return (int)collection.size(); }
};

#endif
