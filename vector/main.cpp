#include "vector.hpp"
#include <iostream> 
#include <utility> 

using namespace std; 

int main() { 

    Vector<int> a; 

    for (int i = 1; i <= 10; ++i) { 
        a.push_back(i); 
    }

    int* ptr = a.data(); 

    cout << *ptr << endl;

    a.pop_back(); 
    
    cout << a.back() << endl;

    a.pop_back(); 

    cout << a.back() << endl;

    a.push_back(5); 

    cout << a.back() << endl;



    return 0;   
}