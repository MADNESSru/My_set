#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm> // для std::max
// для замеров скорости
#include <set>
#include <chrono>
#include <random>
#include <vector>

template <typename T>
class MySet {
private:
    struct Node {
        T key;
        Node* left = nullptr;
        Node* right = nullptr;

        Node(const T& key) : key(key) {}
    };

    Node* root = nullptr;

    bool find(Node* node, const T& key) const {
        while (node) {
            if (key == node->key) return true;
            node = (key < node->key) ? node->left : node->right;
        }
        return false;
    }

    Node* insert(Node* node, const T& key) {
        if (!node) return new Node(key);
        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        return node;
    }

    Node* erase(Node* node, const T& key) {
        if (!node) return nullptr;

        if (key < node->key) {
            node->left = erase(node->left, key);
        } else if (key > node->key) {
            node->right = erase(node->right, key);
        } else {
            if (!node->left) {
                Node* right = node->right;
                delete node;
                return right;
            }
            if (!node->right) {
                Node* left = node->left;
                delete node;
                return left;
            }
            Node* minRight = node->right;
            while (minRight->left) minRight = minRight->left;
            node->key = minRight->key;
            node->right = erase(node->right, minRight->key);
        }
        return node;
    }

    void print(Node* node) const {
        if (!node) return;
        print(node->left);
        std::cout << node->key << " ";
        print(node->right);
    }

    int getSize(Node* node) const {
        if (!node) return 0;
        return 1 + getSize(node->left) + getSize(node->right);
    }
    //полный снос дерева для деструктора
    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    // Высота дерева 
    int height(Node* node) const {
        if (!node) return 0;
        return 1 + std::max(height(node->left), height(node->right));
    }

public:
    MySet() = default;

    ~MySet() {
        destroy(root);
    }

    void insert(const T& key) {
        root = insert(root, key);
    }
    //удаление элемента
    void erase(const T& key) {
        root = erase(root, key);
    }

    bool find(const T& key) const {
        return find(root, key);
    }

    void print() const {
        print(root);
        std::cout << std::endl;
    }

    int getSize() const {
        return getSize(root);
    }

    // Очистка множества
    void clear() {
        destroy(root);
        root = nullptr;
    }

    // Минимальный элемент
    T min() const {
        if (!root) throw std::runtime_error("Set is empty");
        Node* node = root;
        while (node->left) node = node->left;
        return node->key;
    }

    // Максимальный элемент
    T max() const {
        if (!root) throw std::runtime_error("Set is empty");
        Node* node = root;
        while (node->right) node = node->right;
        return node->key;
    }

    // Высота дерева
    int height() const {
        return height(root);
    }
};

int main() {

    //тесты int, string, char
    
    MySet<int> intSet;
    intSet.insert(50);
    intSet.insert(30);
    intSet.insert(70);
    intSet.insert(20);
    intSet.insert(40);
    intSet.insert(60);
    intSet.insert(80);

    std::cout << "Вывод [int] элементов: ";
    intSet.print(); // 20 30 40 50 60 70 80

    std::cout << "[int] Поиск 60: " << std::boolalpha << intSet.find(60) << std::endl; //true
    std::cout << "[int] Размер: " << intSet.getSize() << std::endl;
    std::cout << "[int] Минимум: " << intSet.min() << std::endl;
    std::cout << "[int] Максимум: " << intSet.max() << std::endl;
    std::cout << "[int] Высота: " << intSet.height() << std::endl;

    intSet.erase(30);
    std::cout << "Set после удаления 30: ";
    intSet.print();

    intSet.clear();
    std::cout << "Размер после clear: " << intSet.getSize() << std::endl;


    MySet<std::string> stringSet;
    stringSet.insert("Darius");
    stringSet.insert("Madness");
    stringSet.insert("I_<3_C++");
    stringSet.insert("Hello");

    std::cout << "\nВывод [string] элементов: ";
    stringSet.print(); //

    std::cout << "[string] Поиск 'Darius': " << std::boolalpha << stringSet.find("Darius") << std::endl; //true
    std::cout << "[string] Минимум: " << stringSet.min() << std::endl;   // 
    std::cout << "[string] Максимум: " << stringSet.max() << std::endl; // 
    std::cout << "[string] Размер: " << stringSet.getSize() << std::endl;
    std::cout << "[string] Высота: " << stringSet.height() << std::endl;

    stringSet.erase("Hello");
    std::cout << "Set после удаления 'Hello': ";
    stringSet.print();


    MySet<char> charSet;
    charSet.insert('d');
    charSet.insert('a');
    charSet.insert('c');
    charSet.insert('b');
    charSet.insert('e');

    std::cout << "\nВывод [char] элементов: ";
    charSet.print(); // a b c d e

    std::cout << "[char] Поиск 'c': " << std::boolalpha << charSet.find('v') << std::endl; //false
    std::cout << "[char] Минимум: " << charSet.min() << std::endl; // a
    std::cout << "[char] Максимум: " << charSet.max() << std::endl; // e
    std::cout << "[char] Размер: " << charSet.getSize() << std::endl;
    std::cout << "[char] Высота: " << charSet.height() << std::endl;

    charSet.erase('d');
    std::cout << "Set после удаления 'd': ";
    charSet.print();
    
    // тесты на скорость
    std::cout << "\n";
    
    const int N = 100000; // Количество элементов
    std::vector<int> data;
    data.reserve(N);

    // случайные числа
    std::mt19937 gen(42); // seed
    std::uniform_int_distribution<> dist(1, N * 10);
    for (int i = 0; i < N; ++i) {
        data.push_back(dist(gen));
    }

    // std::set 
    std::set<int> stdSet;
    auto start = std::chrono::high_resolution_clock::now();
    for (int x : data) stdSet.insert(x);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "[std::set] insert: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    for (int x : data) stdSet.find(x);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[std::set] find: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    for (int x : data) stdSet.erase(x);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[std::set] erase: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    // MySet 
    MySet<int> mySet;
    start = std::chrono::high_resolution_clock::now();
    for (int x : data) mySet.insert(x);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[MySet] insert: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    for (int x : data) mySet.find(x);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[MySet] find: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    start = std::chrono::high_resolution_clock::now();
    for (int x : data) mySet.erase(x);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[MySet] erase: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}