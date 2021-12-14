#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

class ISerializable{
    public:
    virtual int serialize(char * buffer) = 0;
    virtual int deserialize(char * buffer) = 0;
};


#endif // ISERIALIZABLE_H