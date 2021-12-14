#ifndef MATH_H
#define MATH_H
#include <iostream>
#include <math.h>
#include "core/ISerializable.h"
#include <cstring>

template <class T, int L>
class Vector : ISerializable{
    public:

    Vector(){
        vals = new T[L];
        
        for(int i = 0; i < L; i ++){
            vals[i] = 0;
        }
    }

    Vector(T x, T y){
        vals = new T[L];
        vals[0] = x;
        vals[1] = y;
    }

    Vector(T x, T y, T z){
        vals = new T[L];
        vals[0] = x;
        vals[1] = y;
        vals[2] = z;
    }

    Vector(T* vs){
        vals = new T[L];
        vals = vs;
    }
    
    Vector(const Vector& o){
        vals = new T[L];
        for(int i = 0; i < L; i++){
            vals[i] = o[i];
        }
    }
    
    void swap(Vector& o ) {
        std::swap(vals, o.vals);
    }

    int serialize(char * buffer){
        std::memcpy(buffer,vals,sizeof(T) * L);
        return sizeof(T) * L;
    }

    int deserialize(char * buffer){
        std::memcpy(vals,buffer,sizeof(T) * L);
        return sizeof(T) * L;
    }

    float sq_norm(){
        float out = 0;
        for(int i = 0; i < L; i++){
            out += vals[i] *vals[i];
        }
        return out;
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
    

    Vector& operator=(const Vector& o){
        Vector tmp(o);
        swap(tmp);
        return *this;
    }

    uint8_t size(){
        return L;
    }
    
    const T& operator [] (uint8_t i) const { return vals[i];}
    T& operator [] (uint8_t i) { return vals[i]; }

    friend Vector operator * (const T &r, const Vector<T,L> o) {
        T* vs = new T[L];

        for(int i = 0; i < L; i++){
            vs[i] = o[i] * r;
        }
        return Vector(vs);
    }

    friend Vector operator * (const Vector<T,L> o, const T &r) {
        return r * o;
    }

    friend Vector operator * (const Vector<T,L> r, const Vector<T,L> o) {
        T* vs = new T[L];

        for(int i = 0; i < L; i++){
            vs[i] = o[i] * r[i];
        }
        return Vector(vs);
    }


    friend Vector operator / (const T &r, const Vector<T,L> o) {
        T* vs = new T[L];

        for(int i = 0; i < L; i++){
            vs[i] = r / o[i];
        }
        return Vector(vs);
    }

    friend Vector operator / (const Vector<T,L> o, const T &r) {
        return o * (1/r);
    }

    friend Vector operator / (const Vector<T,L> r, const Vector<T,L> o) {
        return r * (1/o);
    }

    friend Vector operator + (const T &r, const Vector<T,L> o) {
        T* vs = new T[L];

        for(int i = 0; i < L; i++){
            vs[i] = o[i] + r;
        }
        return Vector(vs);
    }
    
    friend Vector operator + (const Vector<T,L> o, const T &r) {
        return r + o;
    }

    friend Vector operator + (const Vector<T,L> r, const Vector<T,L> o) {
        T* vs = new T[L];

        for(int i = 0; i < L; i++){
            vs[i] = o[i] + r[i];
        }
        return Vector(vs);
    }

    friend Vector operator - (const Vector<T,L> o) {
        T* vs = new T[L];

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

    ~Vector(){
        delete[] vals;
    }



    T* vals;
};

using Vec2 = Vector<float,2>;
using Vec2Int = Vector<int,2>;
using Vec3 = Vector<float,3>;
using Vec3Int = Vector<int,3>;




#endif // MATH_H
