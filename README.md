# WinSockLib


This is a header only Winsock class using inline functions, I only did this so I wouldn't have to compile a library very often.

It has a packet handle callback system. It has a simple method to deserialize and serialize packets with packet ids. Below is an example of a TCP Server using such headers.

![image](https://github.com/Matthew-C-2022/WinSock-Library/assets/112186489/95c19651-3621-4611-9f59-f096bc8bfdc9)



In the class I decided to use a template and std::functions to create a packet handler callback.

![image](https://github.com/Matthew-C-2022/WinSock-Library/assets/112186489/65e960f4-4466-45e6-adc1-954327799711)


the callback isn't dynamic and requires 3 arguments char* buf, int clientID, and int packlen.

Shown below is an example I've made using the header only library.

![image](https://github.com/Matthew-C-2022/WinSock-Library/assets/112186489/19cd5d05-f60e-4e69-aeef-b2da4b8e386d)


it shows that the sendallexcluding function works, Deserialisation works, PacketHandleCallback works, and serialization works.

![image](https://github.com/Matthew-C-2022/WinSock-Library/assets/112186489/adc1b50c-acc5-4536-81cd-6842ae4df699)
