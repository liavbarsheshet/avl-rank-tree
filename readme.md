# C++ AVL Rank Tree
Generic AVL (Balanced) Rank Tree written in c++.

---

## Declaration

Supports >= c++11:

```c++
#include "avl.hpp"

auto avl_tree = AVL::AVLRankTree<Key,Value>();
```

## Template

```c++
/**
 * Class: Represents the entire AVL Rank Tree.
 * @tparam Key - The type/class of the key.
 * @tparam Value - The type/class of the value.
 * @tparam RankInfo - Inherited Rank Class.
 * @tparam Compare - Compare Function Object.
 * @tparam Number - Class/Primitive for numbers representation.
 */
template<typename Key, typename Value, typename Number, class RankInfo, class Compare>
class AVL::AVLRankTree{...}
```

## Enums

```c++
// Represents a compare result.
typedef enum {
    LESS_THAN = -1, EQUAL = 0, GREATER_THAN = 1
} COMPARE_RESULT;

// Represents a node position in the tree.
typedef enum {
    ROOT, LEFT_CHILD, RIGHT_CHILD
} NODE_POSITION;

```

## RankInfo

Create your own custom rank class.

_Every rank info class must be in that pattern._

```c++
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

    DefaultRank();

    DefaultRank(Key key, Value value);

    DefaultRank(const DefaultRank<Key, Value, Number> &_rank);

    ~DefaultRank();

    AVL::DefaultRank<Key, Value, Number> &operator=(const DefaultRank<Key, Value, Number> &_rank) ;

    AVL::DefaultRank<Key, Value, Number> &operator-=(const DefaultRank<Key, Value, Number> &_rank);

    AVL::DefaultRank<Key, Value, Number> &operator+=(const DefaultRank<Key, Value, Number> &_rank);

    std::ostream &Print(std::ostream &os) const ;
};
```

## Compare Function

Create comparing function objects between custom ds as keys.

_Every comparing function object must be inherited from this class._

```c++

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
    virtual AVL::COMPARE_RESULT operator()(const Key key1, const Key key2) const;
};

```

## Filter Object

use to filter elements from queries.

```c++
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
};
```

## Methods

Implementation of a Generic AVL(Balanced) Rank Tree.

```cpp
    /**
     * Constructor: Constructs an empty AVL rank tree.
     * @note Worst-Time Complexity: O(1).
     */
    AVLRankTree();

    /**
     * Copy Constructor: Creates a copy from an existing AVL rank tree.
     * @note Worst-Time Complexity: O(n).
     * @param tree - AVL rank tree as a reference.
     */
    AVLRankTree(const AVLRankTree &tree);

    /**
     * Combine Constructor: Creates a joined AVL rank tree from two given AVL rank trees.
     * @note Worst-Time Complexity: O(n) - n=max{size1,size2}.
     * @note Worst-Space Complexity: O(n) - n=max{size1,size2}.
     * @param first_tree - AVL rank tree as a reference.
     * @param second_tree - AVL rank tree as a reference.
     */
    AVLRankTree(const AVL::AVLRankTree<Key, Value, Number, RankInfo, Compare> &first_tree,
                const AVL::AVLRankTree<Key, Value, Number, RankInfo, Compare> &second_tree);

    /**
     * Destructor: Deallocates the entire class.
     * @note Worst-Time Complexity: O(n).
     */
    ~AVLRankTree();

    /**
     * Gets the AVL rank tree size.
     * @note Worst-Time Complexity: O(1).
     * @return {Number} Tree size.
     */
    Number GetSize();

    /**
     * Gets the AVL rank tree height.
     * @note Worst-Time Complexity: O(1).
     * @return {Number} Tree height.
     */
    Number GetHeight();

    /**
     * Gets the index of a specific elements by key.
     * @note Worst-Time Complexity: O(log(n)).
     * @param key - The element key.
     * @return {Number} Index of an element as if it was in a sorted array.
     */
    Number GetIndexOfKey(const Key &key);

    /**
     * Gets the Max element by key.
     * @note Worst-Time Complexity: O(1).
     * @return {KeyValuePair<Key, Value>} The maximum element or NULL if the tree is empty.
     */
    KeyValuePair<Key, Value> *GetMax() const;

    /**
     * Gets the Min element by key.
     * @note Worst-Time Complexity: O(1).
     * @return {KeyValuePair<Key, Value>} The minimum element or NULL if the tree is empty.
     */
    KeyValuePair<Key, Value> *GetMin();

    /**
     * Find an element by its key.
     * @note Worst-Time Complexity: O(log(n)).
     * @param key - The element key.
     * @return {KeyValuePair<Key, Value>} element or NULL if not found.
     */
    KeyValuePair<Key, Value> *Find(const Key key);

    /**
     * Find an element by its index.
     * @note Worst-Time Complexity: O(log(n)).
     * @param index - The element index as if it was in a sorted array.
     * @return {KeyValuePair<Key, Value>} element or NULL if not found.
     */
    KeyValuePair<Key, Value> *FindIndex(const Number &index) const;

    /**
     * Find the closest element to a specific key.
     * @note Worst-Time Complexity: O(log(n)).
     * @param key -  Key that defines the range.
     * @param range - Defines which key closer to the key (LESS_THAN|GREATER_THAN) Default: LESS_THAN.
     * @return {KeyValuePair<Key, Value>} element or NULL if not found.
     */
    KeyValuePair<Key, Value> *Closest(const Key key, COMPARE_RESULT range = LESS_THAN);

    /**
     * Insert new element to the tree.
     * @note Worst-Time Complexity: O(log(n)).
     * @param key - The element key.
     * @param value - The element value.
     */
    void Insert(const Key key, const Value value);

    /**
     * Removes an element from the tree.
     * @note Worst-Time Complexity: O(log(n)).
     * @param key - The element key.
     * @return {bool} True if removed o.w False.
     */
    bool Remove(const Key key);

    /**
     * Collect relative rank within a given filter object.
     * @note the rank here will be considered as number of elements.
     * @note Worst-Time Complexity: O(log(n)).
     * @param filter - Filter object which contains information considering the traverse.
     * @return {RankInfo} an object containing collective rank information.
     */
    RankInfo *
    CollectRank(const AVL::FilterObject<Key, Value, Number> &filter =
    AVL::FilterObject<Key, Value, Number>()) const;

    /**
     * Find an element by its index.
     * @note Worst-Time Complexity: O(log(n)).
     * @param index - The element index as if it was in a sorted array.
     * @return {KeyValuePair<Key, Value>} element or NULL if not found.
     */
    KeyValuePair<Key, Value> *operator[](const Number &index);

    /**
   * Collect elements within a given filter object.
   * @note Worst-Time Complexity: O(n).
   * @note Worst-Space Complexity: O(n).
   * @param filter - Filter object which contains information considering the traverse.
   * @return {QueryResult<Key, Value, Number>} an object containing result array and total amount of elements.
   */
    QueryResult<Key, Value, Number>
    Query(const AVL::FilterObject<Key, Value, Number> &filterObject =
    AVL::FilterObject<Key, Value, Number>()) const;

    /**
    * Prints the entire tree.
    * @note Worst-Time Complexity: O(n).
    * @param filter - Filter object which contains information considering the traverse.
    */
    std::ostream &PrintTree(std::ostream &os) const;
```

## Author

[Liav Barsheshet, LBDevelopments](https://github.com/liavbarsheshet)

## License

[MIT](LICENSE)
