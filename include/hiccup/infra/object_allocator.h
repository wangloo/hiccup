/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef OBJECT_ALLOCATOR_H
#define OBJECT_ALLOCATOR_H

#include "hiccup/infra/link.h"
#include <new>

namespace hiccup {

template<typename T>
class ObjectAllocator {
public:
    explicit ObjectAllocator(size_t capacity) {
        for (size_t i = 0; i < capacity; i++) {
            auto elem = new (std::nothrow) Element;
            if (!elem) {
				return;
			}
            elems_.push_back(elem->node_);
        }
    }

    ~ObjectAllocator() {
        while (!elems_.empty()) {
            auto elem = (Element*)(elems_.pop_front());
            if (elem) delete elem;
        }
    }
	
	// Destructor has removed the rvalue constructor and assignment operator implicitly.
	// So we need to add them back.
	ObjectAllocator(ObjectAllocator&&) noexcept = default;
	ObjectAllocator& operator=(ObjectAllocator&&) noexcept = default;

    // Alloc memory but do not construct !!!
    T* Alloc() {
        Element *elem = (Element*) (elems_.pop_front());
        if (!elem) elem = new (std::nothrow) Element;
		assert(elem && "Out of memory");
        return (T*) (elem->buff_);
    }

    // Free memory but do not destruct !!!
    void Free(T &elem) {
        elems_.push_front(*(ElemNode*) (&elem));
    }

    // Alloc memory and construct !!!
    template<class ... Args>
    T* New(Args &&... args) {
		T *elem = Alloc();
		assert(elem && "Out of memory");
		return new(elem) T(std::forward<Args>(args)...);
    }

    // Free memory and destruct !!!
    void Delete(T &elem) {
        elem.~T();
        Free(elem);
    }

    size_t GetAvailableSize() const {
        return elems_.size();
    }

private:
    struct ElemNode: LinkNode<ElemNode> {
    };

    union Element {
        Element() {
        }
        ElemNode node_;
        char buff_[sizeof(T)];
    };

private:
    Link<ElemNode> elems_;
};

}

#endif
