#ifndef IPRINTABLE_H
#define IPRINTABLE_H
#include <iostream>

class IPrintable{

    public:
    
    virtual std::ostream& print(std::ostream& s) const = 0;

};


#endif //IPRINTABLE_H