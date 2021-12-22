#ifndef PACKET_BUFFER_H
#define PACKET_BUFFER_H
#include <set>
#include <algorithm>

template <class T>
class PacketBuffer{

    public:

    struct Container{

        Container(T msg, unsigned int seq_number) : msg(msg),seq_number(seq_number){

        };

        friend bool operator<(const Container& l, const Container& r){
            return l.seq_number < r.seq_number;
        }

        T msg;
        unsigned int seq_number;

    };

    PacketBuffer(){

    }

    PacketBuffer(int max_size) : max_size(max_size){

    }


    void enqueue(T m, unsigned int seq_number){
        messages.emplace(m,seq_number);

        if(max_size && messages.size() > max_size){
            messages.erase(messages.begin());
        }
    }
    
    T pop(){
        assert(messages.size() > 0);
        
        auto gs = T(messages.begin()->msg);
        messages.erase(messages.begin());
        return gs;
    }

    void erase_before(unsigned int seq_number){
        
        auto it = messages.begin();
        while(messages.size() > 0 && it != messages.end()){

            if(it->seq_number < seq_number){
                it++;
            } else {
                messages.erase(messages.begin(),it);
                return;
            }
        }
    }

    typename std::multiset<Container>::iterator iterator(){
        assert(messages.size() > 0);

        return messages.begin();
    }

    typename std::multiset<Container>::iterator end(){
        assert(messages.size() > 0);
        
        return messages.end();
    }
    
    int size(){
        return messages.size();
    }

    std::multiset<Container> messages;
    int max_size = -1;
};

#endif // PACKET_BUFFER_H