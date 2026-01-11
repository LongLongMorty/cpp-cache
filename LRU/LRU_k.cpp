//
// Created by Morty on 2026-01-11.
//

#include <unordered_map>
#include <memory>

struct Node {
    int key;
    int value;
    int freq;        // 访问次数
    Node *prev;
    Node *next;

    Node()
            : key(0), value(0), freq(1), prev(nullptr), next(nullptr) {}  //默认为1

    Node(int k, int v)
            : key(k), value(v), freq(1), prev(nullptr), next(nullptr) {}
};

class LRU_K {
public:
    LRU_K(int capacity, int k)
            : capacity(capacity), K(k), size(0) {
        initList(hHead, hTail);
        initList(cHead, cTail);
    }

    int get(int key) {
        if (!nodes.count(key)) return -1;

        Node *node = nodes[key].get();
        node->freq++;

        if (node->freq == K) {
            // 晋升到 cache
            remove(node);
            addToHead(cHead, node);
        } else if (node->freq > K) {
            // cache 内移动
            remove(node);
            addToHead(cHead, node);
        } else {
            // history 内移动
            remove(node);
            addToHead(hHead, node);
        }
        return node->value;
    }

    void put(int key, int value) {
        if (capacity == 0) return;

        if (nodes.count(key)) {
            Node *node = nodes[key].get();
            node->value = value;
            get(key); // 复用逻辑
            return;
        }

        if (size == capacity) {
            evict();
        }

        auto newNode = std::make_unique<Node>(key, value);
        Node *raw = newNode.get();

        addToHead(hHead, raw);      // 新数据先进 history
        nodes[key] = std::move(newNode);
        size++;
    }

private:
    int capacity;
    int K;
    int size;

    std::unordered_map<int, std::unique_ptr<Node>> nodes;

    Node *hHead;
    Node *hTail;
    Node *cHead;
    Node *cTail;

    void initList(Node *&head, Node *&tail) {
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
    }

    void remove(Node *node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void addToHead(Node *head, Node *node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    void evict() {
        Node *victim = nullptr;

        // 优先淘汰 history
        if (hTail->prev != hHead) {
            victim = hTail->prev;
        } else {
            victim = cTail->prev;
        }

        remove(victim);
        nodes.erase(victim->key); // unique_ptr 自动 delete
        size--;
    }
};
