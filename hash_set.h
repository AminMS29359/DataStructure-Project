#pragma once
#pragma once
#include "hash.h"
template<typename K>
class hash_set {
private:
    struct set_node {
        K key;
        set_node* next;
        set_node(const K& k) : key(k),next(nullptr) {}
    };

    set_node** table;
    size_t bucket_count;
    size_t size;
    double max_load_factor;
    hash<K> hash_function;
    size_t prime_index;

    static const size_t primes[];


    size_t key_index(const K& key) {
        return hash_function(key) % bucket_count;
    }

    void rehash() {
        size_t new_bucket_count = primes[++prime_index];
        set_node** new_table = new set_node * [new_bucket_count];

        for (size_t i = 0; i < new_bucket_count; i++)
            new_table[i] = nullptr;

        for (size_t i = 0; i < bucket_count; i++) {
            set_node* curr = table[i];
            while (curr) {
                set_node* next = curr->next;
                size_t new_table_index = hash_function(curr->key) % new_bucket_count;

                curr->next = new_table[new_table_index];
                new_table[new_table_index] = curr;

                curr = next;
            }
        }

        delete[] table;
        table = new_table;
        bucket_count = new_bucket_count;
    }

public:
    class iterator {
    private:
        set_node** table;
        size_t bucket;
        size_t bucket_count;
        set_node* current;

    public:
        iterator(set_node** ta, size_t bu, size_t bc, set_node* cu)
            : table(ta), bucket(bu), bucket_count(bc), current(cu) {
        }

        bool operator==(const iterator& other) const {
            return current == other.current && table == other.table;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        const K& operator*() const {
            if (current == nullptr)
                throw std::runtime_error("invalid iterator dereference");
            return current->key;
        }

        iterator& operator++() {
            if (current->next) {
                current = current->next;
                return *this;
            }

            bucket++;
            while (bucket < bucket_count && table[bucket] == nullptr)
                bucket++;

            if (bucket < bucket_count)
                current = table[bucket];
            else
                current = nullptr;
            return *this;
        }
    };

    hash_set(size_t _buckets = 11, double _load_factor = 0.8)
        : bucket_count(_buckets),
        size(0),
        max_load_factor(_load_factor) ,
        prime_index(0)
    {

        table = new set_node * [bucket_count];
        for (size_t i = 0; i < bucket_count; i++)
            table[i] = nullptr;
    }

    hash_set(const hash_set& other)
    {
        bucket_count = other.bucket_count;
        hash_function = other.hash_function;
        max_load_factor = other.max_load_factor;
        prime_index = other.prime_index;
        size = other.size;

        table = new set_node * [bucket_count];

        for (size_t i = 0; i < bucket_count; ++i)
        {
            if (other.table[i] != nullptr)
            {
                set_node* curr = other.table[i];
                set_node* head = new set_node(curr->key);
                table[i] = head;
                curr = curr->next;
                while (curr)
                {
                    set_node* new_node = new set_node(curr->key);
                    head->next = new_node;
                    curr = curr->next;
                    head = new_node;
                }
            }
            else table[i] = nullptr;

        }
    }

    ~hash_set() {
        for (size_t i = 0; i < bucket_count; i++) {
            set_node* curr = table[i];
            while (curr) {
                set_node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
        delete[] table;
    }

    double loadFactor() {
        return static_cast<double>(size) / bucket_count;
    }

    void insert(const K& key) {
        size_t index = key_index(key);
        set_node* curr = table[index];

        while (curr) {
            if (curr->key == key) {
                return;
            }
            curr = curr->next;
        }

        set_node* node = new set_node(key);
        node->next = table[index];
        table[index] = node;
        ++size;

        if (loadFactor() > max_load_factor)
            rehash();
    }

    iterator find(const K& key) {
        size_t index = key_index(key);
        set_node* curr = table[index];

        while (curr) {
            if (curr->key == key) {
                return iterator(table , index ,bucket_count ,curr);
            }
            curr = curr->next;
        }
        return iterator(table, bucket_count, bucket_count, nullptr);
    }

    void erase(const K& key) {
        size_t index = key_index(key);
        set_node* curr = table[index];
        set_node* prev = nullptr;

        while (curr) {
            if (curr->key == key) {
                if (prev)
                    prev->next = curr->next;
                else
                    table[index] = curr->next;

                delete curr;
                size--;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        return;
    }

    hash_set<K>& operator=(const hash_set& other)
    {
        if (this == &other) return *this;

        for (int i = 0; i < bucket_count; ++i)
        {
            set_node* curr = table[i];
            set_node* prev =nullptr;
            while (curr)
            {
                prev = curr;
                curr = curr->next;
                delete prev;
            }
        }
        delete[]table;

        bucket_count = other.bucket_count;
        hash_function = other.hash_function;
        max_load_factor = other.max_load_factor;
        prime_index = other.prime_index;
        size = other.size;

        table = new set_node * [bucket_count];

        for (size_t i = 0; i < bucket_count; ++i)
        {
            if (other.table[i] != nullptr)
            {
                set_node* curr = other.table[i];
                set_node* head = new set_node(curr->key);
                table[i] = head;
                curr = curr->next;
                while (curr)
                {
                    set_node* new_node = new set_node(curr->key);
                    head->next = new_node;
                    curr = curr->next;
                    head = new_node;
                }
            }
            else table[i] = nullptr;

        }
        return(*this);
    }

    size_t getsize() {
        return size;
    }

    iterator begin() {
        for (size_t i = 0; i < bucket_count; i++)
            if (table[i])
                return iterator(table, i, bucket_count, table[i]);
        return end();
    }

    iterator end() {
        return iterator(table, bucket_count, bucket_count, nullptr);
    }
};

template<typename K>

const size_t hash_set<typename K>:: primes[] = {
  11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421,
  12853, 25717, 51437, 102877, 205759, 411527, 823117,
  1646237, 3292489, 6584983, 13169977, 26339969, 52679969,
  105359939, 210719881, 421439783, 842879579, 1685759167
};