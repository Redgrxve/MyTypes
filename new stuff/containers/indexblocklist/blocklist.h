
#include <cstddef>
#include <cstdint>
#include <array>
#include <algorithm>
#include <stdexcept>
#include <utility>

namespace Details {

struct BlockListNodeBase {
    BlockListNodeBase() = default;

    BlockListNodeBase *next{this};
    BlockListNodeBase *prev{this};
};

template <typename T>
class BlockListNode : public BlockListNodeBase
{
public:
    static constexpr uint16_t SIZE = 4;

    uint16_t count()  const { return count_; }
    bool     filled() const { return count_ == SIZE; }

    template <typename U>
    void set(size_t pos, U &&item) {
        items_[pos] = std::forward<U>(item);
    }

    T &at(size_t pos) {
        if (pos >= count_)
            throw std::out_of_range("Index out of bounds;");
        return items_[pos];
    }

    T &last() {
        return items_[SIZE - 1];
    }

    template <typename U>
    void pushFront(U &&item) {
        if (count_ >= SIZE)
            throw std::out_of_range("Index out of bounds;");

        if (count_ > 0)
            shiftRight();

        items_[0] = std::forward<U>(item);
        ++count_;
    }

    template <typename U>
    void pushBack(U &&item) {
        if (count_ >= SIZE)
            throw std::out_of_range("Index out of bounds;");

        items_[count_] = std::forward<U>(item);
        ++count_;
    }

    template <typename U>
    void push(size_t pos, U &&item) {
        if (count_ >= SIZE)
            throw std::out_of_range("Index out of bounds;");

        if (count_ > 0)
            shiftRight(pos);

        items_[pos] = std::forward<U>(item);
        ++count_;
    }

    void shiftRight(size_t start = 0) {
        for (int i = SIZE - 1; i > start; --i)
            items_[i] = std::move_if_noexcept(items_[i - 1]);
    }

private:
    size_t count_{};
    std::array<T, SIZE> items_{};
};

/*template <typename NodeItemType, size_t CAPACITY>
class IndexTable
{
    using NodeBase = Details::BlockListNodeBase;
    using Node     = Details::BlockListNode<NodeItemType>;

    struct TableRow {
        size_t index{};
        Node *node{};
    };

public:
    IndexTable() = default;

    //       Node *operator[](size_t pos)       { return table_[pos].second(); }
    // const Node *operator[](size_t pos) const { return table_[pos].second(); }

    size_t count() const   { return table_.size_; }
    bool filled() const  { return table_.size() == CAPACITY; }

    bool contains(size_t index) const { return table_.find({index, nullptr}) != table_.end(); }

    const TableRow &findClosest(size_t index) {
        if (table_.empty())
            throw std::out_of_range("Table is empty");

        auto it = table_.lower_bound({index + 1, nullptr});
        --it;

        return *it;
    }

    void insert(size_t index, Node *node) {
        if (table_.size() >= CAPACITY)
            throw std::out_of_range("Table is filled");

        table_.insert({index, node});
    }

    void update(size_t index, Node *node, bool incrementOther = true) {
        auto it = table_.find({index, nullptr});
        if (it == table_.end())
            throw std::out_of_range("Position not found in table");

        table_.erase(it);
        table_.insert({index, node});

        if(incrementOther) {
            ++it;
            while (it != table_.end()) {
                ++it->index;
                ++it;
            }
        }
    }

    void erase(size_t index) {
        table_.erase(index);
    }

    void clear() {
        table_.clear();
    }

private:
    struct RowComparator {
        bool operator()(const TableRow &a, const TableRow &b) const {
            return a.index < b.index;
        }
    };

    std::set<TableRow, RowComparator> table_{};
};*/

template <typename NodeItemType, size_t CAPACITY>
class IndexTable
{
    using NodeBase   = Details::BlockListNodeBase;
    using Node       = Details::BlockListNode<NodeItemType>;

private:
    struct TableRow {
        size_t index{};
        Node *item{};
    };

    std::array<TableRow, CAPACITY> table_{};
    uint16_t count_{};

public:
    IndexTable() = default;

          TableRow &operator[](size_t i)       { return table_[i]; }
    const TableRow &operator[](size_t i) const { return table_[i]; }

    bool contains(size_t index) const { return table_.find({index, nullptr}) != table_.end(); }

    size_t count() const   { return table_.size_; }
    bool filled() const  { return count_ >= CAPACITY; }

    const TableRow &findClosest(size_t index) {
        if (table_.empty())
            throw std::out_of_range("Table is empty");

        auto it = std::lower_bound(table_.begin(), table_.end(),
                                   {index + 1, nullptr},
                                   [](const TableRow &a, const TableRow &b)
                                   { return a.index < b.index; });
        --it;

        return *it;
    }

    void insert(size_t index, Node *item) {
        if (count_ == CAPACITY)
            throw std::out_of_range("Index out of bounds!");

        table_[count_++] = {index, item};
    }

    void update(size_t index, Node *item) {
        table_[index] = item;
    }

    void erase(size_t index) {
        table_.erase(index);
    }

    void clear() {
        table_.clear();
        count_ = 0;
    }
 };


/* template <typename NodeItemType, size_t CAPACITY>
// class IndexTable
// {
//     using NodeBase   = Details::BlockListNodeBase;
//     using Node       = Details::BlockListNode<NodeItemType>;

// public:
//     IndexTable() = default;

//           Node*  operator[](size_t index)       { return table_[index]; }
//     const Node*  operator[](size_t index) const { return table_[index]; }

//     size_t count()    const  { return table_.size(); }
//     bool   isFilled() const  { return table_.size() >= CAPACITY; }

//     const auto begin() const { return table_.begin(); }
//     const auto end()   const { return table_.end(); }

//     void insert(size_t index, Node *item) {
//         if (table_.size() >= CAPACITY)
//             throw std::out_of_range("Table is filled!");

//         if (table_.count(index) > 0) return;

//         table_[index] = item;
//     }

//     void update(size_t index, Node *item) {
//         table_[index] = item;
//     }

//     void erase(size_t index) {
//         table_.erase(index);
//     }

//     void clear() {
//         table_.clear();
//     }

// private:
//     std::unordered_map<size_t, Node*> table_{};
};
*/
}

template <typename T>
class BlockList
{
    static constexpr uint16_t TABLE_CAP = 7;
    using IndexTable = Details::IndexTable<T, TABLE_CAP>;

    using NodeBase   = Details::BlockListNodeBase;
    using Node       = Details::BlockListNode<T>;

public:
    BlockList()
        : size_(0), sent_(new NodeBase) {}

    BlockList(const BlockList &other)
        : size_(0), sent_(new NodeBase)
    {
        for (const T &el : other)
            insertBack(el);
    }

    BlockList(BlockList &&other) noexcept
        : size_(other.size_), sent_(other.sent_)
    {
        other.sent_ = new NodeBase;
        other.size_ = 0;
    }

    BlockList(const std::initializer_list<T> &initList)
        : size_(0), sent_(new NodeBase)
    {
        for (const T &el : initList)
            insertBack(el);
    }

    ~BlockList() {
        clear();
        delete sent_;
    }

    T &operator[](size_t pos) {
        if (pos >= size_)
            throw std::out_of_range("Index out of bounds");

        Node *node = findNode(pos);
        return node->at(pos);
    }

    BlockList &operator=(const BlockList &other) {
        if (this == &other) return *this;

        clear();
        for (const T &el : other)
            insertBack(el);

        return *this;
    }

    BlockList &operator=(BlockList &&other) noexcept {
        if (this == &other) return *this;

        clear();
        delete sent_;

        size_ = other.size_;
        sent_ = other.sent_;
        other.sent_ = new NodeBase;
        other.size_ = 0;

        return *this;
    }

    BlockList &operator=(const std::initializer_list<T> &initList) {
        clear();
        for (const T &el : initList)
            insertBack(el);

        return *this;
    }

    size_t size()  const { return size_; }
    bool   empty() const { return sent_->next == sent_; }

    template <typename U>
    void insertFront(U &&item) {
        ++size_;

        Node *head = static_cast<Node *>(sent_->next);
        if (empty()) {
            initFirstNode(std::forward<U>(item));
            return;
        }

        if (!head->filled()) {
            head->pushFront(std::forward<U>(item));
            return;
        }

        Node *add = new Node;
        add->pushFront(std::forward<U>(item));

        insertNodeBetween(sent_, add, head);

        if (table_.contains(0))
            table_.update(0, add);
        else if (!table_.filled())
            table_.insert(0, add);
    }

    template <typename U>
    void insertBack(U &&item) {
        ++size_;

        if (empty()) {
            initFirstNode(std::forward<U>(item));
            return;
        }

        Node *tail = static_cast<Node *>(sent_->prev);
        if (!tail->filled()) {
            tail->pushBack(std::forward<U>(item));
            return;
        }

        Node *add = new Node;
        add->pushBack(std::forward<U>(item));

        insertNodeBetween(tail, add, sent_);

        if (!table_.filled())
            table_.insert(size_ - 1, add);
    }

    template <typename U>
    void insert(size_t pos, U &&item) {
        if (pos > size_)
            throw std::out_of_range("Index out of bounds");

        if (pos == 0) {
            insertFront(std::forward<U>(item));
            return;
        }

        if (pos == size_) {
            insertBack(std::forward<U>(item));
            return;
        }

        ++size_;

        Node *curr = findNode(pos);
        if (!curr->filled()) {
            curr->push(pos, std::forward<U>(item));
            return;
        }

        T lastItem = std::move(curr->last());
        curr->shiftRight(pos);
        curr->set(pos, std::forward<U>(item));

        Node *next = static_cast<Node *>(curr->next);
        if (!next->filled()) {
            next->pushFront(std::move(lastItem));
        } else {
            Node *add = new Node;
            add->pushBack(std::move(lastItem));

            insertNodeBetween(curr, add, curr->next);

            if (!table_.filled())
                table_.insert(pos, add);
        }
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

        table_.clear();

        size_ = 0;
    }

private:
    void insertNodeBetween(NodeBase *before, NodeBase *node, NodeBase *after) {
        node->next = after;
        node->prev = before;

        after->prev = node;
        before->next = node;
    }

    template <typename U>
    void initFirstNode(U &&item) {
        Node *add = new Node;
        add->pushFront(std::forward<U>(item));

        insertNodeBetween(sent_, add, sent_);

        if (!table_.filled())
            table_.insert(size_ - 1, add);
    }

    // Node *findNode(size_t &pos) {
    //     Node *res = static_cast<Node *>(sent_->next);

    //     while (res != sent_) {
    //         if (pos < res->count())
    //             return res;

    //         pos -= res->count();
    //         res = static_cast<Node *>(res->next);
    //     }

    //     return sent_;
    // }

    Node *findNode(size_t &pos) {
        auto found = table_.findClosest(pos);
        Node *curr = found.node;
        size_t index = found.index;

        while (index + curr->count() <= pos) {
            index += curr->count();
            curr = static_cast<Node*>(curr->next);
        }

        pos -= index;
        return curr;
    }

    size_t size_;
    NodeBase *sent_;
    IndexTable table_{};
};

/*template <typename U>
    void insertFront(U &&item) {
        // Node *head = static_cast<Node *>(sent_->next);
        // ++size_;
        // if (head == sent_) {
        //     addFirst(std::forward<U>(item));
        //     return;
        // }

        // if (!head->isFilled()) {
        //     head->pushFront(std::forward<U>(item));
        //     return;
        // }
        // Node *next = static_cast<Node *>(head->next);
        // if (next == sent_) {
        //     next = new Node;

        //     next->next = head->next;
        //     next->prev = head;

        //     head->next = next;
        // }

        // if (!next->isFilled()) {
        //     T last = head->items[head->size - 1];
        //     head->pushFront(std::forward<U>(item));
        //     next->pushFront(std::forward<U>(last));
        // } else {
        //     Node *add = new Node;
        //     add->pushFront(std::forward<U>(item));

        //     add->next = head;
        //     add->prev = sent_;

        //     head->prev = add;

        //     sent_->next = add;
        // }
    }*/

