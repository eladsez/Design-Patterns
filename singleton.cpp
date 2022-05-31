#include "cmake-build-debug/singleton.hpp"

Singleton *Singleton::_instance = nullptr;

Singleton::Singleton() {

}

Singleton *Singleton::get_instance() {
    if (_instance == NULL) {
        _instance = new Singleton();
    }
    return _instance;
}

Singleton::~Singleton() {
    if (_instance != NULL) {
        delete _instance;
        _instance = nullptr;
    }
}

