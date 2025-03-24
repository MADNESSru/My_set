#include <iostream>
#include <string>

using namespace std;

template<class T>
class MySet {
private:

    class Node
    {
    public:
        T key;
        Node* root;
        Node* left = nullptr, * right = nullptr;

        Node(T key = T(), Node* n = nullptr)
        {
            this->key = key;
            this->root = n;
        }
    };

    Node* root; //корневой элемент

    bool find(Node* n, T key)
    {
        while (n && n->key != key)
        {
            if (n->key > key)
                n = n->left;
            else
                n = n->right;
        }
        return n != nullptr;
    }

    void insert(Node* n, T key) 
    {
        if (!root) {
            root = new Node(key);
            return;
        }
        if (key == n->key)
            return;
        if (key < n->key) {
            if (n->left)
                insert(n->left, key);
            else
                n->left = new Node(key);
        }
        else {
            if (n->right)
                insert(n->right, key);
            else
                n->right = new Node(key);
        }
    }

    void print(Node* n) //DFS обход
    {
        if (!n)
            return;
        print(n->left);
        cout << n->key << " ";
        print(n->right);
    }

    int GetSize(Node* n) 
    {
        if (n == nullptr)
        {
            return 0;
        }
        int L = GetSize(n->left);
        int R = GetSize(n->right);
        return 1 + L + R;
    }

    void erase(Node* n, T key)
    {
        Node* parent = nullptr; //указатель на предка удаляемого элемента
        while (n && n->key != key)
        {
            parent = n;
            if (n->key > key)
                n = n->left;
            else
                n = n->right;
        }
        if (!n)
            return;
        if (n->left == nullptr) //если нет левого соседа, то подвешиваю правое поддерево
        {
            if (parent && parent->left == n)
                parent->left = n->right;
            if (parent && parent->right == n)
                parent->right = n->right;
            delete n;
            return;
        }
        if (n->right == nullptr) //если нет правого соседа, то подвешиваю левое поддерево
        {
            if (parent && parent->left == n)
                parent->left = n->left;
            if (parent && parent->right == n)
                parent->right = n->left;
            delete n;
            return;    
        }
        Node* replace = n->right; //если есть два потомка, тогда на место элемента ставлю наименьшее из его правого поддерева                           // наименьший элемент из его правого поддерева
        while (replace->left)
            replace = replace->left;
        T replace_value = replace->key;
        erase(replace_value);
        n->key = replace_value;
    }

    void destroy(Node* n) //освобождение динамической памяти
    {
        if (!n)
            return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

public:

    MySet() //конструктор
    {
        root = nullptr;
    }

    bool find(T key) //проверка на наличие элемента
    {
        return find(root, key);
    }

    void insert(T key) //вставка
    {
        insert(root, key);
    }

    void print() //вывод коллекции
    {
        print(root);
    }

    int GetSize() //получение длины контейнера
    {
        return GetSize(root);
    }

    void erase(T key) //удаление
    {
        erase(root, key);
    }

    ~MySet() //деструктор
    {
        destroy(root);
    }

};

int main()
{
    MySet<string> set; //изменить <тип> при других тестах!
    
    //тест на проверку с числами
    /*while (true) {
        int elem;
        cin >> elem;

        if (elem==0)
            break;

        set.insert(elem);
    }

    set.print();

    cout << '\n' << set.GetSize();
    */
    
    //тест на проверку строками
    set.insert("Hello");
    set.erase("Pablo");
    set.insert("Hi");
    set.insert("Amigo");
    set.erase("Hi");
    set.print();
    cout << endl << set.GetSize();
    cout << endl << set.find("Ami");

    
}