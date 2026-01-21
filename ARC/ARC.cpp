//
// Created by Morty on 2026-01-17.
//
#include <unordered_map>
#include <memory>
#include <algorithm>
struct DummyNode {
    DummyNode *pre;
    DummyNode *next;

    DummyNode() : pre(nullptr), next(nullptr) {}
};

struct Node : public DummyNode {
    int key;
    int value;

    Node() : key(0), value(0) {

    }

    Node(int k, int v) : key(k), value(v) {

    }
};
struct List{
    std::unique_ptr<DummyNode> head;
    std::unique_ptr<DummyNode> tail;
    int size;
    List(): size(0){
        head = std::make_unique<DummyNode>();
        tail = std::make_unique<DummyNode>();
        head->next = tail.get();
        tail->pre = head.get();
    }
    bool empty() const{
        return size == 0;
    }
    void addToHead(DummyNode* node) {
        node->next = head->next;
        node->pre = head.get();
        head->next->pre = node;
        head->next = node;
        size++;
    }
    void remove(DummyNode* node) {
        node->pre->next = node->next;
        node->next->pre = node->pre;
        size--;
    }
    DummyNode* removeLast() {
        if (empty()) return nullptr;
        DummyNode* node = tail->pre;
        remove(node);
        return static_cast<Node*>node;
    }
};
class ARCCache {
public:
    int get(int key) {
        if (cache.count(key)) {
            Node* node = cache[key].get();
            // 在 T1 或 T2
            if (inT1(node)) {
                T1.remove(node);
                T2.addToHead(node);
            } else {
                T2.remove(node);
                T2.addToHead(node);
            }
            return node->value;
        }

        // 命中 B1
        if (B1map.count(key)) {
            p = std::min(capacity, p + std::max(1, B2.size / std::max(1, B1.size)));
            replace(key);
            moveGhostToT2(key);
            return cache[key]->value;
        }

        // 命中 B2
        if (B2map.count(key)) {
            p = std::max(0, p - std::max(1, B1.size / std::max(1, B2.size)));
            replace(key);
            moveGhostToT2(key);
            return cache[key]->value;
        }

        return -1;
    }
    void put(int key, int value) {
        if (capacity == 0) return;

        if (cache.count(key)) {
            cache[key]->value = value;
            get(key);
            return;
        }

        if (T1.size + B1.size == capacity) {
            if (T1.size < capacity) {
                DummyNode* g = B1.removeLast();
                B1map.erase(static_cast<GhostNode*>(g)->key);
                delete g;
                replace(key);
            } else {
                DummyNode* v = T1.removeLast();
                evictToB1(static_cast<Node*>(v));
            }
        } else if (T1.sz + B1.sz < capacity) {
            int total = T1.sz + T2.sz + B1.sz + B2.sz;
            if (total >= capacity) {
                if (total == 2 * capacity) {
                    DummyNode* g = B2.removeLast();
                    B2map.erase(static_cast<GhostNode*>(g)->key);
                    delete g;
                }
                replace(key);
            }
        }

        auto node = std::make_unique<Node>(key, value);
        Node* raw = node.get();
        T1.addToHead(raw);
        cache[key] = std::move(node);
    }
private:
    struct GhostNode : public DummyNode {
        int key;
        GhostNode(int k) : key(k) {}
    };
    int capacity;
    int p;
    List T1, T2, B1, B2;
    std::unordered_map<int, std::unique_ptr<Node>> cache;
    std::unordered_map<int, GhostNode*> B1map;
    std::unordered_map<int, GhostNode*> B2map;
    bool inT1(Node* n) {
        DummyNode* cur = T1.head->next;
        while (cur != T1.tail.get()) {
            if (cur == n) return true;
            cur = cur->next;
        }
        return false;
    }
    void replace(int key) {
        if (T1.size > 0 && (T1.size > p || (B2map.count(key) && T1.size == p))) {
            DummyNode* v = T1.removeLast();
            evictToB1(static_cast<Node*>(v));
        } else {
            DummyNode* v = T2.removeLast();
            evictToB2(static_cast<Node*>(v));
        }
    }
    void evictToB1(Node* node) {
        cache.erase(node->key);
        auto g = new GhostNode(node->key);
        B1.addToHead(g);
        B1map[g->key] = g;
    }

    void evictToB2(Node* node) {
        cache.erase(node->key);
        auto g = new GhostNode(node->key);
        B2.addToHead(g);
        B2map[g->key] = g;
    }
    void moveGhostToT2(int key) {
        auto node = std::make_unique<Node>(key, 0);
        Node* raw = node.get();
        T2.addToHead(raw);
        cache[key] = std::move(node);

        if (B1map.count(key)) {
            GhostNode* g = B1map[key];
            B1.remove(g);
            B1map.erase(key);
            delete g;
        } else {
            GhostNode* g = B2map[key];
            B2.remove(g);
            B2map.erase(key);
            delete g;
        }
    }
}