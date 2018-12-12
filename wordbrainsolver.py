# Copyright 2018 Chunshu Wu happycwu@bu.edu
'''EC602 Final HW'''
from sys import argv
import numpy as np

WORDPOOL_SMALL = argv[1]
WORDPOOL_LARGE = argv[2]
DEBUG = False


class Trie:
    "Trie tree"
    def __init__(self):
        self.root = {}
        self.isword = 'end'

    def add_word(self, word):
        "add a word to the trie"
        node = self.root
        for letter in word:

            # setdefault: node = node[letter], if letter does not exist,
            # set node[letter] = {} before the assignment
            node = node.setdefault(letter, {})

        # The value does not matter. As long as a node has .isword,
        # it will be recognized as a word
        node[self.isword] = None

    # find_word() returns 2 bool values. The first to check if the string is
    # on a branch, the second to check if the string is a word
    def find_word(self, word):
        "find a word in the trie"
        node = self.root
        for letter in word:
            if letter not in node:
                return False, False
            node = node[letter]
        return True, self.isword in node


def build_tree(trie_obj, word_pool):
    "Get the words from txt file and build a trie"
    words = open(word_pool).read().splitlines()
    for word in words:
        trie_obj.add_word(word)


def create_new_map(wmap, path_c):
    "the map after letters dropping"
    new_wmap = []
    for x_c in enumerate(wmap):
        index = []
        for y_c in enumerate(wmap[x_c[0]]):
            if (x_c[0], y_c[0]) in path_c:
                index.append(y_c[0])
        new_wmap.append(np.delete(wmap[x_c[0]], index))
    return new_wmap


def find_letter(node, letter):
    if letter not in node:
        return False, False
    return True, 'end' in node[letter]


# @profile
def peek_neighbors(root, node, wmap, path_l, path_c, depth, goal,
                   position, word_n, hint, res, result):
    "peek the neighbors"

    # Enter the next depth level of recursion
    depth += 1

    # Find current coordinates
    x_c = position[0]
    y_c = position[1]
    # If depth reaches the bottom
    # (e.g. the bottom of a 4-letter word is depth = 4)
    # tree.find_word() returns 2 bool values. The first to check if the
    # string is on a branch, the second to check if the string is a word
    if depth == goal and find_letter(node, path_l[depth-1])[1]:

        # Make a copy of res in case all the words (including
        # those don't have following solutions) are appended to res
        res = res.copy()
        res.append(path_l)

        # Number of words obtained
        word_n += 1
        if word_n < len(hint):
            new_wmap = create_new_map(wmap, path_c)

            # Double recursive, find_words calls peek_neighbors
            # and peek_neighbors calls find_words
            find_words(root, new_wmap, hint, word_n, res, result)
        else:

            # Get all possible full paths
            result.add(tuple(res))

    # If depth does not reach the bottom
    elif (depth < goal and find_letter(node, path_l[depth-1])[0]):
        node = node[path_l[depth-1]]

        # Iterate through all neighbors and find the available neighbors
        wmap_len = len(wmap)
        for i in [-1, 0, 1]:
            if (x_c+i < 0 or x_c+i >= wmap_len):
                continue
            col_len = len(wmap[x_c+i])
            for j in [-1, 0, 1]:
                if (y_c+j < 0 or y_c+j >= col_len or (x_c+i, y_c+j) in path_c):
                    continue
                if (wmap[x_c+i][y_c+j] != hint[word_n][depth]
                        and hint[word_n][depth] != '*'):
                    continue
                # if not tree.find_word(path_l)[0]:
                    # continue

                # For each available neighbor, append the corresponding letter
                # and coordinates to a copy of the previous paths
                new_path_l = path_l+wmap[x_c+i][y_c+j]
                new_path_c = path_c.copy()
                new_path_c.append((x_c+i, y_c+j))

                # Use the new paths and peek the next neighbors
                peek_neighbors(root, node, wmap, new_path_l, new_path_c, depth,
                               goal, (x_c+i, y_c+j), word_n, hint, res, result)

    # If depth reaches the bottom and it's not a word, do nothing


def find_words(root, wmap, hint, word_n, res, result):
    "find the words"
    # Iterate through all starting points
    for x_c in range(len(wmap)):
        for y_c in range(len(wmap[x_c])):
            if (wmap[x_c][y_c] != hint[word_n][0]
                    and hint[word_n][0] != '*'):
                continue

            # Path of letters, in string format. e.g. "a" -> "ab" -> "abc"
            path_l = wmap[x_c][y_c]

            # Path of coordinates, a list of tuples.
            # e.g. [(0, 0)] -> [(0, 0), (0, 1)]
            path_c = [(x_c, y_c)]

            # Labels the depth of recursions
            depth = 0
            node = root
            # Prints all possible words as the first word
            peek_neighbors(root, node, wmap, path_l, path_c, depth,
                           len(hint[word_n]), (x_c, y_c), word_n,
                           hint, res, result)


def main():
    "main"
    # Build two tries
    tree_small = Trie()
    build_tree(tree_small, WORDPOOL_SMALL)
    tree_large = Trie()
    build_tree(tree_large, WORDPOOL_LARGE)
    while True:
        try:
            # Get the 1st line of input
            result = set()
            lines = input()

            # The dimension of the word map can be obtained using the 1st line
            dimension = len(lines)

            # Start building the word map
            wmap = np.array([list(lines)])

            # Get the rest of the lines and build the word map
            i = 0
            while i < dimension-1:
                i += 1
                line = input()
                wmap = np.vstack((wmap, np.array([list(line)])))
            hint = input().split()

            # Finally get a word map like
            # [['c', 'b', 'a'], ['f', 'e', 'd'], ['i', 'h', 'g']]
            # a  d  g
            # b  e  h
            # c  f  i
            # This format should be convenient to use in letters dropping part
            wmap = wmap[::-1]
            wmap = np.transpose(wmap)

            # 3 is for testing
            word_n = 0
            res = []
            find_words(tree_small.root, wmap, hint, word_n, res, result)
            if result == set() and not DEBUG:
                find_words(tree_large.root, wmap, hint, word_n, res, result)
            result = list(result)
            result.sort()
            for string in result:
                print(" ".join(string))
            print('.')
        except EOFError:
            break


main()
