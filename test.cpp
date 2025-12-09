
#include <ctime>
#include <iostream>

int main() {
    std::time_t now = std::time(nullptr);  // current epoch time in seconds
    std::cout << "Now: " << now << std::endl;
    return 0;
}
