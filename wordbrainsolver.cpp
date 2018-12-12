#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
const int ALPHABET_SIZE = 26;
const bool DEBUG = 0;
int ENDFILE = 0;
bool FOUNDWORD = 0;
struct trie_node {
    struct trie_node* children[ALPHABET_SIZE];
    bool isword;
};

struct data {
    int** matrix;
    int dimension;
    int word_n;
    std::vector<int> letter_n;
    //int all_letter_n;
    std::vector<std::string> hint;
};

struct recur {
    int start_coord[2];
    int layer;
    int nth_word;
    std::string word;
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

void drop_letters_column(int* column, int column_size) {
    int i, j;
    for (i = 0, j = 0; j < column_size; j++) {
        if (column[j] != -1)
            column[i++] = column[j];
    }
    while (i < column_size)
        column[i++] = -1;
}

void drop_letters_matrix(int** matrix, int dimension) {
    for (int i = 0; i < dimension; i++)
        drop_letters_column(matrix[i], dimension);
}

int* str_to_int_array(std::string line, int line_len) {
    int* int_array = new int[line_len];
    for (int i = 0; i < line_len; i++)
        // Use ASCII
        int_array[i] = line.at(i);
    return int_array;
}

data get_letter_matrix() {
    data mat_info;
    std::string input_line, temp;
    getline(std::cin, input_line);
    int line_len = input_line.size();
    int word_n = 0;
    //int all_letter_n = 0;
    int* first_line = str_to_int_array(input_line, line_len);
    std::vector<int> letter_n;
    std::vector<std::string> hint;
    int** matrix = new int*[line_len];
    for (int i = 0; i < line_len; i++)
        matrix[i] = new int[line_len];
    for (int i = 0; i < line_len; i++)
        matrix[i][line_len-1] = first_line[i]-'a';
    for (int j = 1; j < line_len; j++) {
        getline(std::cin, input_line);
        for (int i = 0; i < line_len; i++) {
            matrix[i][line_len-1-j] = input_line[i]-'a';
        }
    }
    getline(std::cin, input_line);
    std::stringstream ss;
    ss.str(input_line);
    while (ss >> temp) {
        word_n += 1;
        //all_letter_n += temp.size();
        letter_n.push_back(temp.size());
        hint.push_back(temp);
    }
    mat_info.matrix = matrix;
    mat_info.dimension = line_len;
    mat_info.word_n = word_n;
    mat_info.letter_n = letter_n;
    //mat_info.all_letter_n = all_letter_n;
    mat_info.hint = hint;
    if (std::cin.eof())
        ENDFILE = 1;
    return mat_info;
}

int** copy_matrix(data mat_info) {
    int dim = mat_info.dimension;
    int** new_matrix = new int*[dim];
    for (int i = 0; i < dim; i++) {
        int* p = new int[dim];
        for (int j = 0; j < dim; j++)
            p[j] = mat_info.matrix[i][j];
        new_matrix[i] = p;
    }
    return new_matrix;
}
bool* look_up(trie_node* &node_ptr, char letter) {
    bool* found = new bool[2];
    found[0] = 0;
    found[1] = 0;
    node_ptr = node_ptr->children[letter];
    //std::cout << "in look_up: " << node_ptr << std::endl; 
    if (!node_ptr){
        return found;
    }
    found[1] = 1;
    if (node_ptr->isword)
        found[0] = 1;
    return found;
}

void find_next_word(trie_node* root, trie_node* node, data mat_info, recur recur_data);

void find_all_words(trie_node* root, data mat_info, recur recur_data) {
    for (int i = 0; i < mat_info.dimension; i++) {
        for (int j = 0; j < mat_info.dimension; j++) {
            if (mat_info.matrix[i][j] == -1)
                continue;
            if (mat_info.matrix[i][j]+'a' != mat_info.hint.at(recur_data.nth_word)[0] && mat_info.hint.at(recur_data.nth_word)[0] != '*')
                continue;
            recur new_recur_data = recur_data;
            new_recur_data.layer = 0;
            new_recur_data.start_coord[0] = i;
            new_recur_data.start_coord[1] = j;
            data mat_info_cpy = mat_info;
            mat_info_cpy.matrix = copy_matrix(mat_info);
            find_next_word(root, root, mat_info_cpy, new_recur_data);
        }
    }
}

void find_next_word(trie_node* root, trie_node* node, data mat_info, recur recur_data) {
    trie_node* node_cpy = node;
    //std::cout << recur_data.layer << std::endl;
    //std::cout << "Before " << node_cpy << std::endl;
    int x = recur_data.start_coord[0], y = recur_data.start_coord[1];
    bool* flags = look_up(node_cpy, mat_info.matrix[x][y]);
    //std::cout << "After " << node_cpy << std::endl;
    if (!flags[1]){
        return;
    }
    recur_data.word.push_back(mat_info.matrix[x][y]+'a');
    //std::cout << recur_data.word << std::endl;
    //std::cout << recur_data.layer << std::endl;
    mat_info.matrix[x][y] = -1;
    /*
    for (int j = mat_info.dimension-1; j >= 0; j--) {
        for (int i = 0; i < mat_info.dimension; i++)
            std::cout << mat_info.matrix[i][j] << " ";
        std::cout << std::endl;
    }
    */
    if (recur_data.layer == mat_info.letter_n.at(recur_data.nth_word)-1) {
        if (!flags[0])
            return;
        //std::cout << flags[0] << std::endl;
        recur_data.word.push_back(' ');
        recur_data.nth_word += 1;
        if (recur_data.nth_word == mat_info.word_n && recur_data.word != "") {
            std::cout << recur_data.word << std::endl;
            //std::cout << recur_data.layer << std::endl;
            FOUNDWORD = 1;
            return;
        }
        drop_letters_matrix(mat_info.matrix, mat_info.dimension);
        find_all_words(root, mat_info, recur_data);
        //return;
    }
    else {
        for (int i = -1; i < 2; i++) {
            if (x+i < 0 || x+i >= mat_info.dimension)
                continue;
            for (int j = -1; j < 2; j++) {
                if (y+j < 0 || y+j >= mat_info.dimension)
                    continue;
                if (mat_info.matrix[x+i][y+j] == -1)
                    continue;
                data new_info = mat_info;
                new_info.matrix = copy_matrix(mat_info);
                recur new_recur_data;
                new_recur_data.nth_word = recur_data.nth_word;
                new_recur_data.word = recur_data.word;
                new_recur_data.layer = recur_data.layer+1;
                new_recur_data.start_coord[0] = x+i;
                new_recur_data.start_coord[1] = y+j;
                find_next_word(root, node_cpy, new_info, new_recur_data);
            }
        }
    }
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
    drop_letters_column(a, 9);
    //for (int i = 0; i < 9; i++)
    //    std::cout << a[i];
    //std::cout << std::endl;
    data info = get_letter_matrix();
    //for (int j = info.dimension-1; j >= 0; j--) {
    //    for (int i = 0; i < info.dimension; i++)
    //        std::cout << info.matrix[i][j] << " ";
    //    std::cout << std::endl;
    //}
    //std::cout << info.word_n << std::endl;
    //std::cout << info.all_letter_n << std::endl;
    //for (int i = 0; i < info.letter_n.size(); i++)
    //    std::cout << info.letter_n.at(i) << " ";
    //std::cout << std::endl;
    //std::cout << info.hint << std::endl;
    //drop_letters_matrix(info.matrix, info.dimension);

    //for (int j = info.dimension-1; j >= 0; j--) {
    //    for (int i = 0; i < info.dimension; i++)
    //        std::cout << info.matrix[i][j] << " ";
    //    std::cout << std::endl;
    //}
    recur recur_data;
    recur_data.word = "";
    recur_data.layer = 0;
    recur_data.start_coord[0] = 0;
    recur_data.start_coord[1] = 0;
    recur_data.nth_word = 0;
    data info_cpy = info;
    info_cpy.matrix = copy_matrix(info);
    //find_next_word(root, node, info_cpy, recur_data);
    /*
    02 12 22
    01 11 21
    00 10 20
    */
}

int main(int argc, char* argv[]) {
    if (DEBUG) {
        debug();
        //return 0;
    }
    else {
        std::string line;
        trie_node* root_small = add_node();
        trie_node* root_large = add_node();
        std::ifstream word_pool_small(argv[1]), word_pool_large(argv[2]);
        while (getline(word_pool_small, line))
            add_word(root_small, line);
        while (getline(word_pool_large, line))
            add_word(root_large, line);
        while (1) {
            data info = get_letter_matrix();
            /*
            for (int j = info.dimension-1; j >= 0; j--) {
                for (int i = 0; i < info.dimension; i++)
                    std::cout << info.matrix[i][j] << "\t";
                std::cout << std::endl;
            }
            std::cout << std::endl;
            for (int j = 0; j < info.hint.size(); j++)
                std::cout << info.hint.at(j) << std::endl;
            */
            recur recur_data;
            recur_data.word = "";
            recur_data.layer = 0;
            recur_data.start_coord[0] = 0;
            recur_data.start_coord[1] = 0;
            recur_data.nth_word = 0;
            find_all_words(root_small, info, recur_data);
            if (!FOUNDWORD)
                find_all_words(root_large, info, recur_data);
            FOUNDWORD = 0;
            if (ENDFILE)
                break;
            std::cout << '.' << std::endl;
        }
        /*
        std::cout << root_small->children[2]->isword << std::endl;
        std::cout << root_small->children[2]->children[0]->isword << std::endl;
        std::cout << root_small->children[2]->children[0]->children[19]->isword << std::endl;
        std::cout << root_small->children[2]->children[0]->children[19]->children[2]->isword << std::endl;
        std::cout << root_small->children[2]->children[0]->children[19]->children[2]->children[7]->isword << std::endl;
        */
    }
    return 0;
}