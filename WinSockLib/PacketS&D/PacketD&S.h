#pragma once
#include <Windows.h>
#include <cstring>

// Used to serialize structs into packets and slot a packet ID in the front
template <typename T>
inline char* Serialize(char packetId, const T& data) {
    char* buffer = new char[sizeof(char) + sizeof(T)];
    memcpy(buffer, &packetId, sizeof(char));
    memcpy(buffer + sizeof(char), &data, sizeof(T));
    return buffer;
}

// This is a function created to deserialize structs from packets and extract the packet ID
template <typename T>
inline T Deserialize(char* buffer, int size, char& Packid) {
    char* data = new char[size];
    Packid = buffer[0];
    memcpy(data, buffer + sizeof(char), sizeof(T));
    T* obj = reinterpret_cast<T*>(data);
    T result = *obj;

    delete[] data; 
    return result;
}
