#ifndef MATH_H
#define MATH_H
#include <iostream>
#include <math.h>
#include "core/ISerializable.h"
#include <cstring>
#include "core/macros.h"

template <class T, int L>
class Vector : ISerializable{
    public:

    Vector(){
        for(int i = 0; i < L; i ++){
            vals[i] = 0;
        }
    }

    template <class O>
    explicit operator Vector<O, L>() {
        O vs[L];

        for(int i = 0; i < L; i++){
            vs[i] = (O)vals[i];
        }

        return Vector<O, L>(vs);
    }

    Vector(T x, T y){
        vals[0] = x;
        vals[1] = y;
    }

    Vector(T x, T y, T z){
        vals[0] = x;
        vals[1] = y;
        vals[2] = z;
    }

    Vector(T vs[]){
        for(int i = 0; i < L; i++){
            vals[i] = vs[i];
        }
    }

    int serialize(char * buffer) const{
        std::memcpy(buffer,vals,sizeof(T) * L);
        return sizeof(T) * L;
    }

    int deserialize(char * buffer){
        std::memcpy(vals,buffer,sizeof(T) * L);
        return sizeof(T) * L;
    }

    float sq_norm(){
        return this->dot(*this);
    }
    float norm(){
        return sqrt(sq_norm());
    }

    void normalize(){
        auto n = norm();
        if(n == 0){
            return;
        }

        for (int i = 0; i < L; i++){
            vals[i] = vals[i] / n;
        }
    }

    void clamp(T min, T max){
        for(int i = 0; i < L; i++){
            vals[i] = std::max(std::min(vals[i],max),min);
        }
    }
    
    float dot(const Vector & o){
        float dot = 0;
        for(int i = 0; i < L; i++){
            dot += vals[i] * o[i];
        }
        return dot;
    }


    uint8_t size(){
        return L;
    }
    
    const T& operator [] (uint8_t i) const { return vals[i];}
    T& operator [] (uint8_t i) { return vals[i]; }

    bool operator==(const Vector& o) const { 
        for(int i =0; i < L;i++){
            if(!APPROX_EQ(vals[i],o[i]))
                return false;
        } 
        return true;
    }

    friend Vector operator * (const T &r, const Vector<T,L> o) {
        T vs[L];

        for(int i = 0; i < L; i++){
            vs[i] = o[i] * r;
        }
        return Vector(vs);
    }

    friend Vector operator * (const Vector<T,L> o, const T &r) {
        return r * o;
    }

    friend Vector operator * (const Vector<T,L> r, const Vector<T,L> o) {
        T vs[L];

        for(int i = 0; i < L; i++){
            vs[i] = o[i] * r[i];
        }
        return Vector(vs);
    }


    friend Vector operator / (const T &r, const Vector<T,L> o) {
        T vs[L];

        for(int i = 0; i < L; i++){
            vs[i] = r / o[i];
        }
        return Vector(vs);
    }

    friend Vector operator / (const Vector<T,L> o, const T &r) {
        T vs[L];

        for(int i = 0; i < L; i++){
            vs[i] = o[i] / r;
        }
        return Vector(vs);    }

    friend Vector operator / (const Vector<T,L> r, const Vector<T,L> o) {
        T vs[L];

        for(int i = 0; i < L; i++){
            vs[i] = r[i] / o[i];
        }
        return Vector(vs);     }

    friend Vector operator + (const T &r, const Vector<T,L> o) {
        T vs[L];

        for(int i = 0; i < L; i++){
            vs[i] = o[i] + r;
        }
        return Vector(vs);
    }
    
    friend Vector operator + (const Vector<T,L> o, const T &r) {
        return r + o;
    }

    friend Vector operator + (const Vector<T,L> r, const Vector<T,L> o) {
        T vs[L];

        for(int i = 0; i < L; i++){
            vs[i] = o[i] + r[i];
        }
        return Vector(vs);
    }

    friend Vector operator - (const Vector<T,L> o) {
        T vs[L];

        for(int i = 0; i < L; i++){
            vs[i] = -o[i];
        }
        return Vector(vs);
    }

    friend Vector operator - (const Vector<T,L> o, const T &r) {
        return o + (-r);
    }
    friend Vector operator - (const T &r, const Vector<T,L> o) {
        return r + (-o);
    }
    friend Vector operator - (const Vector<T,L> r, const Vector<T,L> o) {
        return r + (-o);
    }


    friend std::ostream& operator << (std::ostream &s, const Vector &v){
        s << '(';
        for(int i = 0; i < L - 1; i++){
            s << v[i] << ',';
        }
        s << v[L-1]  << ')';
        
        return s;
    }
    T vals[L];
};

using Vec2 = Vector<float,2>;
using Vec2Int = Vector<int,2>;
using Vec3 = Vector<float,3>;
using Vec3Int = Vector<int,3>;




#endif // MATH_H
