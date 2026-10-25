#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Part 1: Min-heap backed by a std::vector.
class MinHeap {
private:
    std::vector<int> data;

    static std::size_t parent(std::size_t index) {
        return (index - 1) / 2;
    }

    static std::size_t leftChild(std::size_t index) {
        return 2 * index + 1;
    }

    static std::size_t rightChild(std::size_t index) {
        return 2 * index + 2;
    }

    void heapifyUp(std::size_t index) {
        while (index > 0 && data[index] < data[parent(index)]) {
            std::swap(data[index], data[parent(index)]);
            index = parent(index);
        }
    }

    void heapifyDown(std::size_t index) {
        while (true) {
            std::size_t smallest = index;
            std::size_t left = leftChild(index);
            std::size_t right = rightChild(index);

            if (left < data.size() && data[left] < data[smallest]) {
                smallest = left;
            }

            if (right < data.size() && data[right] < data[smallest]) {
                smallest = right;
            }

            if (smallest == index) {
                break;
            }

            std::swap(data[index], data[smallest]);
            index = smallest;
        }
    }

public:
    void insert(int value) {
        data.push_back(value);
        heapifyUp(data.size() - 1);
    }

    int peekMin() const {
        if (data.empty()) {
            throw std::runtime_error("Cannot peek an empty heap");
        }

        return data.front();
    }

    int removeMin() {
        if (data.empty()) {
            throw std::runtime_error("Cannot remove from an empty heap");
        }

        int result = data.front();

        data.front() = data.back();
        data.pop_back();

        if (!data.empty()) {
            heapifyDown(0);
        }

        return result;
    }

    const std::vector<int>& array() const {
        return data;
    }
};

void printHeap(const MinHeap& heap, const std::string& label) {
    std::cout << label << ": ";

    for (std::size_t i = 0; i < heap.array().size(); ++i) {
        if (i > 0) {
            std::cout << ' ';
        }

        std::cout << heap.array()[i];
    }

    std::cout << '\n';
}

// Part 2: Treap using min-priority heap order.
struct TreapNode {
    int key;
    int priority;
    TreapNode* left;
    TreapNode* right;

    TreapNode(int keyValue, int priorityValue)
        : key(keyValue),
          priority(priorityValue),
          left(NULL),
          right(NULL) {}
};

class Treap {
private:
    TreapNode* root;

    static TreapNode* rotateRight(TreapNode* node) {
        TreapNode* newRoot = node->left;

        node->left = newRoot->right;
        newRoot->right = node;

        return newRoot;
    }

    static TreapNode* rotateLeft(TreapNode* node) {
        TreapNode* newRoot = node->right;

        node->right = newRoot->left;
        newRoot->left = node;

        return newRoot;
    }

    static TreapNode* insert(
        TreapNode* node,
        int key,
        int priority
    ) {
        if (node == NULL) {
            return new TreapNode(key, priority);
        }

        if (key == node->key) {
            return node;
        }

        if (key < node->key) {
            node->left = insert(node->left, key, priority);

            if (node->left->priority < node->priority) {
                node = rotateRight(node);
            }
        }
        else {
            node->right = insert(node->right, key, priority);

            if (node->right->priority < node->priority) {
                node = rotateLeft(node);
            }
        }

        return node;
    }

    static TreapNode* search(TreapNode* node, int key) {
        if (node == NULL || node->key == key) {
            return node;
        }

        if (key < node->key) {
            return search(node->left, key);
        }

        return search(node->right, key);
    }

    static void printInOrder(TreapNode* node) {
        if (node == NULL) {
            return;
        }

        printInOrder(node->left);

        std::cout << "("
                  << node->key
                  << ", p="
                  << node->priority
                  << ") ";

        printInOrder(node->right);
    }

    static void destroy(TreapNode* node) {
        if (node == NULL) {
            return;
        }

        destroy(node->left);
        destroy(node->right);

        delete node;
    }

public:
    Treap() : root(NULL) {}

    ~Treap() {
        destroy(root);
    }

    void insert(int key, int priority) {
        root = insert(root, key, priority);
    }

    bool search(int key) const {
        return search(root, key) != NULL;
    }

    void printInOrder() const {
        printInOrder(root);
        std::cout << '\n';
    }
};

int main() {
    // Test the min-heap with 10 integers.
    MinHeap heap;

    int values[] = {
        42, 17, 8, 99, 23,
        4, 31, 12, 55, 1
    };

    for (int value : values) {
        heap.insert(value);
    }

    printHeap(heap, "Heap after 10 inserts");

    std::cout << "peekMin: "
              << heap.peekMin()
              << '\n';

    std::cout << "removeMin: "
              << heap.removeMin()
              << '\n';

    printHeap(heap, "Heap after one removeMin");

    heap.insert(2);

    printHeap(heap, "Heap after inserting 2");

    std::cout << "removeMin: "
              << heap.removeMin()
              << '\n';

    printHeap(heap, "Heap after second removeMin");

    // Test the treap with 10 key-priority pairs.
    Treap treap;

    int pairs[][2] = {
        {50, 40},
        {30, 20},
        {70, 60},
        {20, 10},
        {40, 35},
        {60, 50},
        {80, 15},
        {10, 5},
        {25, 30},
        {65, 25}
    };

    for (const auto& pair : pairs) {
        treap.insert(pair[0], pair[1]);
    }

    std::cout << "Treap in-order traversal "
              << "(key, priority):\n";

    treap.printInOrder();

    std::cout << "Search 65: "
              << (treap.search(65) ? "found" : "not found")
              << '\n';

    std::cout << "Search 99: "
              << (treap.search(99) ? "found" : "not found")
              << '\n';

    return 0;
}