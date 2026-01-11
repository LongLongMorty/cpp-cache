//
// Created by Morty on 2026-01-11.
//
struct Node {
    int key, value;
    Node *pre, *next;
    Node() : key(0), value(0), pre(nullptr), next(nullptr) {}
    Node(int _key, int _val)
            : key(_key), value(_val), pre(nullptr), next(nullptr) {}
};
class LRUCache {
public:
    LRUCache(int _capacity):capacity(_capacity),size(0){
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->pre = head;
    }
    ~LRUCache(){
        delete head;
        delete tail;
    }
    int get(int key) {
        if(!cache.count(key)){
            return -1;
        }
        Node* node = cache[key].get();
        remove(node);
        addToHead(node);
        return node->value;
    }

    void put(int key, int value) {
        if(cache.count(key)){
            Node* node = cache[key].get();
            node->value = value;
            remove(node);
            addToHead(node);
        }else{
            if(size == capacity){
                Node* old = tail->pre;
                remove(old);
                cache.erase(old->key);
                size--;
            }
            unique_ptr<Node> new_node = make_unique<Node>(key,value);
            Node* raw = new_node.get();
            addToHead(raw);
            cache[key] = move(new_node);
            size++;
        }
    }
private:
    int capacity;
    int size;
    Node* head;
    Node* tail;
    unordered_map<int, unique_ptr<Node>> cache;
    void remove(Node* node) {
        node->pre->next = node->next;
        node->next->pre = node->pre;
    }
    void addToHead(Node* node) {
        node->next = head->next;
        node->pre  = head;
        node->next->pre = node;
        head->next = node;
    }
};

