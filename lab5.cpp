#include "lab5.h"
#include <iostream>

int main() 
{
    // Создаём таблицу с размером 5, номер студента = 5 (параметры c=0, d=5)
    HashTable<std::string> ht(5, 5);

    std::cout << "Add elements" << std::endl;
    ht.insert(42, "Hello");
    ht.insert(7, "World");
    ht[100] = "Foo";   // через operator[]
    ht[7] = "Updated"; // замена
    ht.printToConsole();

    std::cout << "\nContains?" << std::endl;
    std::cout << "Contains 42? " << ht.contains(42) << std::endl;
    std::cout << "Contains 99? " << ht.contains(99) << std::endl;

    std::cout << "\n delete key 7" << std::endl;
    ht.remove(7);
    ht.printToConsole();

    std::cout << "\nChange hash function on 2" << std::endl;
    ht.setHashFunction(2);
    ht.printToConsole();

    std::cout << "\nChange size 10" << std::endl;
    ht.resize(10);
    ht.printToConsole();

    std::cout << "\ncopy and =" << std::endl;
    HashTable<std::string> ht2 = ht;           // копирование
    HashTable<std::string> ht3;
    ht3 = ht;                                   // присваивание
    std::cout << "ht2:" << std::endl;
    ht2.printToConsole();

    std::cout << "\n\Swap" << std::endl;
    HashTable<std::string> empty(3);
    empty.swap(ht2);
    std::cout << "After swap with void:" << std::endl;
    empty.printToConsole();

    return 0;
}