#ifndef INTRUSIVE_PTR_H
#define INTRUSIVE_PTR_H

#include <utility>
#include <iostream>

template<typename T>
class TRefCounter {
public:
    TRefCounter() : ref_count(0) {}

    void NewRef() {
        ++ref_count;
    }

    void Release() {
        if (--ref_count == 0) {
            delete static_cast<T*>(this);
        }
    }

    size_t RefCount() const {
        return ref_count;
    }

private:
    size_t ref_count;
};

template<typename T>
class TPtr {
public:
    TPtr(T* ptr = nullptr) : ptr(ptr) {
        if (ptr) {
            ptr->NewRef();
        }
    }

    TPtr(const TPtr& other) : ptr(other.ptr) {
        if (ptr) {
            ptr->NewRef();
        }
    }

    TPtr(TPtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    ~TPtr() {
        Reset();
    }

    TPtr& operator=(const TPtr& other) {
        if (this != &other) {
            Reset();
            ptr = other.ptr;
            if (ptr) {
                ptr->NewRef();
            }
        }
        return *this;
    }

    TPtr& operator=(TPtr&& other) noexcept {
        if (this != &other) {
            Reset();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    T* operator->() const {
        return ptr;
    }

    T& operator*() const {
        return *ptr;
    }

    explicit operator bool() const {
        return ptr != nullptr;
    }

    bool operator==(const TPtr& other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const TPtr& other) const {
        return ptr != other.ptr;
    }

    T* Get() const {
        return ptr;
    }

    void Reset(T* ptr_new = nullptr) {
        if (ptr) {
            ptr->Release();
        }
        ptr = ptr_new;
        if (ptr) {
            ptr->NewRef();
        }
    }

    void Reset(const TPtr& ptr_new) {
        Reset(ptr_new.Get());
    }

    T* Release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }

    size_t RefCount() const {
        return ptr ? ptr->RefCount() : 0;
    }

protected:
    T* ptr;
};

template<typename T>
class TIntrusivePtr : public TPtr<T> {
public:
    using TPtr<T>::TPtr;

    size_t RefCount() const {
        return TPtr<T>::RefCount();
    }
};

template<typename T, typename... Args>
TIntrusivePtr<T> MakeIntrusive(Args&&... args) {
    return TIntrusivePtr<T>(new T(std::forward<Args>(args)...));
}

#endif
