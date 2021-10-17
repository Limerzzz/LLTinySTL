#include <cstddef>
#include <iostream>
#include <vector>
using namespace std;
template <class T>
class Widget {
   private:
    T value;
    T* ptr;

   public:
    Widget(size_t n, T val) { std::cout << "constructor 1" << std::endl; }
    template <class It>
    Widget(It begin, It end) {
        std::cout << "constructor 2" << std::endl;
    }
};

int main() {
    Widget<int> a(10, 20);
// test for delete
    // int c = 10;
    // int *p = new int(10);
    // ::printf("%ld \n", p);
    // ::printf("%ld \n", p - 1);
    // p = &c;
    vector<int> b(10,0);
    b.emplace_back(10);
    // b.erase(b.end());
    for(auto i : b){
        ::printf("%d", i);
    }
    system("pause");
    return 0;
}