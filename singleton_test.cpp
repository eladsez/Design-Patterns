#include "singleton.hpp"

using namespace std;

/**
 * @brief This class is used to test the Singleton class.
 * @details Every class which have a friend class declaration of the Singleton class can create an instance
 *          using the Singleton<the desired class>::Instance() function call (the instance be the same for all the calls).
 * @author Elad Sezanayev
 */
class Test {
private:
    friend class Singleton<Test>; // The statement Test Friends for singleton class templates
    string str;

    Test() : str("abc") {}

public:
    void set_str(string t) {
        str = t;
    }

    void print() {
        cout << "str = " << str << endl;
        cout << "The instance pointer = " << this << endl;
    }
};

int main() {
    Test *pt1 = Singleton<Test>::Instance();
    Test *pt2 = Singleton<Test>::Instance();
    cout << "blabla"<< endl;
    pt1->print();
    pt2->print(); // should print the same address

    pt1->set_str("ABCDEFG");
    pt2->print(); // should print the ABCDEFG, although we changed the str of pt1

    return 0;
}