#ifndef SRC_HPP
#define SRC_HPP

#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

template <typename T>
class ESet {
public:
    struct Node {
        T key;
        unsigned int priority;
        Node *left, *right, *parent;
        int size;

        Node(T k) : key(k), left(nullptr), right(nullptr), parent(nullptr), size(1) {
            priority = rand();
        }
    };

    class iterator {
        Node* ptr;
        Node* root_ptr; // Store root to handle operator-- on end()

    public:
        iterator(Node* p = nullptr, Node* r = nullptr) : ptr(p), root_ptr(r) {}
        T operator*() const { return ptr->key; }
        Node* get_ptr() const { return ptr; }

        bool operator==(const iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }

        iterator& operator++() {
            if (ptr->right) {
                ptr = ptr->right;
                while (ptr->left) ptr = ptr->left;
            } else {
                Node* p = ptr->parent;
                while (p && ptr == p->right) {
                    ptr = p;
                    p = p->parent;
                }
                ptr = p;
            }
            return *this;
        }

        iterator& operator--() {
            if (ptr == nullptr) {
                ptr = root_ptr;
                if (ptr) while (ptr->right) ptr = ptr->right;
                return *this;
            }
            if (ptr->left) {
                ptr = ptr->left;
                while (ptr->right) ptr = ptr->right;
            } else {
                Node* p = ptr->parent;
                while (p && ptr == p->left) {
                    ptr = p;
                    p = p->parent;
                }
                ptr = p;
            }
            return *this;
        }
    };

private:
    Node* root;

    int getSize(Node* t) { return t ? t->size : 0; }
    void updateSize(Node* t) {
        if (t) t->size = getSize(t->left) + getSize(t->right) + 1;
    }

    void split(Node* t, T key, Node*& l, Node*& r) {
        if (!t) {
            l = r = nullptr;
            return;
        }
        if (t->key <= key) {
            split(t->right, key, t->right, r);
            l = t;
            if (t->right) t->right->parent = t;
        } else {
            split(t->left, key, l, t->left);
            r = t;
            if (t->left) t->left->parent = t;
        }
        if (l) l->parent = nullptr;
        if (r) r->parent = nullptr;
        updateSize(t);
    }

    Node* merge(Node* l, Node* r) {
        if (!l || !r) return l ? l : r;
        if (l->priority > r->priority) {
            l->right = merge(l->right, r);
            if (l->right) l->right->parent = l;
            updateSize(l);
            return l;
        } else {
            r->left = merge(l, r->left);
            if (r->left) r->left->parent = r;
            updateSize(r);
            return r;
        }
    }

    Node* copy(Node* t, Node* p) {
        if (!t) return nullptr;
        Node* newNode = new Node(t->key);
        newNode->priority = t->priority;
        newNode->size = t->size;
        newNode->parent = p;
        newNode->left = copy(t->left, newNode);
        newNode->right = copy(t->right, newNode);
        return newNode;
    }

    void clear(Node* t) {
        if (!t) return;
        clear(t->left);
        clear(t->right);
        delete t;
    }

public:
    ESet() : root(nullptr) {}
    ~ESet() { clear(root); }
    ESet(const ESet& other) { root = copy(other.root, nullptr); }
    ESet& operator=(const ESet& other) {
        if (this != &other) {
            clear(root);
            root = copy(other.root, nullptr);
        }
        return *this;
    }

    std::pair<iterator, bool> emplace(T key) {
        Node* curr = root;
        while (curr) {
            if (key < curr->key) curr = curr->left;
            else if (curr->key < key) curr = curr->right;
            else return {iterator(curr, root), false};
        }

        Node *l, *r;
        split(root, key, l, r);
        Node* m = new Node(key);
        root = merge(l, merge(m, r));
        if (root) root->parent = nullptr;
        return {iterator(m, root), true};
    }

    void erase(T key) {
        Node *l, *m, *r;
        split(root, key - 1, l, r);
        split(r, key, m, r);
        if (m) clear(m);
        root = merge(l, r);
        if (root) root->parent = nullptr;
    }

    iterator find(T key) {
        Node* curr = root;
        while (curr) {
            if (key < curr->key) curr = curr->left;
            else if (curr->key < key) curr = curr->right;
            else return iterator(curr, root);
        }
        return end();
    }

    int range(T b, T c) {
        if (b > c) return 0;
        Node *l, *m, *r;
        split(root, b - 1, l, r);
        split(r, c, m, r);
        int res = getSize(m);
        root = merge(l, merge(m, r));
        if (root) root->parent = nullptr;
        return res;
    }

    iterator end() { return iterator(nullptr, root); }
};

#endif
