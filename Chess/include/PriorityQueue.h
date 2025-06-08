//
// Created by Erez on 29/04/2025.
//

#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <list>
#include "PriorityQueueException.h"
#include <ostream>
#include "mutex"

// Primary template for non-pointer types
template<typename T>
struct MyComparator {
    bool operator()(const T& a, const T& b) const {
        return a < b;
    }
};


// Partial specialization for pointer types
template<typename T>
struct MyComparator<T*> {
    bool operator()(T* const& a, T* const& b) const {
        return *a < *b;  // Dereference for pointer types
    }
};


template <typename T>
class PriorityQueue {

    std::list<T> priority_queue;
    MyComparator<T> comp;
    mutable mutex pqMutex;

public:
    PriorityQueue() : priority_queue() {}

    void push(const T& item) {
        lock_guard lock(pqMutex);
        auto it = priority_queue.begin();
        while (it != priority_queue.end() && comp(*it, item)) {
            ++it;
        }
        priority_queue.insert(it, item);

        if (priority_queue.size() > 5) {
            priority_queue.pop_front();
        }
    }

    T pull() {
        lock_guard lock(pqMutex);
        if (priority_queue.empty()) {
            throw PriorityQueueException("The priority queue is empty");
        }
        T back = priority_queue.back();
        priority_queue.pop_back();
        return back;
    }

    friend ostream& operator<<(ostream& os, const PriorityQueue& pq) {
        if (pq.empty()) {
            throw PriorityQueueException("The priority queue is empty");
        }
        int count = 0;
        for (auto it = pq.priority_queue.rbegin(); it != pq.priority_queue.rend() && count < 3; ++it, ++count) {
            os << *it << std::endl;
        }
        return os;
    }

    int size() const {
        return priority_queue.size();
    }

    bool empty() const {
        return priority_queue.empty();
    }

    void clear() {
        priority_queue.clear();
    }

    ~PriorityQueue() {
        priority_queue.clear();

    }
};


#endif //PRIORITYQUEUE_H
