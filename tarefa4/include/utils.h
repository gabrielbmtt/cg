#pragma once

#include <iostream>
#include <string>

template <typename T>
class SmartPtr {
    public:
        T* pointer = nullptr;

        SmartPtr (T* pointer);
        
        T& operator * ();
        T* operator -> ();

        SmartPtr<T> operator = (SmartPtr<T>& ptr);

        // 'destrutor' para liberar memoria do ponteiro quando o escopo acaba
        ~SmartPtr ();
};

template <typename T>
using Sp = SmartPtr<T>;