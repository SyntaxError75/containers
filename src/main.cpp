#include <iostream>
#include "s21_vector.h"

int main() {
    
    s21::vector<int> n(5);
    
    std::cout << (n.data()== nullptr ) << " " << n.size() << " " << n.capacity() << "\n";
    return 0;
}