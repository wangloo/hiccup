/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef LINK_NODE_H
#define LINK_NODE_H

namespace ccup {

template<typename T> struct List;

template<typename T>
struct LinkNode {
    LinkNode() {
        link_.prev_ = 0;
        link_.next_ = 0;
    }

    void remove() {
        // Notice: Just used in scenes careless num of link!!!
        link_.prev_->link_.next_ = link_.next_;
        link_.next_->link_.prev_ = link_.prev_;
    }

    T* next() {
        return link_.next_;
    }

    const T* next() const {
        return link_.next_;
    }

    T* prev() {
        return link_.prev_;
    }

    const T* prev() const {
        return link_.prev_;
    }

    friend struct List<T> ;

    struct Chain {
        T *volatile next_;
        T *volatile prev_;
    }; // __cacheline_aligned;

    Chain link_;
};

}

#endif
