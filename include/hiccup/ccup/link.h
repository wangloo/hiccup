/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef LINK_H
#define LINK_H

#include "hiccup/ccup/link_node.h"
#include <stddef.h>
#include <new>

namespace ccup {

template<typename T>
struct Link {

    struct Iterator {
        Iterator() noexcept :
                elem_(0), next_(0) {
        }

        explicit Iterator(T *elem) noexcept :
                elem_(elem), next_(next_of(elem)) {
        }

        Iterator(const Iterator &rhs) noexcept :
                elem_(rhs.elem_), next_(rhs.next_) {
        }

        Iterator& operator=(const Iterator &other) noexcept {
            if (this != &other) {
                elem_ = other.elem_;
                next_ = other.next_;
            }
            return *this;
        }

        bool operator==(const Iterator &rhs) const noexcept {
            return elem_ == rhs.elem_;
        }

        bool operator!=(const Iterator &rhs) const noexcept {
            return !(*this == rhs);
        }

        void reset() noexcept {
            elem_ = 0;
            next_ = 0;
        }

        T* operator->() noexcept {
            return elem_;
        }

        T& operator*() noexcept {
            return *elem_;
        }

        T* value() const noexcept {
            return elem_;
        }

        Iterator operator++(int) noexcept {
            Iterator i = *this;

            elem_ = next_;
            next_ = next_of(elem_);

            return i;
        }

        Iterator& operator++() noexcept {
            elem_ = next_;
            next_ = next_of(elem_);

            return *this;
        }

    private:
        static T* next_of(T *elem) noexcept {
            return elem == 0 ? 0 : Link<T>::next_of(elem);
        }

        static T* prev_of(T *elem) noexcept {
            return elem == 0 ? 0 : Link<T>::prev_of(elem);
        }
    private:
        T *elem_;
        T *next_;
    };

    struct ReverseIterator {
        ReverseIterator() noexcept :
                elem_(0), next_(0) {
        }

        explicit ReverseIterator(T *elem) noexcept :
                elem_(elem), next_(next_of(elem)) {
        }

        ReverseIterator(const ReverseIterator &rhs) noexcept :
                elem_(rhs.elem_), next_(rhs.next_) {
        }

        ReverseIterator& operator=(const ReverseIterator &other) noexcept {
            if (this != &other) {
                elem_ = other.elem_;
                next_ = other.next_;
            }
            return *this;
        }

        bool operator==(const ReverseIterator &rhs) const noexcept {
            return elem_ == rhs.elem_;
        }

        bool operator!=(const ReverseIterator &rhs) const noexcept {
            return !(*this == rhs);
        }

        void reset() noexcept {
            elem_ = 0;
            next_ = 0;
        }

        T* operator->() noexcept {
            return elem_;
        }

        T& operator*() noexcept {
            return *elem_;
        }

        T* value() const noexcept {
            return elem_;
        }

        ReverseIterator operator++(int) noexcept {
            ReverseIterator i = *this;

            elem_ = next_;
            next_ = next_of(elem_);

            return i;
        }

        ReverseIterator& operator++() noexcept {
            elem_ = next_;
            next_ = next_of(elem_);

            return *this;
        }

    private:
        static T* next_of(T *elem) noexcept {
            return elem == 0 ? 0 : Link<T>::prev_of(elem);
        }

    private:
        T *elem_;
        T *next_;
    };

    Link() : num_(0) {
        head_.next_ = sentinel();
        head_.prev_ = sentinel();
    }

    Link(Link &&other) noexcept {
        *this = std::move(other);
    }

    Link& operator=(Link &&other) noexcept {
        if (this != &other) {
            this->reset();
            this->concat(other);
        }
        return *this;
    }

    bool empty() const {
        return head_.next_ == sentinel();
    }

    size_t size() const {
        return num_;
    }

    Iterator begin() const {
        return Iterator{head_.next_};
    }

    Iterator end() const {
        return Iterator{const_cast<T*>(sentinel())};
    }

    ReverseIterator rbegin() const {
        return ReverseIterator{head_.prev_};
    }

    ReverseIterator rend() const {
        return ReverseIterator{const_cast<T*>(sentinel())};
    }

    bool is_back(T *elem) const {
        return elem == head_.prev_;
    }

    bool is_front(T *elem) const {
        return elem == head_.next_;
    }

    T* front() const {
        return empty() ? 0 : head_.next_;
    }

    T* back() const {
        return empty() ? 0 : head_.prev_;
    }

    void push_back(T &elem) {
        elem.link_.next_ = sentinel();
        elem.link_.prev_ = sentinel()->link_.prev_;
        sentinel()->link_.prev_->link_.next_ = &elem;
        sentinel()->link_.prev_ = &elem;

        num_++;
    }

    void push_front(T &elem) {
        elem.link_.prev_ = sentinel();
        elem.link_.next_ = sentinel()->link_.next_;
        sentinel()->link_.next_->link_.prev_ = &elem;
        sentinel()->link_.next_ = &elem;

        num_++;
    }

    void push_back(Iterator &elem) {
        push_back(*elem);
    }

    void push_front(Iterator &elem) {
        push_front(*elem);
    }

    T* pop_front() {
        Iterator i = begin();
        if (i == end()) {
            return 0;
        }

        erase(i);
        return &(*i);
    }

    void concat(Link<T> &other) {
        if (other.empty()) return;

        other.head_.prev_->link_.next_ = sentinel();
        other.head_.next_->link_.prev_ = sentinel();
        sentinel()->link_.prev_->link_.next_ = other.head_.next_;
        sentinel()->link_.prev_ = other.head_.prev_;

        num_ += other.size();
        other.reset();
    }

    void remove(T &elem) {
        elem.remove();
        num_--;
    }

    void erase(Iterator elem) {
        if (!elem.value()) return;

        remove(*elem);
    }

    void clear() {
        while (pop_front() != 0);
    }

    Iterator next_of(Iterator &i) const {
        return (i == end()) ? end() : ++i;
    }

private:
    static T* next_of(T *elem) {
        return elem->link_.next_;
    }

    static T* prev_of(T *elem) {
        return elem->link_.prev_;
    }

    void reset() {
        head_.next_ = sentinel();
        head_.prev_ = sentinel();
        num_ = 0;
    }

    T* sentinel() {
        return (T*) ((char*) (&(head_.next_)) - offsetof(T, link_));
    }

    const T* sentinel() const {
        return (T*) ((char*) (&(head_.next_)) - offsetof(T, link_));
    }

private:
    typename LinkNode<T>::Chain head_;
    size_t num_;
};

}

#endif
