#ifndef DESIGN_PATTERNS_SINGELTON_HPP
#define DESIGN_PATTERNS_SINGELTON_HPP


class Singleton {
    private:
        static Singleton* _instance;
        Singleton();
        Singleton(const Singleton&);
        Singleton& operator=(const Singleton&);

    public:
        static Singleton* get_instance();
        ~Singleton();
};


#endif //DESIGN_PATTERNS_SINGELTON_HPP
