#ifndef OOP_EXCEPTIONS_H
#define OOP_EXCEPTIONS_H
#include <exception>
#include <string>

class GameException : public std::exception {
protected:
    std::string message;
public:
    explicit GameException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class ResourceLoadException : public GameException {
public:
    explicit ResourceLoadException(const std::string& msg)
        : GameException("Load Error: " + msg) {}
};

class GenerationException : public GameException {
public:
    explicit GenerationException(const std::string& msg)
        : GameException("Generation Error: " + msg) {}
};

class PhysicsException : public GameException {
public:
    explicit PhysicsException(const std::string& msg)
        : GameException("Physics Error: " + msg) {}
};

#endif //OOP_EXCEPTIONS_H