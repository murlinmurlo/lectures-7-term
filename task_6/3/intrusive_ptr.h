#include <iostream>
#include <utility>
#include <cassert>

template<typename T>
class TRef_counter {
private:
    size_t ref_count;

public:
    TRef_counter() : ref_count(0) {}

    void NewRef() {
        ++ref_count;
    }

    void Release() {
        --ref_count;
        if (ref_count == 0) {
            delete static_cast<T*>(this);
        }
    }

    size_t RefCount() const {
        return ref_count;
    }
};

template<typename T>
class TPtr {
public:
    T* ptr;

    TPtr(T* ptr = nullptr) : ptr(ptr) {}

    bool operator==(const TPtr& other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const TPtr& other) const {
        return ptr != other.ptr;
    }

    T* operator->() const {
        return ptr;
    }

    T& operator*() const {
        return *ptr;
    }

    operator bool() const {
        return ptr != nullptr;
    }

    T* Get() const {
        return ptr;
    }
};

template<typename T>
class TIntrusivePtr : public TPtr<T> {
public:
    using TPtr<T>::TPtr;
        size_t RefCount() const {
        return this->ptr ? this->ptr->RefCount() : 0;
    }

    void Reset(T* p = nullptr) {
        if (this->ptr) {
            this->ptr->Release();
        }
        this->ptr = p;
        if (this->ptr) {
            this->ptr->NewRef();
        }
    }

    void Reset(TIntrusivePtr& other) {
        if (this != &other) {
            if (this->ptr) {
                this->ptr->Release();
            }
            this->ptr = other.ptr;
            if (this->ptr) {
                this->ptr->NewRef();
            }
        }
    }

    void Reset(TIntrusivePtr&& other) {
        if (this != &other) {
            if (this->ptr) {
                this->ptr->Release();
            }
            this->ptr = other.ptr;
            other.ptr = nullptr;
        }
    }

    T* Release() {
        T* temp = this->ptr;
        this->ptr = nullptr;
        return temp;
    }

    TIntrusivePtr(T* p = nullptr) : TPtr<T>(p) {
        if (this->ptr) {
            this->ptr->NewRef();
        }
    }

    TIntrusivePtr(const TIntrusivePtr& other) : TPtr<T>(other.ptr) {
        if (this->ptr) {
            this->ptr->NewRef();
        }
    }

    TIntrusivePtr(TIntrusivePtr&& other) noexcept : TPtr<T>(other.ptr) {
        other.ptr = nullptr;
    }

    ~TIntrusivePtr() {
        Reset();
    }

    TIntrusivePtr& operator=(const TIntrusivePtr& other) {
        if (this != &other) {
            if (this->ptr) {
                this->ptr->Release();
            }
            this->ptr = other.ptr;
            if (this->ptr) {
                this->ptr->NewRef();
            }
        }
        return *this;
    }

    TIntrusivePtr& operator=(TIntrusivePtr&& other) noexcept {
        if (this != &other) {
            if (this->ptr) {
                this->ptr->Release();
            }
            this->ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }
};

template<typename T, typename... Args>
TIntrusivePtr<T> MakeIntrusive(Args&&... args) {
    return TIntrusivePtr<T>(new T(std::forward<Args>(args)...));
}