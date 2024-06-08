#include <iostream>
#include <atomic>
#include <memory>
#include <type_traits>

// Custom deleter class
template <typename T>
struct CustomDeleter {
    void operator()(T* ptr) {
        std::cout << "Custom deleter called" << std::endl;
        delete ptr;
    }
};

// unique_ptr implementation
template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
public:
    explicit UniquePtr(T* ptr = nullptr) : ptr_(ptr) {}
    ~UniquePtr() { delete_();  }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) {
        if (this != &other) {
            delete_();
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T* get() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }

private:
    T* ptr_;
    Deleter deleter_;

    void delete_() {
        if (ptr_) {
            deleter_(ptr_);
            ptr_ = nullptr;
        }
    }
};

// shared_ptr implementation
template <typename T>
class SharedPtr {
public:
    explicit SharedPtr(T* ptr = nullptr) : ptr_(ptr), refCount_(new std::atomic<int>(1)) {}
    ~SharedPtr() { decrementAndDelete(); }

    SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), refCount_(other.refCount_) {
        (*refCount_)++;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            decrementAndDelete();
            ptr_ = other.ptr_;
            refCount_ = other.refCount_;
            (*refCount_)++;
        }
        return *this;
    }

    SharedPtr(SharedPtr&& other) : ptr_(other.ptr_), refCount_(other.refCount_) {
        other.ptr_ = nullptr;
        other.refCount_ = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) {
        if (this != &other) {
            decrementAndDelete();
            ptr_ = other.ptr_;
            refCount_ = other.refCount_;
            other.ptr_ = nullptr;
            other.refCount_ = nullptr;
        }
        return *this;
    }

    T* get() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    int use_count() const { return *refCount_; }

private:
    T* ptr_;
    std::atomic<int>* refCount_;

    void decrementAndDelete() {
        if (--(*refCount_) == 0) {
            delete ptr_;
            delete refCount_;
        }
    }
};

// weak_ptr implementation
template <typename T>
class WeakPtr {
public:
    WeakPtr() : ptr_(nullptr), refCount_(nullptr) {}
    WeakPtr(const SharedPtr<T>& other) : ptr_(other.ptr_), refCount_(other.refCount_) {}

    WeakPtr(const WeakPtr& other) : ptr_(other.ptr_), refCount_(other.refCount_) {}
    WeakPtr& operator=(const WeakPtr& other) {
        ptr_ = other.ptr_;
        refCount_ = other.refCount_;
        return *this;
    }

    bool expired() const { return refCount_ == nullptr || *refCount_ == 0; }
    SharedPtr<T> lock() const {
        if (!expired()) {
            return SharedPtr<T>(*this);
        }
        return SharedPtr<T>();
    }

private:
    T* ptr_;
    std::atomic<int>* refCount_;
};

// make_unique function template
template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

// make_shared function template
template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

// Test suite
void testUnique() {
    std::cout << "Testing UniquePtr..." << std::endl;

    UniquePtr<int> p1(new int(42));
    std::cout << "*p1 = " << *p1 << std::endl;

    UniquePtr<int> p2(std::move(p1));
    std::cout << "*p2 = " << *p2 << std::endl;

    UniquePtr<int, CustomDeleter<int>> p3(new int(10));
    std::cout << "*p3 = " << *p3 << std::endl;

    std::cout << std::endl;
}

void testShared() {
    std::cout << "Testing SharedPtr..." << std::endl;

    SharedPtr<int> p1(new int(42));
    std::cout << "*p1 = " << *p1 << ", use_count: " << p1.use_count() << std::endl;

    SharedPtr<int> p2(p1);
    std::cout << "*p2 = " << *p2 << ", use_count: " << p2.use_count() << std::endl;

    SharedPtr<int> p3;
    p3 = p1;
    std::cout << "*p3 = " << *p3 << ", use_count: " << p3.use_count() << std::endl;

    std::cout << std::endl;
}

void testWeak() {
    std::cout << "Testing WeakPtr..." << std::endl;

    SharedPtr<int> sp(new int(42));
    WeakPtr<int> wp(sp);

    std::cout << "WeakPtr expired: " << std::boolalpha << wp.expired() << std::endl;

    SharedPtr<int> sp2 = wp.lock();
    if (sp2) {
        std::cout << "*sp2 = " << *sp2 << std::endl;
    }

    sp.reset();
    std::cout << "WeakPtr expired: " << std::boolalpha << wp.expired() << std::endl;

    std::cout << std::endl;
}

void testMakeUnique() {
    std::cout << "Testing make_unique..." << std::endl;

    auto p = make_unique<int>(42);
    std::cout << "*p = " << *p << std::endl;

    std::cout << std::endl;
}

void testMakeShared() {
    std::cout << "Testing make_shared..." << std::endl;

    auto p = make_shared<int>(42);
    std::cout << "*p = " << *p << ", use_count: " << p.use_count() << std::endl;

    std::cout << std::endl;
}

int main() {
    testUnique();
    testShared();
    testWeak();
    testMakeUnique();
    testMakeShared();

    return 0;
}
