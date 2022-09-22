/**
 * Generic AVL (Balanced) Rank Tree.
 *
 * @file avl.hpp
 *
 * @brief Implementation of a Generic AVL(Balanced) Rank Tree.
 *
 * @author Liav Barsheshet
 * Contact: liavbarsheshet@gmail.com
 *
 * This implementation is free: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This implementation is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <stdlib.h>
#include <iostream>

#ifndef _AVL_RANK_TREE_HPP
#define _AVL_RANK_TREE_HPP

namespace AVL {
    typedef enum {
        LESS_THAN = -1, EQUAL = 0, GREATER_THAN = 1
    } COMPARE_RESULT;

    typedef enum {
        ROOT, LEFT_CHILD, RIGHT_CHILD
    } NODE_POSITION;

    class InvalidRankInfo : public std::exception {
    };

    template<typename Key, typename Value, typename Number = long long>
    class FilterObject;

    template<typename Key>
    class CompareFunc;

    template<typename Key, typename Value, typename Number=long long>
    class DefaultRank;

    template<typename Key, typename Value, class RankInfo, typename Number = long long>
    class Node;

    template<typename Key, typename Value>
    class KeyValuePair;

    template<typename Key, typename Value, typename Number=long long>
    class QueryResult;

    template<typename Key, typename Value,
            typename Number = long long,
            class RankInfo=DefaultRank<Key, Value, Number>,
            class Compare = CompareFunc<Key>>
    class AVLRankTree;

}

/**
 * Class: Compare Function.
 * Plans how to compare between two keys.
 * @tparam Key - The type/class of the key.
 */
template<typename Key>
class AVL::CompareFunc {
public:
    /**
     * Virtual function comparing between two keys.
     * @param key1 - First key.
     * @param key2  - Second key.
     * @return Compare Result (LESS_THAN,EQUAL,GREATER_THAN).
     */
    virtual AVL::COMPARE_RESULT operator()(const Key key1, const Key key2) const {
        if (key1 < key2) {
            return LESS_THAN;
        }
        if (key1 > key2) {
            return GREATER_THAN;
        }
        return EQUAL;
    };
};

/**
 * Class: Filter Object.
 * An object query that lets you modify the traverse or the items.
 * @tparam Key - Type/Class of the key.
 * @tparam Value - Type/Class of the data.
 * @tparam Number - Class/Primitive for numbers representation.
 */
template<typename Key, typename Value, typename Number>
class AVL::FilterObject {
public:
    /* Represents the query result limit. (Default: -1) */
    Number limit;
    /* Represents the query result order. (Default: false) */
    bool reverse;

    /* Represents the query min range. (Default: NULL) */
    Key *min_range;
    /* Represents the query max range. (Default: NULL) */
    Key *max_range;

    /* Custom filter function. (Default: NULL) */
    bool (*FilterFunction)(Key, Value);


    FilterObject() :
            limit(-1),
            reverse(false),
            min_range(NULL),
            max_range(NULL),
            FilterFunction(NULL) {}
};

/**
 * [Example] Class: Default class for rank information.
 * @tparam Key - The type/class of the key.
 * @tparam Value - The type/class of the value.
 * @tparam Number - Class/Primitive for numbers representation.
 */
template<typename Key, typename Value, typename Number>
class AVL::DefaultRank {
public:
    Number rank;

    DefaultRank() :
            rank(0) {}

    DefaultRank(Key key, Value value) :
            rank(1) {}

    DefaultRank(const DefaultRank<Key, Value, Number> &_rank) {
        this->rank = _rank.rank;
    }

    ~DefaultRank() {}

    AVL::DefaultRank<Key, Value, Number> &operator=(const DefaultRank<Key, Value, Number> &_rank) {
        this->rank = _rank.rank;
        return (*this);
    }

    AVL::DefaultRank<Key, Value, Number> &operator-=(const DefaultRank<Key, Value, Number> &_rank) {
        this->rank -= _rank.rank;
        return (*this);
    }

    AVL::DefaultRank<Key, Value, Number> &operator+=(const DefaultRank<Key, Value, Number> &_rank) {
        this->rank += _rank.rank;
        return (*this);
    }

    std::ostream &Print(std::ostream &os) const {
        os << "{" << this->rank << "}";
        return os;
    }
};

/**
 * Class: Represents nodes inside the AVL Rank Tree.
 * @tparam Key - The type/class of the key.
 * @tparam Value - The type/class of the value.
 * @tparam RankInfo - Inherited Rank Class.
 * @tparam Number - Class/Primitive for numbers representation.
 */
template<typename Key, typename Value, class RankInfo, typename Number>
class AVL::Node {
public:
    Node *parent;
    Node *right_child;
    Node *left_child;

    Key key;
    Value value;
    Number height;

    RankInfo *rank;

    Node() :
            parent(NULL),
            right_child(NULL),
            left_child(NULL),
            height(0),
            rank(new RankInfo()) {
    }

    Node(Key key, Value value) :
            parent(NULL),
            right_child(NULL),
            left_child(NULL),
            key(key),
            value(value),
            height(0),
            rank(new RankInfo(key, value)) {
    }

    Node(const Node<Key, Value, RankInfo, Number> &node) :
            parent(NULL),
            right_child(NULL),
            left_child(NULL),
            key(node.key),
            value(node.value),
            height(node.height),
            rank(new RankInfo((*node.rank))) {}

    ~Node() {
        delete this->rank;
    }

    std::ostream &Print(std::ostream &os) const {
        os << "{ Key: " << this->key << ",\t";
        os << "Height:" << this->height << ",\t";
        os << "Rank: ";
        this->rank->Print(os);
        os << " } \n";
        return os;
    }
};

/**
 * Class: Represents a Pair of key and value.
 * @tparam Key - The type/class of the key.
 * @tparam Value - The type/class of the value representation.
 */
template<typename Key, typename Value>
class AVL::KeyValuePair {
public:
    Key key;
    Value value;

    KeyValuePair() = default;

    KeyValuePair(Key key, Value value) :
            key(key),
            value(value) {}

    std::ostream &Print(std::ostream &os) const {
        os << "(" << this->key << ", " << this->value << ")";
        return os;
    }
};

template<typename Key, typename Value>
std::ostream &operator<<(std::ostream &os, const AVL::KeyValuePair<Key, Value> &pair) {
    pair.Print(os);
    return os;
}

/**
 * Class: Represents an object containing array of results and the amount of items within it.
 * @tparam Key - The type/class of the key.
 * @tparam Value - The type/class of the value.
 * @tparam Number - Class/Primitive for numbers representation.
 */
template<typename Key, typename Value, typename Number>
class AVL::QueryResult {
public:
    AVL::KeyValuePair<Key, Value> *result;
    Number total;

    QueryResult() :
            result(NULL),
            total(0) {}

    ~QueryResult() {
        delete[] result;
    }

    std::ostream &Print(std::ostream &os) const {
        os << "Elements: [";
        if (this->result) {
            for (Number i = 0; i < this->total; ++i) {
                if (i != 0) {
                    os << ", ";
                }
                os << this->result[i].key;
                if (i >= 3) {
                    os << "...";
                    break;
                }
            }
        }
        os << "]" << std::endl;
        os << "Total Elements: " << this->total << std::endl;
        return os;
    }

};

template<typename Key, typename Value, typename Number>
std::ostream &operator<<(std::ostream &os, const AVL::QueryResult<Value, Key, Number> &query) {
    query.Print(os);
    return os;
}

/**
 * Class: Represents the entire AVL Rank Tree.
 * @tparam Key - The type/class of the key.
 * @tparam Value - The type/class of the value.
 * @tparam RankInfo - Inherited Rank Class.
 * @tparam Compare - Compare Function Object.
 * @tparam Number - Class/Primitive for numbers representation.
 */
template<typename Key, typename Value, typename Number, class RankInfo, class Compare>
class AVL::AVLRankTree {

    Number size;
    AVL::Node<Key, Value, RankInfo, Number> *root;
    AVL::Node<Key, Value, RankInfo, Number> *max_node;
    AVL::Node<Key, Value, RankInfo, Number> *min_node;
    Compare compare;

    /** Rotations & Balance */
    void
    UpdateRank(AVL::Node<Key, Value, RankInfo, Number> *target, AVL::Node<Key, Value, RankInfo, Number> *child_node1,
               AVL::Node<Key, Value, RankInfo, Number> *child_node2) {
        if (!target) {
            return;
        }
        RankInfo tmp = RankInfo(target->key, target->value);
        (*target->rank) = tmp;

        if (child_node1) {
            (*target->rank) += (*child_node1->rank);
        }
        if (child_node2) {
            (*target->rank) += (*child_node2->rank);
        }
    }

    Number GetHeight(Node<Key, Value, RankInfo, Number> *node) const {
        if (!node) {
            return -1;
        }
        return node->height;
    }

    AVL::Node<Key, Value, RankInfo, Number> *RotateR(AVL::Node<Key, Value, RankInfo, Number> *node) {
        Node<Key, Value, RankInfo, Number> *x = node;
        Node<Key, Value, RankInfo, Number> *y = x->left_child;
        Node<Key, Value, RankInfo, Number> *y_right = y->right_child;
        y->right_child = x;
        y->parent = x->parent;
        x->parent = y;
        x->left_child = y_right;
        if (y_right) {
            y_right->parent = x;
        }
        x->height = (std::max(this->GetHeight(y_right), this->GetHeight(x->right_child)) + 1);
        y->height = (std::max(this->GetHeight(y->left_child), this->GetHeight(x)) + 1);
        this->UpdateRank(x, y_right, x->right_child);
        this->UpdateRank(y, y->left_child, x);
        return y;
    }

    AVL::Node<Key, Value, RankInfo, Number> *RotateL(AVL::Node<Key, Value, RankInfo, Number> *node) {
        Node<Key, Value, RankInfo, Number> *x = node;
        Node<Key, Value, RankInfo, Number> *y = x->right_child;
        Node<Key, Value, RankInfo, Number> *y_left = y->left_child;
        y->left_child = x;
        y->parent = x->parent;
        x->parent = y;
        x->right_child = y_left;
        if (y_left) {
            y_left->parent = x;
        }
        x->height = (std::max(this->GetHeight(y_left), this->GetHeight(x->left_child)) + 1);
        y->height = (std::max(this->GetHeight(y->right_child), this->GetHeight(x)) + 1);
        this->UpdateRank(x, y_left, x->left_child);
        this->UpdateRank(y, y->right_child, x);
        return y;
    }

    Number GetBalance(Node<Key, Value, RankInfo, Number> *node) const {
        return (this->GetHeight(node->left_child) - this->GetHeight(node->right_child));
    }

    AVL::Node<Key, Value, RankInfo, Number> *Balance(AVL::Node<Key, Value, RankInfo, Number> *node) {
        // LL Case
        if (this->GetBalance(node) == 2 && this->GetBalance(node->left_child) >= 0) {
            return this->RotateR(node);
        }
            // RR Case
        else if (this->GetBalance(node) == -2 && this->GetBalance(node->right_child) <= 0) {
            return this->RotateL(node);
        }
            // LR Case
        else if (this->GetBalance(node) == 2 && this->GetBalance(node->left_child) < 0) {
            node->left_child = this->RotateL(node->left_child);
            return (this->RotateR(node));
        }
            // RL Case
        else if (this->GetBalance(node) == -2 && this->GetBalance(node->right_child) > 0) {
            node->right_child = this->RotateR(node->right_child);
            return (this->RotateL(node));
        }
        return node;
    }

    /** Private Methods */

    AVL::Node<Key, Value, RankInfo, Number> *FindMin(AVL::Node<Key, Value, RankInfo, Number> *node) {
        if (!node) {
            return NULL;
        }
        if (!node->left_child) {
            return node;
        }
        return this->FindMin(node->left_child);
    }

    AVL::Node<Key, Value, RankInfo, Number> *FindMax(AVL::Node<Key, Value, RankInfo, Number> *node) {
        if (!node) {
            return NULL;
        }
        if (!node->right_child) {
            return node;
        }
        return this->FindMax(node->right_child);
    }

    AVL::NODE_POSITION GetNodePosition(AVL::Node<Key, Value, RankInfo, Number> *node) const {
        Compare comparing_func;
        if (!node->parent) {
            return ROOT;
        }
        if (!node->parent->left_child) {
            return RIGHT_CHILD;
        }
        if (!node->parent->right_child) {
            return LEFT_CHILD;
        }
        COMPARE_RESULT result = comparing_func(node->parent->left_child->key, node->key);

        return (result == EQUAL ? LEFT_CHILD : RIGHT_CHILD);
    }

    AVL::Node<Key, Value, RankInfo, Number> *
    FindTraverse(AVL::Node<Key, Value, RankInfo, Number> *node, const Key key) const {
        if (!node) {
            return NULL;
        }
        Compare comparing_func;
        COMPARE_RESULT result = comparing_func(key, node->key);
        Node<Key, Value, RankInfo, Number> *tmp;
        if (result == EQUAL) {
            return node;
        } else if (result == LESS_THAN) {
            return this->FindTraverse(node->left_child, key);
        }
        return this->FindTraverse(node->right_child, key);
    }

    void
    ClosestTraverse(AVL::Node<Key, Value, RankInfo, Number> *node, const Key key,
                    AVL::Node<Key, Value, RankInfo, Number> **result_node, COMPARE_RESULT range) const {
        if (!node) {
            return;
        }
        Compare comparing_func;
        COMPARE_RESULT result = comparing_func(key, node->key);
        Node<Key, Value, RankInfo, Number> *tmp;
        if (result == EQUAL) {
            (*result_node) = node;
            return;
        } else if (result == LESS_THAN) {
            if (range == GREATER_THAN) {
                (*result_node) = node;
            }
            return this->ClosestTraverse(node->left_child, key, result_node, range);
        }
        if (range == LESS_THAN) {
            (*result_node) = node;
        }
        return this->ClosestTraverse(node->right_child, key, result_node, range);
    }


    void GetRelativeRank(RankInfo &rank, Node<Key, Value, RankInfo, Number> *node,
                         NODE_POSITION direction = LEFT_CHILD) const {
        if (!node) {
            return;
        }
        rank = (*node->rank);
        if (node->right_child && (direction != RIGHT_CHILD)) { //LEFT_CHILD
            rank -= *(node->right_child->rank);
        }
        if (node->left_child && (direction != LEFT_CHILD)) { // RIGHT_CHILD
            rank -= *(node->left_child->rank);
        }
    }

    Node<Key, Value, RankInfo, Number> *
    FindIndexTraverse(Node<Key, Value, RankInfo, Number> *node, const Number &index, Number &cur_index) const {
        if (!node) {
            return NULL;
        }
        if (index == cur_index) {
            return node;
        }
        RankInfo rank = RankInfo();
        if (index > cur_index) {
            this->GetRelativeRank(rank, node->right_child);
            cur_index += rank.rank;
            return this->FindIndexTraverse(node->right_child, index, cur_index);
        }
        if (node->left_child->right_child) {
            cur_index -= node->left_child->right_child->rank->rank;
        }
        cur_index -= 1;
        return this->FindIndexTraverse(node->left_child, index, cur_index);
    }

    Node<Key, Value, RankInfo, Number> *InsertTraverse(Node<Key, Value, RankInfo, Number> *node, const Key key,
                                                       const Value value) {
        if (!node) {
            return (new Node<Key, Value, RankInfo, Number>(key, value));
        }
        const COMPARE_RESULT compare_result = this->compare(key, node->key);
        // LESS THAN
        if (compare_result == LESS_THAN) {
            node->left_child = this->InsertTraverse(node->left_child, key, value);
            node->left_child->parent = node;
        } else {
            node->right_child = this->InsertTraverse(node->right_child, key, value);
            node->right_child->parent = node;
        }
        node->height = (std::max(this->GetHeight(node->left_child), this->GetHeight(node->right_child)) + 1);
        this->UpdateRank(node, node->left_child, node->right_child);
        return this->Balance(node);
    }

    Node<Key, Value, RankInfo, Number> *RemoveTraverse(Node<Key, Value, RankInfo, Number> *node, const Key key) {
        if (!node) {
            return NULL;
        }
        const COMPARE_RESULT compare_result = this->compare(key, node->key);
        switch (compare_result) {
            case LESS_THAN:
                node->left_child = this->RemoveTraverse(node->left_child, key);
                if (node->left_child) {
                    node->left_child->parent = node;
                }
                break;
            case GREATER_THAN:
                node->right_child = this->RemoveTraverse(node->right_child, key);
                if (node->right_child) {
                    node->right_child->parent = node;
                }
                break;
            case EQUAL:
                if (node->left_child && node->right_child) {
                    Node<Key, Value, RankInfo, Number> *min_val = this->FindMin(node->right_child);
                    node->value = min_val->value;
                    node->key = min_val->key;
                    RankInfo *rank = new RankInfo(*min_val->rank);
                    delete node->rank;
                    node->rank = rank;
                    Node<Key, Value, RankInfo, Number> *tmp = this->RemoveTraverse(node->right_child, min_val->key);
                    node->right_child = tmp;
                    if (tmp) {
                        tmp->parent = node;
                    }
                } else if (node->left_child) {
                    Node<Key, Value, RankInfo, Number> *temp = node->left_child;
                    delete node;
                    return temp;
                } else if (node->right_child) {
                    Node<Key, Value, RankInfo, Number> *temp = node->right_child;
                    delete node;
                    return temp;
                } else { // NO CHILDREN NODES
                    delete node;
                    return NULL;
                }
                break;
        }
        node->height = (std::max(this->GetHeight(node->left_child), this->GetHeight(node->right_child)) + 1);
        this->UpdateRank(node, node->left_child, node->right_child);
        return this->Balance(node);
    }

    void QueryTraverse(Node<Key, Value, RankInfo, Number> *node,
                       QueryResult<Key, Value, Number> *query,
                       Node<Key, Value, RankInfo, Number> *result_node,
                       const AVL::FilterObject<Key, Value, Number> &filter) const {
        Compare compare_func;
        COMPARE_RESULT compare_result_min;
        COMPARE_RESULT compare_result_max;

        if (!node || ((filter.limit <= query->total) && filter.limit > -1)) {
            return;
        }

        if (filter.min_range) {
            compare_result_min = compare_func(node->key, (*filter.min_range));
            if (compare_result_min == LESS_THAN) {
                return this->QueryTraverse(node->right_child, query, result_node, filter);
            }
        }
        if (filter.max_range) {
            compare_result_max = compare_func(node->key, (*filter.max_range));
            if (compare_result_max == GREATER_THAN) {
                return this->QueryTraverse(node->left_child, query, result_node, filter);
            }
        }

        this->QueryTraverse(node->left_child, query, result_node, filter);
        if (filter.limit <= -1 || (filter.limit > query->total)) {
            if (!filter.FilterFunction || filter.FilterFunction(node->key, node->value)) {
                result_node->left_child->right_child = new Node<Key, Value, RankInfo, Number>(node->key, node->value);
                result_node->left_child = result_node->left_child->right_child;
                result_node->left_child->parent = result_node->parent;
                ++(query->total);
            }
        }
        return this->QueryTraverse(node->right_child, query, result_node, filter);
    }

    QueryResult<Key, Value, Number> *MergeTwoQueries(QueryResult<Key, Value, Number> *first,
                                                     QueryResult<Key, Value, Number> *second) const {
        Compare compare_func;
        QueryResult<Key, Value, Number> *result = new QueryResult<Key, Value, Number>();
        result->total = first->total + second->total;

        if (result->total == 0) {
            return NULL;
        }

        Number i = 0;
        Number j = 0;
        Number t = 0;

        result->result = new KeyValuePair<Key, Value>[result->total]();

        while (t < result->total) {
            KeyValuePair<Key, Value> min;
            if (i < first->total && j < second->total) {
                const COMPARE_RESULT compare = compare_func(first->result[i].key, second->result[j].key);
                min = compare == LESS_THAN ? first->result[i] : second->result[j];
                compare == LESS_THAN ? ++i : ++j;
                result->result[t] = KeyValuePair<Key, Value>(min.key, min.value);
                ++t;
                continue;
            }
            min = (j >= second->total ? first->result[i] : second->result[j]);
            j >= second->total ? ++i : ++j;
            result->result[t] = KeyValuePair<Key, Value>(min.key, min.value);
            ++t;
        }
        return result;
    }

    Node<Key, Value, RankInfo, Number> *TreeFromQuery(QueryResult<Key, Value, Number> *query, Number start,
                                                      Number end) {
        if (start > end) {
            return NULL;
        }
        Number middle = (start + end) / 2;
        Node<Key, Value, RankInfo, Number> *root = new Node<Key, Value, RankInfo, Number>(query->result[middle].key,
                                                                                          query->result[middle].value);
        if (!this->root) {
            this->root = root;
        }
        if (middle == 0) {
            this->min_node = root;
        }
        if (middle == query->total - 1) {
            this->max_node = root;
        }
        root->left_child = (this->TreeFromQuery(query, start, middle - 1));
        if (root->left_child) {
            root->left_child->parent = root;
        }
        root->right_child = this->TreeFromQuery(query, middle + 1, end);
        if (root->right_child) {
            root->right_child->parent = root;
        }
        root->height = (std::max(this->GetHeight(root->left_child), this->GetHeight(root->right_child)) + 1);
        this->UpdateRank(root, root->left_child, root->right_child);
        return root;
    }

    Node<Key, Value, RankInfo, Number> *
    GetMostLowerCommonNode(Node<Key, Value, RankInfo, Number> *root, Node<Key, Value, RankInfo, Number> *node1,
                           Node<Key, Value, RankInfo, Number> *node2) const {
        if (root == NULL) {
            return NULL;
        }

        Compare comparing_func;
        COMPARE_RESULT result_1 = comparing_func(root->key, node1->key);
        COMPARE_RESULT result_2 = comparing_func(root->key, node2->key);

        if (result_1 == GREATER_THAN && result_2 == GREATER_THAN) {
            return this->GetMostLowerCommonNode(root->left_child, node1, node2);
        }

        if (result_1 == LESS_THAN && result_2 == LESS_THAN) {
            return this->GetMostLowerCommonNode(root->right_child, node1, node2);
        }

        return root;
    }

    void Deallocation(Node<Key, Value, RankInfo, Number> *node) {
        if (!node) {
            return;
        }
        this->Deallocation(node->left_child);
        this->Deallocation(node->right_child);
        delete node;
    }

    void
    CollectRankTraverse(Node<Key, Value, RankInfo, Number> *mlc, Node<Key, Value, RankInfo, Number> *node,
                        Node<Key, Value, RankInfo, Number> *relative_node, RankInfo *rank,
                        NODE_POSITION direction) const {
        RankInfo tmp_rank = RankInfo();
        Compare comparing_func;
        COMPARE_RESULT res;
        if (comparing_func(mlc->key, node->key) == EQUAL) {
            return;
        }
        if (direction == RIGHT_CHILD) {
            res = comparing_func(node->key, relative_node->key); // node >= max
            if (res == LESS_THAN) {
                this->CollectRankTraverse(mlc, node->parent, relative_node, rank, direction);
                return;
            }
            this->GetRelativeRank(tmp_rank, node, RIGHT_CHILD);
            (*rank) += tmp_rank;
            this->CollectRankTraverse(mlc, node->parent, relative_node, rank, direction);
            return;
        }

        res = comparing_func(node->key, relative_node->key); // node <= max
        if (res == GREATER_THAN) {
            this->CollectRankTraverse(mlc, node->parent, relative_node, rank, direction);
            return;
        }

        this->GetRelativeRank(tmp_rank, node, LEFT_CHILD);
        (*rank) += tmp_rank;
        this->CollectRankTraverse(mlc, node->parent, relative_node, rank, direction);
    }

    Number GetIndexOfKeyTraverse(Node<Key, Value, RankInfo> *node, const Key &key) const {
        if (!node) {
            return -1;
        }
        Compare comparing_func;
        COMPARE_RESULT result = comparing_func(key, node->key);
        RankInfo relative_rank = RankInfo();
        Number res = 0;
        if (result == EQUAL) {
            this->GetRelativeRank(relative_rank, node);
            res = relative_rank.rank;
            return (res - 1);
        }
        if (result == LESS_THAN) {
            return (this->GetIndexOfKeyTraverse(node->left_child, key));
        }
        this->GetRelativeRank(relative_rank, node);
        res = relative_rank.rank;
        return (res + this->GetIndexOfKeyTraverse(node->right_child, key));
    }

    std::ostream &PrintTreeInOrder(std::ostream &os, Node<Key, Value, RankInfo, Number> *node) const {
        if (!node) {
            return os;
        }
        this->PrintTreeInOrder(os, node->left_child);
        node->Print(os);
        this->PrintTreeInOrder(os, node->right_child);
        return os;
    }

    /** Public Methods */
public:
    /**
     * Constructor: Constructs an empty AVL rank tree.
     * @note Worst-Time Complexity: O(1).
     */
    AVLRankTree() :
            size(0),
            root(NULL),
            max_node(NULL),
            min_node(NULL),
            compare() {}

    /**
     * Copy Constructor: Creates a copy from an existing AVL rank tree.
     * @note Worst-Time Complexity: O(n).
     * @param tree - AVL rank tree as a reference.
     */
    AVLRankTree(const AVLRankTree &tree) :
            size(tree.GetSize()),
            root(NULL),
            max_node(NULL),
            min_node(NULL),
            compare() {
        if (!tree.root) {
            return;
        }
        QueryResult<Key, Value, Number> query = this->Query();
        this->TreeFromQuery(&query, 0, query.total - 1);
        delete query;
    }

    /**
     * Combine Constructor: Creates a joined AVL rank tree from two given AVL rank trees.
     * @note Worst-Time Complexity: O(n) - n=max{size1,size2}.
     * @note Worst-Space Complexity: O(n) - n=max{size1,size2}.
     * @param first_tree - AVL rank tree as a reference.
     * @param second_tree - AVL rank tree as a reference.
     */
    AVLRankTree(const AVL::AVLRankTree<Key, Value, Number, RankInfo, Compare> &first_tree,
                const AVL::AVLRankTree<Key, Value, Number, RankInfo, Compare> &second_tree)
            :
            size(first_tree.size + second_tree.size),
            root(NULL),
            max_node(NULL),
            min_node(NULL),
            compare() {
        if (this->size == 0) {
            return;
        }
        QueryResult<Key, Value, Number> first_query = first_tree.Query();
        QueryResult<Key, Value, Number> second_query = second_tree.Query();
        QueryResult<Key, Value, Number> *query = this->MergeTwoQueries(&first_query, &second_query);
        this->TreeFromQuery(query, 0, query->total - 1);
        delete query;

    }

    /**
     * Destructor: Deallocates the entire class.
     * @note Worst-Time Complexity: O(n).
     */
    ~AVLRankTree() {
        this->Deallocation(this->root);
    }

    /**
     * Gets the AVL rank tree size.
     * @note Worst-Time Complexity: O(1).
     * @return {Number} Tree size.
     */
    Number GetSize() const {
        return this->size;
    }

    /**
     * Gets the AVL rank tree height.
     * @note Worst-Time Complexity: O(1).
     * @return {Number} Tree height.
     */
    Number GetHeight() const {
        return this->GetHeight(this->root);
    }

    /**
     * Gets the index of a specific elements by key.
     * @note Worst-Time Complexity: O(log(n)).
     * @param key - The element key.
     * @return {Number} Index of an element as if it was in a sorted array.
     */
    Number GetIndexOfKey(const Key &key) const {
        return this->GetIndexOfKeyTraverse(this->root, key);
    }

    /**
     * Gets the Max element by key.
     * @note Worst-Time Complexity: O(1).
     * @return {KeyValuePair<Key, Value>} The maximum element or NULL if the tree is empty.
     */
    KeyValuePair<Key, Value> *GetMax() const {
        if (!this->max_node) {
            return NULL;
        }
        KeyValuePair<Key, Value> *result = new KeyValuePair<Key, Value>(this->max_node->key, this->max_node->value);
        return result;
    }

    /**
     * Gets the Min element by key.
     * @note Worst-Time Complexity: O(1).
     * @return {KeyValuePair<Key, Value>} The minimum element or NULL if the tree is empty.
     */
    KeyValuePair<Key, Value> *GetMin() const {
        if (!this->min_node) {
            return NULL;
        }
        KeyValuePair<Key, Value> *result = new KeyValuePair<Key, Value>(this->min_node->key, this->min_node->value);
        return result;
    }

    /**
     * Find an element by its key.
     * @note Worst-Time Complexity: O(log(n)).
     * @param key - The element key.
     * @return {KeyValuePair<Key, Value>} element or NULL if not found.
     */
    KeyValuePair<Key, Value> *Find(const Key key) const {
        Node<Key, Value, RankInfo, Number> *result_node = this->FindTraverse(this->root, key);
        if (!result_node) {
            return NULL;
        }
        KeyValuePair<Key, Value> *result = new KeyValuePair<Key, Value>(result_node->key, result_node->value);
        return result;
    }

    /**
     * Find an element by its index.
     * @note Worst-Time Complexity: O(log(n)).
     * @param index - The element index as if it was in a sorted array.
     * @return {KeyValuePair<Key, Value>} element or NULL if not found.
     */
    KeyValuePair<Key, Value> *FindIndex(const Number &index) const {
        if (index < 0 || index >= this->size) {
            throw std::out_of_range("Index out of range.");
        }
        RankInfo rank = RankInfo();
        this->GetRelativeRank(rank, this->root);
        Number res_index = rank.rank;
        Node<Key, Value, RankInfo, Number> *result_node =
                this->FindIndexTraverse(this->root, (index + 1), res_index);
        if (!result_node) {
            return NULL;
        }
        KeyValuePair<Key, Value> *result = new KeyValuePair<Key, Value>(result_node->key, result_node->value);
        return result;
    }

    /**
     * Find the closest element to a specific key.
     * @note Worst-Time Complexity: O(log(n)).
     * @param key -  Key that defines the range.
     * @param range - Defines which key closer to the key (LESS_THAN|GREATER_THAN) Default: LESS_THAN.
     * @return {KeyValuePair<Key, Value>} element or NULL if not found.
     */
    KeyValuePair<Key, Value> *Closest(const Key key, COMPARE_RESULT range = LESS_THAN) {
        if (range == EQUAL) {
            return this->Find(key);
        }
        Node<Key, Value, RankInfo, Number> *result_node = NULL;
        this->ClosestTraverse(this->root, key, &result_node, range);
        if (!result_node) {
            return NULL;
        }
        KeyValuePair<Key, Value> *result = new KeyValuePair<Key, Value>(result_node->key, result_node->value);
        return result;
    }

    /**
     * Insert new element to the tree.
     * @note Worst-Time Complexity: O(log(n)).
     * @param key - The element key.
     * @param value - The element value.
     */
    void Insert(const Key key, const Value value) {
        this->root = this->InsertTraverse(this->root, key, value);
        if (this->root) {
            this->root->parent = NULL;
        }
        this->max_node = this->FindMax(this->root);
        this->min_node = this->FindMin(this->root);
        ++this->size;
    }

    /**
     * Removes an element from the tree.
     * @note Worst-Time Complexity: O(log(n)).
     * @param key - The element key.
     * @return {bool} True if removed o.w False.
     */
    bool Remove(const Key key) {
        KeyValuePair<Key, Value> *result = this->Find(key);
        if (!result) {
            return false;
        }
        delete result;
        this->root = this->RemoveTraverse(this->root, key);
        if (this->root) {
            this->root->parent = NULL;
        }
        this->max_node = this->FindMax(this->root);
        this->min_node = this->FindMin(this->root);
        --this->size;
        return true;
    }

    /**
     * Collect relative rank within a given filter object.
     * @note the rank here will be considered as number of elements.
     * @note Worst-Time Complexity: O(log(n)).
     * @param filter - Filter object which contains information considering the traverse.
     * @return {RankInfo} an object containing collective rank information.
     */
    RankInfo *
    CollectRank(const AVL::FilterObject<Key, Value, Number> &filter =
    AVL::FilterObject<Key, Value, Number>()) const {
        RankInfo *rank = new RankInfo();
        RankInfo tmp = RankInfo();
        Node<Key, Value, RankInfo, Number> *max = this->max_node;
        Node<Key, Value, RankInfo, Number> *min = this->min_node;
        Node<Key, Value, RankInfo, Number> *tmp_max = NULL;
        Node<Key, Value, RankInfo, Number> *tmp_min = NULL;
        Compare comparing_func;

        if (filter.max_range) {
            this->ClosestTraverse(this->root, *filter.max_range, &tmp_max, LESS_THAN);
            if (tmp_max) {
                max = tmp_max;
            }
        }
        if (filter.min_range) {
            this->ClosestTraverse(this->root, *filter.min_range, &tmp_min, GREATER_THAN);
            if (tmp_min) {
                min = tmp_min;
            }
        }

        if (!max || !min) {
            return rank;
        }

        if (comparing_func(max->key, min->key) == EQUAL) {
            this->GetRelativeRank(tmp, max, ROOT);
            (*rank) += tmp;
            return rank;
        }

        this->GetRelativeRank(tmp, this->root);
        Number res_index = tmp.rank;
        if (filter.limit > 0) {
            Number max_index = this->GetIndexOfKey(max->key); // Already 0<=index<n
            Number min_index = this->GetIndexOfKey(min->key); // Already 0<=index<n
            Number index = 0;
            if (filter.reverse) {
                index = max_index - filter.limit + 1;
                if (index >= min_index) {
                    min = FindIndexTraverse(this->root, index + 1, res_index);
                }
            } else {
                index = min_index + filter.limit - 1;
                if (index <= max_index) {
                    max = FindIndexTraverse(this->root, index + 1, res_index);
                }
            }

            if (!max || !min) {
                return rank;
            }

            if (comparing_func(max->key, min->key) == EQUAL) {
                tmp = RankInfo();
                this->GetRelativeRank(tmp, max, ROOT);
                (*rank) += tmp;
                return rank;
            }
        }

        Node<Key, Value, RankInfo, Number> *mlc = this->GetMostLowerCommonNode(this->root, min, max);
        tmp = RankInfo();
        this->CollectRankTraverse(mlc, max, max, rank, LEFT_CHILD);
        this->CollectRankTraverse(mlc, min, min, rank, RIGHT_CHILD);

        this->GetRelativeRank(tmp, mlc, ROOT);
        (*rank) += tmp;
        return rank;
    }

    /**
     * Find an element by its index.
     * @note Worst-Time Complexity: O(log(n)).
     * @param index - The element index as if it was in a sorted array.
     * @return {KeyValuePair<Key, Value>} element or NULL if not found.
     */
    KeyValuePair<Key, Value> *operator[](const Number &index) {
        return this->FindIndex(index);
    }

    /**
   * Collect elements within a given filter object.
   * @note Worst-Time Complexity: O(n).
   * @note Worst-Space Complexity: O(n).
   * @param filter - Filter object which contains information considering the traverse.
   * @return {QueryResult<Key, Value, Number>} an object containing result array and total amount of elements.
   */
    QueryResult<Key, Value, Number>
    Query(const AVL::FilterObject<Key, Value, Number> &filterObject =
    AVL::FilterObject<Key, Value, Number>()) const {
        QueryResult<Key, Value, Number> query = QueryResult<Key, Value, Number>();
        Node<Key, Value, RankInfo, Number> *result_chain = new Node<Key, Value, RankInfo, Number>();
        result_chain->parent = result_chain;
        result_chain->left_child = result_chain;

        this->QueryTraverse(this->root, &query, result_chain, filterObject);
        if (!result_chain) {
            return query;
        }
        query.result = new KeyValuePair<Key, Value>[query.total];
        Number i = 0;
        Node<Key, Value, RankInfo, Number> *tmp_chain;
        tmp_chain = result_chain;
        result_chain = result_chain->right_child;
        delete tmp_chain;

        while (result_chain) {
            tmp_chain = result_chain;
            query.result[i] = KeyValuePair<Key, Value>(result_chain->key, result_chain->value);
            result_chain = result_chain->right_child;
            ++i;
            delete tmp_chain;
        }
        return query;
    }

    /**
    * Prints the entire tree.
    * @note Worst-Time Complexity: O(n).
    * @param filter - Filter object which contains information considering the traverse.
    */
    std::ostream &PrintTree(std::ostream &os) const {
        if (!this->root) {
            std::cout << "Empty Tree" << std::endl;
        }
        this->PrintTreeInOrder(os, this->root);
        KeyValuePair<Key, Value> *min = this->GetMin();
        KeyValuePair<Key, Value> *max = this->GetMax();
        os << std::endl;
        os << "[AVL::Tree Stats]" << std::endl;
        os << "--> Root: " << this->root->key << ", Size: " << this->GetSize() << ", Height: " << this->GetHeight() <<
           ", Min: " << min->value << ", Max: " << max->value;
        delete max;
        delete min;
        return os;
    };
};

template<typename Key, typename Value, typename Number, class Rank, class Compare>
std::ostream &operator<<(std::ostream &os, const AVL::AVLRankTree<Value, Key, Rank, Compare, Number> &tree) {
    tree.PrintTree(os);
    os << std::endl;
    return os;
}

template<typename Key, typename Value, typename Number, class Rank, class Compare>
std::ostream &operator<<(std::ostream &os, const AVL::AVLRankTree<Value, Key, Rank, Compare, Number> *tree) {
    tree->PrintTree(os);
    os << std::endl;
    return os;
}

#endif
