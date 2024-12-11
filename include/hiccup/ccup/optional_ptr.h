/**
* Copyright (c) wangbo@joycode.art 2024
*/

#ifndef OPTIONAL_PTR_H
#define OPTIONAL_PTR_H

#include <shared_mutex>
#include <utility>

namespace ccup
{ 

enum class SyncMode {
    None,
    Sync,
};

template<typename T, SyncMode = SyncMode::None>
class OptionalPtr {
public:
    explicit OptionalPtr(T* ptr) : ptr_(ptr) {}

    OptionalPtr(OptionalPtr&& other) noexcept 
    : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    OptionalPtr& operator=(OptionalPtr&& other) noexcept {
        if (this != &other) {
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    bool HasValue() const { return ptr_ != nullptr; }
    explicit operator bool() const { return HasValue(); }
    T* Get() { return ptr_; }

    T* operator->() {
        assert(ptr_ && "OptionalPtr is null. Assertion failed.");
        return ptr_;
    }

    T& operator*() { 
        return *ptr_; 
    }

    template<typename Handle, typename Fail>
    void Match(Handle& handle, Fail& fail) {
        if (ptr_) {
            handle(*ptr_);
        } else {
            fail();
        }
    }

    template<typename Handle>
    void Apply(Handle& handle) {
        assert(ptr_ && "OptionalPtr is null. Assertion failed.");
        handle(*ptr_);
    }

private:
    T* ptr_;
};

template<typename T>
class OptionalPtr<const T, SyncMode::None> {
public:
    explicit OptionalPtr(const T* ptr) : ptr_(ptr) {}

    OptionalPtr(OptionalPtr&& other) noexcept 
    : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    OptionalPtr& operator=(OptionalPtr&& other) noexcept {
        if (this != &other) {
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    bool HasValue() const { return ptr_ != nullptr; }
    explicit operator bool() const { return HasValue(); }
    const T* Get() const { return ptr_; }

    const T* operator->() const {
        assert(ptr_ && "OptionalPtr is null. Assertion failed.");
        return ptr_;
    }

    const T& operator*() const { 
        return *ptr_; 
    }

    template<typename Handle, typename Fail>
    void Match(const Handle& handle, const Fail& fail) const {
        if (ptr_) {
            handle(*ptr_);
        } else {
            fail();
        }
    }

    template<typename Handle>
    void Apply(const Handle& handle) const {
        assert(ptr_ && "OptionalPtr is null. Assertion failed.");
        handle(*ptr_);
    }

private:
    const T* ptr_;
};

template<typename T>
class OptionalPtr<T, SyncMode::Sync> {
public:
    OptionalPtr(std::shared_timed_mutex& mtx, T* ptr) 
    : lock_(mtx), ptr_(ptr) {
    }

    OptionalPtr(OptionalPtr&& other) noexcept 
    : lock_(std::move(other.lock_)), ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    OptionalPtr& operator=(OptionalPtr&& other) noexcept {
        if (this != &other) {
            lock_ = std::move(other.lock_);
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    bool HasValue() const { return ptr_ != nullptr; }
    explicit operator bool() const { return HasValue(); }
    T* Get() { return ptr_; }

    T* operator->() {
        assert(ptr_ && "OptionalPtr is null. Assertion failed.");
        return ptr_;
    }

    T& operator*() { 
        return *ptr_; 
    }

    template<typename Handle, typename Fail>
    void Match(Handle& handle, Fail& fail) {
        if (ptr_) {
            handle(*ptr_);
        } else {
            fail();
        }
    }

    template<typename Handle>
    void Apply(Handle& handle) {
        assert(ptr_ && "OptionalPtr is null. Assertion failed.");
        handle(*ptr_);
    }

private:
    std::unique_lock<std::shared_timed_mutex> lock_;
    T* ptr_;
};

template<typename T>
class OptionalPtr<const T, SyncMode::Sync> {
public:
    OptionalPtr(std::shared_timed_mutex& mtx, T* ptr) 
    : lock_(mtx), ptr_(ptr)  {
    }

    OptionalPtr(OptionalPtr&& other) noexcept 
    : lock_(std::move(other.lock_)), ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    OptionalPtr& operator=(OptionalPtr&& other) noexcept {
        if (this != &other) {
            lock_ = std::move(other.lock_);
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    bool HasValue() const { return ptr_ != nullptr; }
    explicit operator bool() const { return HasValue(); }
    const T* Get() const { return ptr_; }

    const T* operator->() const {
        assert(ptr_ && "OptionalPtr is null. Assertion failed.");
        return ptr_;
    }

    const T& operator*() const { 
        return *ptr_; 
    }

    template<typename Handle, typename Fail>
    void Match(const Handle& handle, const Fail& fail) const {
        if (ptr_) {
            handle(*ptr_);
        } else {
            fail();
        }
    }

    template<typename Handle>
    void Apply(const Handle& handle) const {
        assert(ptr_ && "OptionalPtr is null. Assertion failed.");
        handle(*ptr_);
    }

private:
    std::shared_lock<std::shared_timed_mutex> lock_;
    const T* ptr_;
};

}

#endif
