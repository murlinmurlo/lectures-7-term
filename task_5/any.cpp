#include <iostream>
#include <memory>
#include <typeinfo>
#include <stdexcept>
#include <string>

class AnyException : public std::runtime_error {
public:
    explicit AnyException(const std::string& message) : std::runtime_error(message) {}
};

class Any {
public:
    Any() : content(nullptr) {}

    template<typename T>
    Any(const T& value) : content(std::make_unique<Holder<T>>(value)) {}

    ~Any() = default;

    template<typename T>
    T get() const {
        return getValue<T>();
    }

private:
    class BaseHolder {
    public:
        virtual ~BaseHolder() = default;
        virtual const std::type_info& type() const = 0;
    };

    template<typename T>
    class Holder : public BaseHolder {
    public:
        Holder(const T& value) : value(value) {}
        const std::type_info& type() const override {
            return typeid(T);
        }

        T value;
    };

    std::unique_ptr<BaseHolder> content;

    template<typename T>
    T getValue() const {
        checkEmpty();
        checkType<T>();
        return static_cast<Holder<T>*>(content.get())->value;
    }

    void checkEmpty() const {
        if (!content) {
            throw AnyException("No Any here");
        }
    }

    template<typename T>
    void checkType() const {
        if (typeid(T) != content->type()) {
            throw AnyException("Bad cast");
        }
    }
};

int main() {
    Any a(5);
    std::cout << a.get<int>() << std::endl;

    try {
        a.get<std::string>();
    } catch (const AnyException& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    Any b(std::string("Hello, World!"));
    std::cout << b.get<std::string>() << std::endl;

    return 0;
}
