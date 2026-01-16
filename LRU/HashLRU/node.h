//
// Created by Morty on 2026-01-12.
//
#pragma once

struct Node {
    int key;
    int value;
    Node* prev;
    Node* next;
    Node()
            : key(0), value(0), prev(nullptr), next(nullptr) {}
    Node(int k, int v)
            : key(k), value(v), prev(nullptr), next(nullptr) {}
};
