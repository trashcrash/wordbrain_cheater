#include <iostream>
#include <vector>
#include <string>
const int ALPHABET_SIZE = 26;
const bool DEBUG = 1;

struct trie_node {
    struct trie_node* children[ALPHABET_SIZE];
    bool isword;
};

trie_node* add_node() {
    trie_node* new_node = new trie_node;
    new_node->isword = 0;
    return new_node;
}

void add_word(trie_node* root, std::string word) {
    int word_size = word.size();
    int index;
    for (int i = 0; i < word_size; i++) {
        index = word.at(i)-'a';
        if (!root->children[index])
            root->children[index] = add_node();
        root = root->children[index];
    }
    root->isword = 1;
}

int* drop_letters_column(int* column, int column_size) {
    int i, j;
    for (i = column_size-1, j = column_size-1; j >= 0; j--) {
        if (column[j] != 0)
            column[i--] = column[j];
    }
    while (i >= 0)
        column[i--] = 0;
    return column;
}

int* str_to_int_array(std::string line, int line_len) {
    int* int_array = new int[line_len];
    for (int i = 0; i < line_len; i++)
        // Use ASCII
        int_array[i] = line.at(i);
    return int_array;
}

int** get_letter_matrix() {
    std::string input_line;
    getline(std::cin, input_line);
    int line_len = input_line.size();
    int* first_line = str_to_int_array(input_line, line_len);
    int** matrix = new int*[line_len];
    matrix[0] = first_line;
    for (int i = 1; i < line_len; i++) {
        getline(std::cin, input_line);
        matrix[i] = str_to_int_array(input_line, line_len);
    }
    return matrix;
}
void debug() {
    trie_node* root = add_node();
    add_word(root, "ass");
    std::cout << root->isword << std::endl;
    std::cout << root->children[0]->isword << std::endl;
    std::cout << root->children[0]->children[18]->isword << std::endl;
    std::cout << root->children[0]->children[18]->children[18]->isword << std::endl;
    std::cout << root << std::endl;
    std::cout << root->children[0] << std::endl;
    std::cout << root->children[0]->children[18] << std::endl;
    std::cout << root->children[0]->children[18]->children[18] << std::endl;
    std::cout << root->children[0]->children[18]->children[18]->children[0] << std::endl;
    int a[] = {1,7,0,2,5,7,8,0,3};
    int* p = drop_letters_column(a, 9);
    for (int i = 0; i < 9; i++)
        std::cout << p[i];
    int** matrix = get_letter_matrix();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            std::cout << matrix[i][j] << " ";
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (DEBUG) {
        debug();
        return 0;
    }
    //std::string input_line;
    //getline(std::cin, input_line);

    return 0;
}