#pragma once
#include <Windows.h>
#include <cstring>

template <typename T>
inline char* Serialize(char packetId, const T& data) {
    char* buffer = new char[sizeof(char) + sizeof(T)];
    memcpy(buffer, &packetId, sizeof(char));
    memcpy(buffer + sizeof(char), &data, sizeof(T));
    return buffer;
}

template <typename T>
inline T Deserialize(char* buffer, int size) {
    char* data = new char[size];
    memcpy(data, buffer + sizeof(char), sizeof(T));
    T* obj = reinterpret_cast<T*>(data);
    T result = *obj;

    delete[] data; 
    return result;
}