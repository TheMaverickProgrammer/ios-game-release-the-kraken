/*
 *  Sequence.h
 *  Jiwon
 *
 *  Created by Brian M. Peppers on 6/10/2011.
 *  Copyright 2011 Brian M. Peppers. All rights reserved.
 *
 */

#include "Requirements.h"
#include <deque.h>

namespace Jiwon
{
    class Sequence
    {
    private:
        std::deque<boost::function<bool ()> > eventFunctions;
        
    public:
        Sequence();
        ~Sequence();
        
        void addEventFunction(boost::function<bool ()>);
        
        void update();
        
        Sequence &operator=(const Sequence &rhs);
    };
}
