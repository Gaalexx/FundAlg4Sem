#include <queue>       
#include <functional>  
#include <cstddef>     
#include <exception>   
#include <compare> 
#include <iostream>

template <typename T>
class IComparator
{
public:
    virtual ~IComparator() = default;
    virtual bool compare(const T &a, const T &b) const = 0;
};

template <typename T>
class DefaultComparator : public IComparator<T>
{
public:
    bool compare(const T &a, const T &b) const override final { return std::less<T>()(a, b); }
};

template <typename T>
class GreaterComparator : public IComparator<T>
{
public:
    bool compare(const T &a, const T &b) const override final { return std::greater<T>()(a, b); }
};

template <typename Key, typename Value, template <typename> typename Comporator = DefaultComparator>
class BST
{
protected:
    Comporator<Key> comp = Comporator<Key>();
    class Node
    {
        friend class InorderIterator;
        friend class BST;

        Key key;
        Value value;
        Node *left = nullptr, *right = nullptr;
        Node() {}
        Node(const Key &_key, const Value &_value) : key(_key), value(_value) {}
        Node(const Key &_key, const Value &_value, const Node *&_left, const Node *&_right) : key(_key), value(_value)
        {
            left = _left;
            right = _right;
        }

        auto operator<=>(const Node &other)
        {

            if (comp.compare(key, other.key))
            {
                return std::strong_ordering::less;
            }
            else
            {
                return std::strong_ordering::greater;
            }
        }

        bool operator==(const Node &other) const = default;
        bool operator!=(const Node &other) const = default;
        bool operator<(const Node &other) const = default;
        bool operator<=(const Node &other) const = default;
        bool operator>(const Node &other) const = default;
        bool operator>=(const Node &other) const = default;

        virtual ~Node() = default;
    };

public:
    class InorderIterator
    {
        friend class BST;

    protected:
        Node *_node;
        std::queue<Node *> allNodes;

    public:
        InorderIterator()
        {
            _node = nullptr;
        }
        InorderIterator(Node *node)
        {
            _node = node;
        }

        InorderIterator &operator++()
        {
            if (!allNodes.empty())
            {
                _node = allNodes.front();
                allNodes.pop();
            }
            else
            {
                _node = nullptr;
            }
            return *this;
        }

        InorderIterator operator++(int)
        {
            InorderIterator ret(*this);
            if (!allNodes.empty())
            {
                _node = allNodes.front();
                allNodes.pop();
            }
            else
            {
                _node = nullptr;
            }
            return ret;
        }

        Value operator*()
        {
            Value ret = this->_node->value;
            return ret;
        }

        const Value operator*() const
        {
            return this->_node->value;
        }

        bool operator!=(const InorderIterator &other)
        {
            if (this->_node == other._node)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        bool operator==(const InorderIterator &other)
        {
            if (this->_node == other._node)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        virtual ~InorderIterator() = default;
    };

    Node *_head;
    size_t _size;
    size_t _depth;

public:
    BST() : _head(nullptr), _size(0), _depth(0) {};

    InorderIterator begin()
    {
        Node *cur = _head;
        while (cur->left != nullptr)
        {
            cur = cur->left;
        }
        InorderIterator ret(cur);
        std::queue<Node *> *inQ = getInorderNodesQueue(*this);
        ret.allNodes = *inQ;
        if (!ret.allNodes.empty())
        {
            ret.allNodes.pop();
        }
        delete inQ;
        return ret;
    }

    InorderIterator end()
    {
        return InorderIterator(nullptr);
    }

    void clear()
    {
        recursive_clear(_head);
        _size = 0;
        return;
    }

    void inorderTraversal(std::function<void(const Key &, const Value &)> func) const
    {
        _inorderTraversal(_head, func);
    }

    void preorderTraversal(std::function<void(const Key &, const Value &)> func) const
    {
        _preorderTraversal(_head, func);
    }

    void postorderTraversal(std::function<void(const Key &, const Value &)> func) const
    {
        _postorderTraversal(_head, func);
    }

    void remove(const Key &key)
    {
        Node *toRemove = recursive_find(key, _head);
        if (!toRemove)
        {
            throw std::exception();
        }
        else
        {
            if (toRemove->left != nullptr)
            {

                Node *tmp = toRemove->left;
                Node *prev = toRemove;
                while (tmp->right != nullptr)
                {
                    prev = tmp;
                    tmp = tmp->right;
                }

                prev->right = nullptr;
                toRemove->key = tmp->key;
                toRemove->value = tmp->value;
                delete tmp;
            }
            else if (toRemove->right != nullptr)
            {

                Node *tmp = toRemove->right;
                Node *prev = toRemove;
                while (tmp->left != nullptr)
                {
                    prev = tmp;
                    tmp = tmp->left;
                }

                prev->left = nullptr;
                toRemove->key = tmp->key;
                toRemove->value = tmp->value;
                delete tmp;
            }
            else
            {
                delete toRemove; //здесь ошибка. нужно как-то узнать прошлую ноду
                toRemove = nullptr;
            }
            --_size;
        }
    }

    void insert(const Key &key, const Value &value)
    {
        ++_size;
        if (_head == nullptr)
        {
            _head = new Node(key, value);
            ++_depth;
        }
        else
        {
            size_t depth = 1;
            Node *toAdd = new Node(key, value);
            recursive_insert(toAdd, _head, _depth);
            _depth = depth;
        }
    }

    Value *find(const Key &key) const
    {
        auto res = recursive_find(key, _head);
        if(!res){
            return nullptr;
        }
        else{
            return &res->value;
        }
    }

    bool contains(const Key &key)
    {
        if (find(key) != nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    size_t size() const
    {
        return _size;
    }

    ~BST()
    {
        clear();
    }

private:
    void _inorderTraversal(Node *node, std::function<void(const Key &, const Value &)> func) const
    {
        if (!node)
            return;
        _inorderTraversal(node->left, func);
        func(node->key, node->value);
        _inorderTraversal(node->right, func);
    }

    void _preorderTraversal(Node *node, std::function<void(const Key &, const Value &)> func) const
    {
        if (!node)
            return;
        func(node->key, node->value);
        _preorderTraversal(node->left, func);
        _preorderTraversal(node->right, func);
    }

    void _postorderTraversal(Node *node, std::function<void(const Key &, const Value &)> func) const
    {
        if (!node)
            return;
        _postorderTraversal(node->left, func);
        _postorderTraversal(node->right, func);
        func(node->key, node->value);
    }

    Node *recursive_find(const Key &key, Node *cur) const
    {
        if (cur == nullptr)
        {
            return nullptr;
        }
        else
        {
            if (cur->key == key)
            {
                return cur;
            }
            else if (comp.compare(key, cur->key))
            {
                return recursive_find(key, cur->left);
            }
            else
            {
                return recursive_find(key, cur->right);
            }
        }
    }

    void recursive_clear(Node *node)
    {
        if (node == nullptr)
        {
            return;
        }
        recursive_clear(node->left);
        recursive_clear(node->right);
        delete node;
    }

    void recursive_insert(Node *toAdd, Node *cur, size_t &depth)
    {
        ++depth;
        if (comp.compare(toAdd->key, cur->key))
        {
            if (cur->left != nullptr)
            {
                recursive_insert(toAdd, cur->left, depth);
                return;
            }
            cur->left = toAdd;
        }
        else
        {
            if (cur->right != nullptr)
            {
                recursive_insert(toAdd, cur->right, depth);
                return;
            }
            cur->right = toAdd;
        }
    }

    static std::queue<Node *> *getInorderNodesQueue(const BST &tree)
    {
        std::queue<Node *> *inorderNodes = new std::queue<Node *>();

        _recursive_getInorderNodesQueue(tree._head, *inorderNodes);

        return inorderNodes;
    }

    static void _recursive_getInorderNodesQueue(Node *node, std::queue<Node *> &inorderNodes)
    {
        if (node == nullptr)
        {
            return;
        }
        _recursive_getInorderNodesQueue(node->left, inorderNodes);
        inorderNodes.push(node);
        _recursive_getInorderNodesQueue(node->right, inorderNodes);
    }
};

template <typename T, typename F>
using callback = std::function<void(const T &, const F &)>;

template <typename Key, typename Value, template <typename, typename, template <typename> typename> class TreeType = BST, template <typename> typename Comparator = DefaultComparator>
class MyMap
{
    TreeType<Key, Value, Comparator> _tree;

public:
    MyMap()
    {
        _tree = TreeType<Key, Value, Comparator>();
    }

    void insert(const Key &key, const Value &value)
    {
        _tree.insert(key, value);
    }

    Value *find(const Key &key)
    {
        return _tree.find(key);
    }

    void remove(const Key &key)
    {
        _tree.remove(key);
    }

    bool contains(const Key &key)
    {
        return _tree.contains(key);
    }

    void clear()
    {
        _tree.clear();
    }

    size_t size() const
    {
        return _tree.size();
    }

    BST<Key, Value, Comparator>::InorderIterator begin()
    {
        return _tree.begin();
    }
    BST<Key, Value, Comparator>::InorderIterator end()
    {
        return _tree.end();
    }

    void inorder(callback<Key, Value> func)
    {
        _tree.inorderTraversal(func);
        return;
    }

    void postorder(callback<Key, Value> func)
    {
        _tree.postorderTraversal(func);
        return;
    }

    void preoder(callback<Key, Value> func)
    {
        _tree.preorderTraversal(func);
        return;
    }
};