#include <functional>
#include <iostream>
#include <map>
#include <sstream>

template<class Key, class T>
struct Node {
    using value_type = std::pair<const Key, T>;

    value_type data;
    Node<Key, T>* parent;
    Node<Key, T>* left;
    Node<Key, T>* right;
    unsigned char height; // Высота авл дерева при n узлах равна примерно log(2, n), поэтому одного байта хватит с головой
    
    Node(Key k, T value, Node<Key, T>* parent_node): data{k, value}, parent{parent_node}, left{nullptr}, right{nullptr}, height{1} {}
};

template <class T>
struct DumbAllocator{
    using value_type = T;

    DumbAllocator() = default;
    template<class U>
    constexpr DumbAllocator(const DumbAllocator<U>&) {}

    T* allocate(size_t n){
        T* ptr = static_cast<T*>(::operator new[](sizeof(value_type) * n));
        return ptr;
    }

    void deallocate(T* p, size_t n){
        if(n == 0){
            return;
        }
        ::operator delete[](p);
    }
};

template <class T>
struct BidirectionalIterator : std::iterator<std::bidirectional_iterator_tag, T>{
    using reference = T&;
    using pointer = T*;
    using value_type = T;

    BidirectionalIterator(T* ptr): node{ptr}{}

    reference operator*() const {
        return *node;
    }

    BidirectionalIterator& operator++(){
        if (node->right != 0) 
        {
            node = node->right;
            while (node->left != 0){
                node = node->left;
            }
        }
        else 
        {
            value_type* tmp = node->parent;
            while (node == tmp->right) 
            {
                node = tmp;
                tmp = tmp->parent;
            }
            if (node->right != tmp){
                node = tmp;
            }
        }
        return *this;
    }

    BidirectionalIterator& operator--(){
        if (node->left != 0) 
        {
            node = node->left;
            while (node->right != 0){
                node = node->right;
            }
        }
        else 
        {
            value_type* tmp = node->parent;
            while (node == tmp->left) 
            {
                node = tmp;
                tmp = tmp->parent;
            }
            if (node->left != tmp){
                node = tmp;
            }
        }
        return *this;
    }

    bool operator==(const BidirectionalIterator& to_compare) const {
        return node == to_compare.node;
    }

    bool operator!=(const BidirectionalIterator& to_compare) const {
        return node != to_compare.node;
    }

    T* node;
};


template <class T>
struct ReversedBidirectionalIterator : std::iterator<std::bidirectional_iterator_tag, T>{
    using reference = T&;
    using pointer = T*;
    using value_type = T;

    ReversedBidirectionalIterator(T* ptr): node{ptr}{}

    reference operator*() const {
        return *node;
    }

    ReversedBidirectionalIterator& operator--(){
        if (node->right != 0) 
        {
            node = node->right;
            while (node->left != 0){
                node = node->left;
            }
        }
        else 
        {
            value_type* tmp = node->parent;
            while (node == tmp->right) 
            {
                node = tmp;
                tmp = tmp->parent;
            }
            if (node->right != tmp){
                node = tmp;
            }
        }
        return *this;
    }

    ReversedBidirectionalIterator& operator++(){
        if (node->left != 0) 
        {
            node = node->left;
            while (node->right != 0){
                node = node->right;
            }
        }
        else 
        {
            value_type* tmp = node->parent;
            while (node == tmp->left) 
            {
                node = tmp;
                tmp = tmp->parent;
            }
            if (node->left != tmp){
                node = tmp;
            }
        }
        return *this;
    }

    bool operator==(const ReversedBidirectionalIterator& to_compare){
        return node == to_compare.node;
    }

    bool operator!=(const ReversedBidirectionalIterator& to_compare){
        return node != to_compare.node;
    }

    T* node;
};


template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>
> 
class AVL {

    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node<Key, T>>;
    using AllocTraits = std::allocator_traits<NodeAlloc>;
    using iterator = BidirectionalIterator<Node<Key, T>>;
    using reversed_iterator = ReversedBidirectionalIterator<Node<Key, T>>;

private:
    NodeAlloc alloc;
    Node<Key, T>* root;
    Node<Key, T>* end_node; // Фейковая нода, следующая за последним элементом в end()
    Node<Key, T>* start_node; // Фейковая нода, следующая за последним элементом в rend()

    // Рекурсивный метод вычисления размера контейнера
    size_t size(Node<Key, T>* node) const {
        if((!node->right && !node->left) || node->right == end_node || node->left == start_node){
            return 1;
        } else {
            size_t size_left = 0;
            size_t size_right = 0;
            if(node->left){
                size_left = size(node->left);
            }
            if(node->right){
                size_right = size(node->right);
            }

            return size_left + size_right + 1;
        }
    }

    // Рекурсивный метод поиска элемента 
    Node<Key, T>* find( const Key& key, Node<Key, T>* p ) const {
        if(!p || p == end_node || p == start_node){
            return nullptr;
        }
        if(key < p->data.first) {
            return find(key, p->left);
        } else if( key > p->data.first ) {
            return find(key, p->right);
        } else {
            return p;
        }
    }

    // Геттер для высоты дерева. Необходим для правильной обработки nullptr узлов
    unsigned char get_height(Node<Key, T>* node){
        if(node){
            return node->height;
        } else {
            return 0;
        }
    }

    // Метод вычисления разности между высотой правого и левого поддерева 
    int get_balance_factor(Node<Key, T>* node){
        return get_height(node->right) - get_height(node->left);
    }

    // Метод пересчета высоты дерева в ноде
    void refresh_height(Node<Key, T>* node){
        unsigned char h_l = get_height(node->left);
        unsigned char h_r = get_height(node->right);
        if(h_l > h_r){
            node->height = h_l + 1;
        } else {
            node->height = h_r + 1;
        }
    }

    // Метод правого поворота
    Node<Key, T>* rotate_right(Node<Key, T>* node){
        Node<Key, T>* q = node->left;
        q->parent = node->parent;

        // Перевешиваем правое поддерево ноды q на ноду node
        node->left = q->right;
        if(node->left){node->left->parent = node;}

        // Перевешиваем на правое поддерево q ноду node
        q->right = node;
        node->parent = q;

        refresh_height(node);
        refresh_height(q);
        return q;
    }

    // Метод левого поворота
    Node<Key, T>* rotate_left(Node<Key, T>* node){
        Node<Key, T>* p = node->right;
        p->parent = node->parent;

        node->right = p->left;
        if(node->right){node->right->parent = node;}

        p->left = node;
        node->parent = p;

        refresh_height(node);
        refresh_height(p);
        return p;
    }

    // Поиск минимального значения в дереве
    Node<Key, T>* find_min(Node<Key, T>* node){
        if(node->left && (node->left != start_node)){
            return find_min(node->left);
        } else {
            return node;
        }
    }

    // Поиск максимального значения в дереве
    Node<Key, T>* find_max(Node<Key, T>* node){
        if(node->right && (node->right != end_node)){
            return find_max(node->right);
        } else {
            return node;
        }
    }

    // Корректировка дерева после удаления ноды
    Node<Key, T>* correct_remove(Node<Key, T>* node){
        if( node->left==0 ){
		    return node->right;
        }
        node->left = correct_remove(node->left);
        if(node->left){node->left->parent = node;}
        return balance(node);
    }

    // Рекурсивный метод удаления ноды
    Node<Key, T>* remove(Node<Key, T>* p, Key k)
    {   
        if( !p || (p == end_node) || (p == start_node)) {
            return 0;
        }

        if(k < p->data.first) {
            p->left = remove(p->left,k);
            if(p->left){p->left->parent = p;}
        } else if( k > p->data.first ) {
            p->right = remove(p->right,k);
            if(p->right){p->right->parent = p;}
        } else {
            Node<Key, T>* q = p->left;
            Node<Key, T>* r = p->right;

            AllocTraits::destroy(alloc, p);
            AllocTraits::deallocate(alloc, p, 1);
            
            if( !r ) return q;
            
            Node<Key, T>* min = find_min(r);
            min->right = correct_remove(r);
            if(min->right){min->right->parent = min;}

            min->left = q;
            if(min->left){min->left->parent = min;}
            
            return balance(min);
        }
        return balance(p);
    }

    // Метод перебалансировки дерева
    Node<Key, T>* balance(Node<Key, T>* node){
        refresh_height(node);
      
        if(get_balance_factor(node) == 2){
            if(get_balance_factor(node->right) >= 0){ 
                // Малый левый поворот
                node = rotate_left(node);
                return node;
            } else { 
                // Большой левый поворот
                node->right = rotate_right(node->right);
                return rotate_left(node);
            }
        }
        if(get_balance_factor(node) == -2){
            if(get_balance_factor(node->left) < 0){ 
                // Малый правый поворот
                return rotate_right(node); 
            } else { 
                // Большой правый поворот
                node->left = rotate_left(node->left);
                return rotate_right(node);
            }
        }
        return node; // Дерево и так сбалансировано
    }

    // Рекурсивный метод вставки новой ноды
    Node<Key, T>* insert(Node<Key, T>* current_node, Node<Key, T>* parent_node, Key k, T v){
        if(!current_node) {
            Node<Key, T>* ptr = AllocTraits::allocate(alloc, 1);
            AllocTraits::construct(alloc, ptr, k, v, parent_node);
            return ptr;
        } else if(current_node == end_node){
            Node<Key, T>* ptr = AllocTraits::allocate(alloc, 1);
            AllocTraits::construct(alloc, ptr, k, v, parent_node);
            ptr->right = end_node; // Если вставляемый элемент больше максимального, тогда обновляем end_node
            end_node->parent = ptr;
            return ptr;
        } else if(current_node == start_node){
            Node<Key, T>* ptr = AllocTraits::allocate(alloc, 1);
            AllocTraits::construct(alloc, ptr, k, v, parent_node);
            ptr->left = start_node; // Если вставляемый элемент больше максимального, тогда обновляем end_node
            start_node->parent = ptr;
            return ptr;
        }
        if( k < current_node->data.first ){
            current_node->left = insert(current_node->left, current_node, k, v);
        } else {
            current_node->right = insert(current_node->right, current_node, k, v);
        }
        
        return balance(current_node);
    }

    // Рекурсивный метод очистки дерева
    void clear(Node<Key, T>* node){
        if(!node->right && !node->left){
            if(node != end_node && node != start_node){
                AllocTraits::destroy(alloc, node);
                AllocTraits::deallocate(alloc, node, 1);
            }
        } else {
            if(node->right){clear(node->right);}
            if(node->left){clear(node->left);}
            AllocTraits::destroy(alloc, node);
            AllocTraits::deallocate(alloc, node, 1);
        } 
    }

public:
    // Конструктор инициализации через список инициализации
    AVL(std::initializer_list<std::pair<const Key, T>> init) {

        auto it = init.begin();
        auto end = init.end();

        root = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, root, it->first, it->second, nullptr);

        end_node = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, end_node, it->first, it->second, root);
        end_node->height = 0;
        root->right = end_node;

        start_node = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, start_node, it->first, it->second, root);
        start_node->height = 0;
        root->left = start_node;
        
        ++it;

        for(; it != end; ++it){
            root = insert(root, root, it->first, it->second);
        }

    }

    // Дефолтный конструктор
    AVL(): root{nullptr} {
        end_node = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, end_node, Key(), T(), nullptr);
        end_node->height = 0;

        start_node = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, start_node, Key(), T(), nullptr);
        start_node->height = 0;
    }

    ~AVL(){
        // Очистка дерева
        clear();
        // Удаление фейковых нод
        AllocTraits::destroy(alloc, end_node);
        AllocTraits::deallocate(alloc, end_node, 1);
        AllocTraits::destroy(alloc, start_node);
        AllocTraits::deallocate(alloc, start_node, 1);
    }

    // Обертка над рекурсивным методом size
    size_t size() const {
        if(root){
            return size(root);
        } else {
            return 0;
        }
    }

    bool empty() const {
        return size() == 0;
    }

    T& operator[](Key&& key) {
        auto found = find(key);
        // Проверка, что элемент найден. Иначе инициализируем новый узел
        if(&(*found) != end_node){
            return (*found).data.second;
        } else {
            insert(std::pair<Key, T>(key, T()));
            return (*find(key)).data.second;
        }
    }

    T& at(const Key& key) const {
        auto found = find(key);
        if(&(*found) != end_node){
            return (*found).data.second;
        } else {
            throw std::out_of_range("Key not found");
        }
    }

    // Обертка над рекурсивным методом поиска элемента
    iterator find( const Key& key ) const {
        auto found = find(key, root);
        if(found){
            return iterator(found);
        } else {
            return iterator(end_node);
        }
    }

    bool contains(const Key& key) const {
        return &(*find(key)) != end_node;
    }

    // Метод для тестирования структуры дерева
    std::stringstream& print(){
        std::stringstream tree_struct;
        //std::cout << "=======================================" <<std::endl;
        return print(root, tree_struct);
    }

    std::stringstream& print(Node<Key, T>* node, std::stringstream& tree_struct){
        if(node){
            if(node && node != end_node && node != start_node){
                //std::cout << "Parent node is " << node->data.first <<std::endl;
                tree_struct << node->data.first;
                
                if(node->left){
                    //std::cout << "Left node is " << node->left->data.first << " ";
                    //tree_struct << node->left->data.first;
                }
                if(node->right){
                    //std::cout << "Right node is " << node->right->data.first <<std::endl;
                    //tree_struct << node->right->data.first;
                }
                //std::cout << "-------------------------------" <<std::endl;
                
                if(node->left){
                    print(node->left, tree_struct);
                }
                if(node->right){
                    print(node->right, tree_struct);
                }
            }
        }

        return tree_struct;
    }

    void erase(const Key& key){
        if(root){
            root = remove(root, key);  
        }
    }
    
    // Обертка над рекурсивным методом insert
    void insert(std::pair<Key, T>&& to_insert){
        if(root){
            root = insert(root, root, to_insert.first, to_insert.second);
        } else {
            root = AllocTraits::allocate(alloc, 1);
            AllocTraits::construct(alloc, root, to_insert.first, to_insert.second, nullptr);
            end_node->parent = root;
            root->right = end_node;

            start_node->parent = root;
            root->left = start_node;
        }
    }

    // Обертка над рекурсивным методом очистки дерева
    void clear(){
        if(root){
            clear(root);
            root = nullptr;
            end_node->parent = nullptr;
        }
    }

    iterator begin() noexcept{
        if(empty()){
            return end();
        }
        return iterator(find_min(root));
    }

    iterator end() noexcept{
        return iterator(end_node);
    }  
    
    reversed_iterator rbegin() noexcept{
        if(empty()){
            return rend();
        }
        return reversed_iterator(find_max(root));
    }

    reversed_iterator rend() noexcept{
        return reversed_iterator(start_node);
    }
    
};
