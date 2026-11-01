#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <cmath>

template<typename V = std::string>
class HashTable {
private:
    struct KeyValue {
        int key;
        V value;
    };

    size_t current_size;                       
    int hash_func_id;                           
    int c, d;                                  
    double a;                                   
    std::vector<std::list<KeyValue>> table;

    // Хеш-функция для заданного размера (используется при перестройке)
    size_t hash_for_size(int key, size_t size) const {
        size_t h0 = static_cast<size_t>(key) % size;   // K mod N
        switch (hash_func_id) {
        case 1:
            // hi(K) = (h0 + c + d) mod N
            return (h0 + static_cast<size_t>(c) + static_cast<size_t>(d)) % size;
        case 2: {
            // hi(K) = floor( (h0 * a * N) ) mod N, a = (√5-1)/2
            double val = static_cast<double>(h0) * a * static_cast<double>(size);
            size_t h1 = static_cast<size_t>(std::floor(val));
            return h1 % size;
        }
        case 3: {
            // hi(K) = (h0 + 1 + (K mod (N-2))) mod N
            // Защита от N <= 2
            size_t denom = (size > 2) ? (size - 2) : 1;
            size_t part = static_cast<size_t>(key) % denom;
            return (h0 + 1 + part) % size;
        }
        default:
            return h0;   // на всякий случай
        }
    }

    // Хеш-функция с текущим размером таблицы
    size_t hash(int key) const {
        return hash_for_size(key, current_size);
    }

    // Перестроение таблицы (при смене хеш-функции или размера)
    void rehash() {
        std::vector<std::list<KeyValue>> new_table(current_size);
        for (const auto& chain : table) {
            for (const auto& p : chain) {
                size_t idx = hash(p.key);   // используем новую хеш-функцию
                new_table[idx].push_front(p);
            }
        }
        table.swap(new_table);               // замена std::move на swap
    }

public:
    // Конструктор с заданным размером и номером студента (по умолчанию 5)
    explicit HashTable(size_t size = 16, int student_id = 5)
        : current_size(size),
        hash_func_id(1),
        c(student_id % 5),
        d(student_id % 7),
        a((std::sqrt(5.0) - 1.0) / 2.0),
        table(size) {}

    // Конструктор копирования
    HashTable(const HashTable& other)
        : current_size(other.current_size),
        hash_func_id(other.hash_func_id),
        c(other.c),
        d(other.d),
        a(other.a),
        table(other.table) {}   // глубокое копирование

    // Деструктор не требуется – vector и list сами освобождают память
    ~HashTable() = default;

    // Оператор присваивания (copy-and-swap)
    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            HashTable temp(other);
            swap(temp);
        }
        return *this;
    }

    // Обмен содержимым с другой таблицей (без использования std::swap)
    void swap(HashTable& other) noexcept {
        // ручной обмен примитивных полей
        size_t tmp_size = current_size;
        current_size = other.current_size;
        other.current_size = tmp_size;

        int tmp_id = hash_func_id;
        hash_func_id = other.hash_func_id;
        other.hash_func_id = tmp_id;

        int tmp_c = c;
        c = other.c;
        other.c = tmp_c;

        int tmp_d = d;
        d = other.d;
        other.d = tmp_d;

        double tmp_a = a;
        a = other.a;
        other.a = tmp_a;

        // обмен векторов через метод swap
        table.swap(other.table);
    }

    // Добавление или замена элемента
    void insert(int key, const V& value) {
        size_t idx = hash(key);
        auto& chain = table[idx];
        for (auto& p : chain) {
            if (p.key == key) {
                p.value = value;   // ключ уже есть – обновляем значение
                return;
            }
        }
        chain.push_front(KeyValue{ key, value });
    }

    // Удаление элемента по ключу
    bool remove(int key) {
        size_t idx = hash(key);
        auto& chain = table[idx];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->key == key) {
                chain.erase(it);   // прямое удаление (list::erase)
                return true;
            }
        }
        return false;
    }

    // Проверка наличия ключа
    bool contains(int key) const {
        size_t idx = hash(key);
        for (const auto& p : table[idx]) {
            if (p.key == key) return true;
        }
        return false;
    }

    // Доступ по ключу (чтение и запись) - создает элемент, если не существует
    V& operator[](int key) {
        size_t idx = hash(key);
        auto& chain = table[idx];
        for (auto& p : chain) {
            if (p.key == key) {
                return p.value;
            }
        }
        // Если ключа нет – создаём новый элемент со значением по умолчанию
        chain.push_front(KeyValue{ key, V() });
        return chain.front().value;
    }

    // Вывод содержимого в консоль
    void printToConsole() const {
        for (size_t i = 0; i < table.size(); ++i) {
            std::cout << i << ": ";
            for (const auto& p : table[i]) {
                std::cout << "(" << p.key << ", " << p.value << ") ";
            }
            std::cout << std::endl;
        }
    }

    // Смена хеш-функции (1, 2 или 3)
    void setHashFunction(int id) {
        if (id < 1 || id > 3) return;
        if (id == hash_func_id) return;
        hash_func_id = id;
        rehash();
    }

    // Изменение размера таблицы
    void resize(size_t new_size) {
        if (new_size == current_size) return;
        std::vector<std::list<KeyValue>> new_table(new_size);
        for (const auto& chain : table) {
            for (const auto& p : chain) {
                size_t idx = hash_for_size(p.key, new_size); // хеш с новым размером
                new_table[idx].push_front(p);
            }
        }
        table.swap(new_table);
        current_size = new_size;
    }
};




