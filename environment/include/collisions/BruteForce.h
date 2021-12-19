#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H
#include "core/ICollidable.h"
#include "core/ICollisionChecker.h"
#include <vector>
#include <algorithm>

class BruteForce : public ICollisionChecker{

    public:
    
    ~BruteForce(){
        objects.clear();
    };
    void insert(ICollidable &c){
        objects.push_back(&c);
    };

    void remove(ICollidable &c){
        objects.erase(std::find(objects.begin(), objects.end(),&c));
    };

    void rebuild(){

    };

    std::vector<Hit> hits(){
        std::vector<Hit> h;
        
        Hit a;
        for(int i = 0; i < objects.size(); i++){
            for(int j = i + 1; j < objects.size();j++){

                if((objects[i]->get_ID().entity < 0) || (objects[j]->get_ID().entity < 0)){
                    continue;
                }
                if(objects[i]->collides(*objects[j],a)){
                    h.push_back(Hit(a));
                }
            }
        }

        return h;
    };

    std::vector<ICollidable*> objects = std::vector<ICollidable*>();
};

#endif //BRUTE_FORCE_H