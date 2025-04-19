
#include <cstddef>
#include <iostream>
#include <utility>

template<typename IteratorType>
class ReverseIterator
{
    using T = IteratorType;

private:
    T it_;
};

namespace Details {

struct ListNodeBase {
    ListNodeBase() = default;

    ListNodeBase *next{this};
    ListNodeBase *prev{this};
};

template <typename T>
struct ListNode : public ListNodeBase
{
    template <typename U>
    ListNode(U &&val)
        : value(std::forward<U>(val)) {}

    T value{};
};

}

template <typename T>
class NList
{
    using NodeBase = Details::ListNodeBase;
    using Node     = Details::ListNode<T>;

public:
    class ConstIterator
    {
        friend class NList;

    public:
        ConstIterator() = default;

        //prefix
        ConstIterator &operator++() {
            node_ = node_->next;
            return *this;
        }

        //postfix
        ConstIterator operator++(int) {
            auto tmp = *this;
            node_ = node_->next;
            return tmp;
        }

        //prefix
        ConstIterator &operator--() {
            node_ = node_->prev;
            return *this;
        }

        //postfix
        ConstIterator operator--(int) {
            auto tmp = *this;
            node_ = node_->prev;
            return tmp;
        }

        const T &operator*() const {
            return static_cast<Node*>(node_)->value;
        }

        bool operator!=(const ConstIterator &other) const {
            return node_ != other.node_;
        }

        bool operator==(const ConstIterator &other) const {
            return node_ == other.node_;
        }

    protected:
        explicit ConstIterator(NodeBase *node)
            : node_(node) {}

        NodeBase *node_{};
    };

    class Iterator : public ConstIterator
    {
        friend class NList;

        using ConstIterator::node_;

    public:
        Iterator() : ConstIterator() {}

        //prefix
        Iterator &operator++() {
            node_ = node_->next;
            return *this;
        }

        //postfix
        Iterator operator++(int) {
            auto tmp = *this;
            node_ = node_->next;
            return tmp;
        }

        //prefix
        Iterator &operator--() {
            node_ = node_->prev;
            return *this;
        }

        //postfix
        Iterator operator--(int) {
            auto tmp = *this;
            node_ = node_->prev;
            return tmp;
        }

        T &operator*() {
            return static_cast<Node*>(node_)->value;
        }

    private:
        explicit Iterator(NodeBase *node)
            : ConstIterator(node) {}
    };

    using ReverseIterator      = std::reverse_iterator<NList<T>::Iterator>;
    using ConstReverseIterator = std::reverse_iterator<NList<T>::ConstIterator>;

public:
    NList()
        : size_(0), sent_(new NodeBase) {}

    NList(const NList &other)
        : size_(0), sent_(new NodeBase)
    {
        for (const T &el : other)
            insertBack(el);
    }

    NList(NList &&other) noexcept
        : size_(other.size_), sent_(other.sent_)
    {
        other.sent_ = new NodeBase;
        other.size_ = 0;
    }

    NList(const std::initializer_list<T> &initList)
        : size_(0), sent_(new NodeBase)
    {
        for (const T &el : initList)
            insertBack(el);
    }

    ~NList() {
        clear();
        delete sent_;
    }
    
    NList &operator=(const NList &other) {
        if (this == &other) return *this;

        clear();
        for (const T &el : other)
            insertBack(el);

        return *this;
    }

    NList &operator=(NList &&other) noexcept {
        if (this == &other) return *this;

        clear();
        delete sent_;

        size_ = other.size_;
        sent_ = other.sent_;
        other.sent_ = new NodeBase;
        other.size_ = 0;

        return *this;
    }

    NList &operator=(const std::initializer_list<T> &initList) {
        clear();
        for (const T &el : initList)
            insertBack(el);

        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const NList &list) {
        for (auto &el : list)
            os << el << "\n";
        return os;
    }

    ConstIterator begin()  const { return ConstIterator(sent_->next); }
    Iterator      begin()        { return Iterator(sent_->next); }

    ConstIterator end()    const { return ConstIterator(sent_); }
    Iterator      end()          { return Iterator(sent_); }

    ConstIterator cbegin() const { return ConstIterator(sent_->next); }
    ConstIterator cend()   const { return ConstIterator(sent_); }

    /* ConstReverseIterator crbegin() const { return ConstReverseIterator(cbegin()); }
    // ConstReverseIterator crend()   const { return ConstReverseIterator(cend()); }

    // ConstReverseIterator rbegin()  const { return ConstReverseIterator(cbegin()); }
    // ReverseIterator      rbegin()        { return ConstReverseIterator(begin()); }

    // ConstReverseIterator rend()    const { return ConstReverseIterator(cend()); }
     ReverseIterator      rend()          { return ConstReverseIterator(end()); }*/


    size_t size()  const { return size_; }
    bool   empty() const { return sent_->next == sent_; }


    template <typename U>
    void insertFront(U &&item) {
        Node *add = new Node(std::forward<U>(item));

        add->next = sent_->next;
        add->prev = sent_;

        sent_->next = add;

        ++size_;
    }

    template <typename U>
    void insertBack(U &&item) {
        Node *add = new Node(std::forward<U>(item));
        NodeBase *tail = sent_->prev;

        add->next = sent_;
        add->prev = sent_->prev;

        tail->next = add;
        sent_->prev = add;

        ++size_;
    }

    template <typename U>
    void insert(ConstIterator it, U &&item) {
        Node *add = new Node(std::forward<U>(item));
        NodeBase *cur = it.node_;

        add->next = cur;
        add->prev = cur->prev;

        cur->prev->next = add;
        cur->prev = add;

        ++size_;
    }

    void clear() {
        auto current = static_cast<Node*>(sent_->next);
        while (current != sent_) {
            auto next = static_cast<Node*>(current->next);
            delete current;
            current = next;
        }

        sent_->next = sent_;
        sent_->prev = sent_;
        size_ = 0;
    }

private:
    size_t size_;
    NodeBase *sent_;
};
