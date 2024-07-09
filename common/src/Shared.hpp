#ifndef __SHARED_HPP
#define __SHARED_HPP

#include <mutex>

template <typename T>
class Shared {
public:
    Shared() noexcept : _ptr(nullptr), _refCount(nullptr), _mutex(nullptr) {}

    explicit Shared(T* ptr) noexcept : _ptr(ptr), _refCount(new unsigned int(1)), _mutex(new std::mutex()) {}

    Shared(const Shared<T>& other) noexcept {
        std::lock_guard<std::mutex> lock(*other._mutex);
        _ptr = other._ptr;
        _refCount = other._refCount;
        _mutex = other._mutex;
        incrementeRefCount();
    }

    Shared(Shared<T>&& other) noexcept : _ptr(other._ptr), _refCount(other._refCount), _mutex(other._mutex) {
        other._ptr = nullptr;
        other._refCount = nullptr;
        other._mutex = nullptr;
    }

    ~Shared() {
        release();
    }

    Shared<T>& operator=(const Shared<T>& other) {
        if (this != &other) {
            release();

            std::lock_guard<std::mutex> lock(*other._mutex);
            _ptr = other._ptr;
            _refCount = other._refCount;
            _mutex = other._mutex;
            incrementeRefCount();
        }
        return *this;
    }

    Shared<T>& operator=(Shared<T>&& other) noexcept {
        if (this != &other) {
            release();

            _ptr = other._ptr;
            _refCount = other._refCount;
            _mutex = other._mutex;

            other._ptr = nullptr;
            other._refCount = nullptr;
            other._mutex = nullptr;
        }
        return *this;
    }

    T& operator*() {
        return *_ptr;
    }

    T* operator->() {
        return _ptr;
    }

    T* get() const {
        return _ptr;
    }

    bool ok() const {
        return _ptr != nullptr;
    }

    bool operator!() const {
        return _ptr == nullptr;
    }

    bool operator==(const Shared<T>& other) const {
        return _ptr == other._ptr;
    }

    bool operator!=(const Shared<T>& other) const {
        return _ptr != other._ptr;
    }

private:
    void incrementeRefCount() {
        if (_refCount) {
            std::lock_guard<std::mutex> lock(*_mutex);
            ++(*_refCount);
        }
    }

    void decrementRefCount() {
        if (_refCount) {
            std::lock_guard<std::mutex> lock(*_mutex);
            if (*_refCount > 0) {
                --(*_refCount);
            }
        }
    }

    void release() {
        if (_refCount && _mutex) {
            decrementRefCount();
            if (*_refCount == 0) {
                if (_ptr) delete _ptr;
                delete _refCount;
                delete _mutex;
            }
            _ptr = nullptr;
            _refCount = nullptr;
            _mutex = nullptr;
        }
    }

    T* _ptr;
    unsigned int* _refCount;
    std::mutex* _mutex;
};

#endif // __SHARED_HPP
