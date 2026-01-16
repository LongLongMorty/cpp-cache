//
// Created by Morty on 2026-01-16.
//
// 虚拟节点
struct DummyNode {
    DummyNode* pre;
    DummyNode* next;
    DummyNode() : pre(nullptr), next(nullptr) {}
};
// 业务节点
struct Node : public DummyNode {
    int key;
    int value;
    int freq;
    Node() : key(0), value(0), freq(1) {}
    Node(int k, int v) : key(k), value(v), freq(1) {}
};
struct FreqList {
    unique_ptr<DummyNode> head;
    unique_ptr<DummyNode> tail;  //unique_ptr 安全性高
    FreqList() {
        head = make_unique<DummyNode>();
        tail = make_unique<DummyNode>();
        head->next = tail.get();
        tail->pre = head.get();
    }
    bool empty() const { return head->next == tail.get(); }
    void addToHead(DummyNode* node) {
        node->next = head->next;
        node->pre = head.get();
        head->next->pre = node;
        head->next = node;
    }
    void remove(DummyNode* node) {
        node->pre->next = node->next;
        node->next->pre = node->pre;
    }
    Node* removeLast() {
        if (empty()) {
            return nullptr;
        }
        DummyNode* node = tail->pre;
        remove(node);
        return static_cast<Node*>(node);
    }
};
class LFUCache {
public:
    LFUCache(int capacity) : capacity(capacity), size(0), minfreq(0) {}

    int get(int key) {
        if (!nodes.count(key)) {
            return -1;
        }
        Node* node = nodes[key].get();
        touch(node);
        return node->value;
    }

    void put(int key, int value) {
        if (capacity <= 0) {
            return;
        }
        if (nodes.count(key)) {
            Node* node = nodes[key].get();
            node->value = value;
            touch(node);
            return;
        }
        if (size == capacity) {
            evict();
        }
        auto newNode = make_unique<Node>(key, value);
        minFreq = 1;
        freqMap[1].addToHead(newNode.get());
        nodes[key] = move(newNode);
        size++;
    }

private:
    int size;
    int capacity;
    int minFreq;
    unordered_map<int, unique_ptr<Node>> nodes;
    unordered_map<int, FreqList> freqMap;
    //核心函数
    void touch(Node* node) {
        int freq = node->freq;
        freqMap[freq].remove(node);  //删除该频次
        if (freq == minFreq && freqMap[freq].empty()) {
            minfreq++;  //判断一下是不是最小频次且为唯一最小
        }
        node->freq++;
        freqMap[node->freq].addToHead(node);  //频次改变了，换个链表待着
    }
    void evict() {
        Node* node = freqMap[minFreq].removeLast();
        nodes.erase(node->key);
        size--;
    }
};

