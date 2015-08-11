#ifndef FACTORY_H
#define FACTORY_H

template<typename RETURN_TYPE, typename DATA>
class Factory
{
    virtual RETURN_TYPE *create(DATA &data);
};

#endif