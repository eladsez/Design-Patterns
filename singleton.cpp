#include "singleton.hpp"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>


using namespace std;

/**
 * @brief This class is used to test the Singleton class.
 * @details Every class which have a friend class declaration of the Singleton class can create an instance
 *          using the Singleton<the desired class>::Instance() function call (the instance be the same for all the calls).
 *          This class mapping a file into memory using mmap and have a method to print the content of the file.
 * @author Elad Sezanayev
 */
class Test {
private:
    friend class Singleton<Test>; // using this declaration the class can create an instance of the class using the Instance() function call
    char *str;
    struct stat st;

    Test() {
        int fd = open("./singleton.txt", O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(1);
        }
        if (fstat(fd, &st) == -1) {
            perror("fstat");
            exit(1);
        }
        str = (char *) mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (str == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }
    }

public:
    void print_file_content() {
        for (int i = 0; i < st.st_size; i++) {
            putchar(str[i]);
        }
    }
};

/**
 * @brief This main will show that every instance of the class Test will be the same, and that the print_file_content
 *        method will be print the same file.
 */
int main() {
    Test *pt1 = Singleton<Test>::Instance();
    Test *pt2 = Singleton<Test>::Instance();

    // should print the same address
    cout << "pt1: " << pt1 << endl;
    cout << "pt2: " << pt2 << endl << endl;

    cout << "pt1 file content: " << endl;
    pt1->print_file_content(); // Print the content of the file
    cout << endl << endl;

    cout << "pt2 file content: " << endl;
    pt2->print_file_content(); // This will print the same file as pt1
    cout << endl;

    printf("\033[1;35m"); // change text color
    if (pt1 == pt2) {
        cout << "singleton test end successfully" << endl;
    } else {
        cout << "singleton test failed" << endl;
    }
    return 0;
}