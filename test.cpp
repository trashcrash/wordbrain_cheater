#include <iostream>
#include <vector>
#include <string>

using namespace std;
const int ALPHABET_SIZE = 26;
/*
struct trie_node {
    struct trie_node* children[ALPHABET_SIZE];
    bool isword;
};
trie_node* add_node() {
    trie_node* new_node = new trie_node;
    new_node->isword = 0;
    return new_node;
}
*/
void abc(int* b) {
    b[0] = 5;
}
int main() {
    /*
    int array1[] = {1,2,3,4};
    int array2[] = {5,6,7,8};
    int* a = array1;
    int* b = array2;
    int* ptrarray[] = {a, b};
    int** p = ptrarray;
    cout << p[1][3] << endl;
    */
    /*
    string x = "hello";
    int* p = abc();
    cout << p << endl;
    */
    int a[] = {0,1,2};
    abc(a);
    cout << a[0] << endl;

    return 0;
}