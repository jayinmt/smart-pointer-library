# Custom Smart Pointer Library

This project is a custom smart pointer library implemented in C++. It demonstrates the understanding of memory management, templates, and modern C++ features. The library includes implementations of `unique_ptr`, `shared_ptr`, and `weak_ptr`, along with additional functionality and optimizations.

## Features

1. `UniquePtr`: A custom `unique_ptr` template class that manages the lifetime of a single dynamically allocated object. It supports move semantics, custom deleters, and provides a similar interface to the standard library's `unique_ptr`.

2. `SharedPtr`: A custom `shared_ptr` template class that maintains shared ownership of an object through a reference counting mechanism. It handles thread-safety, supports custom deleters, and provides an interface similar to the standard library's `shared_ptr`.

3. `WeakPtr`: A custom `weak_ptr` template class that acts as a non-owning observer of an object managed by `SharedPtr`. It can detect and avoid dangling references.

4. `make_unique`: A function template that constructs an object of a given type and returns a `UniquePtr` to that object.

5. `make_shared`: A function template that constructs an object of a given type and returns a `SharedPtr` to that object, optimizing the memory allocation by combining the object and the reference count in a single allocation.

6. Test Suite: A comprehensive test suite that demonstrates the usage and behavior of the custom smart pointers in various scenarios, including exception safety, thread safety, and custom deleter functionality.

7. Modern C++ Features: The library utilizes modern C++ features such as move semantics, perfect forwarding, variadic templates, and type traits to enhance the performance and flexibility of the smart pointer implementations.

## Usage

To use the custom smart pointer library, simply include the necessary header files in your C++ project and start using the smart pointers.

```cpp
#include "smart_pointers.h"

int main() {
    // Using UniquePtr
    UniquePtr<int> uniquePtr = make_unique<int>(42);
    std::cout << *uniquePtr << std::endl;

    // Using SharedPtr
    SharedPtr<std::string> sharedPtr = make_shared<std::string>("Hello, World!");
    std::cout << *sharedPtr << std::endl;

    // Using WeakPtr
    WeakPtr<std::string> weakPtr(sharedPtr);
    if (!weakPtr.expired()) {
        SharedPtr<std::string> sharedPtr2 = weakPtr.lock();
        std::cout << *sharedPtr2 << std::endl;
    }

    return 0;
}
```

## Building and Running

To build and run the project, follow these steps:

1. Clone the repository:
   ```
   git clone https://github.com/jayinmt/smart-pointer-library.git
   ```

2. Navigate to the project directory:
   ```
   cd custom-smart-pointer-library
   ```

3. Compile the source files:
   ```
   g++ -std=c++17 -o smart_pointers smart_pointers.cpp
   ```

4. Run the executable:
   ```
   ./smart_pointers
   ```

## Contributing

Contributions to the custom smart pointer library are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).
