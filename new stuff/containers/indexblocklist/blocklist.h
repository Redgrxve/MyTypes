
#include <cstddef>
#include <cstdint>
#include <array>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

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

template <typename NodeItemType, size_t CAPACITY>
class IndexTable
{
    using NodeBase   = Details::BlockListNodeBase;
    using Node       = Details::BlockListNode<NodeItemType>;

private:
    struct TableRow {
        size_t index{};
        Node *node{};
    };

    std::vector<TableRow> table_{};

public:
    IndexTable() { table_.reserve(CAPACITY); }
    IndexTable(const IndexTable &other) = default;
    IndexTable(IndexTable &&other) : table_(std::move(other.table_)) {}


    IndexTable &operator=(const IndexTable &other) = default;
    IndexTable &operator=(IndexTable &&other) noexcept {
        table_ = std::move(other.table_);
        return *this;
    }

          TableRow &operator[](size_t i)       { return table_[i]; }
    const TableRow &operator[](size_t i) const { return table_[i]; }

    bool contains(size_t index) const {
        const auto pred = [index](const TableRow &row){
            return index == row.index;
        };
        const auto it = std::find_if(table_.begin(), table_.end(), pred);
        return it != table_.end();
    }

    size_t capacity() const { return CAPACITY; }
    size_t count()    const { return table_.size(); }
    bool   filled()   const { return table_.size() >= CAPACITY; }
    bool   empty()    const { return table_.size() == 0; }

    const TableRow &findClosest(size_t index) const {
        if (empty())
            throw std::out_of_range("Table is empty");

        const auto pred =[](const TableRow &row, size_t index) {
            return row.index < index;
        };
        auto it = std::lower_bound(table_.begin(), table_.end(), index + 1, pred);
        --it;

        return *it;
    }

    void insert(size_t index, Node *node) {
        if (filled())
            throw std::out_of_range("Table is filled");

        table_.push_back({index, node});
        std::sort(table_.begin(), table_.end(), [](const TableRow &a, const TableRow &b) {
            return a.index < b.index;
        });
    }

    void update(size_t elemIndex, size_t newIndex, Node *newNode) {
        table_[elemIndex] = {newIndex, newNode};
    }

    void incrementAll() {
        for (auto &el : table_) {
            ++el.index;
        }
    }

    void incrementAfter(size_t index) {
        const auto pred = [index](const TableRow &row) {
            return row.index > index;
        };
        auto it = std::find_if(table_.begin(), table_.end(), pred);
        while (it != table_.end()) {
            ++it->index;
            ++it;
        }
    }

    void erase(size_t index) {
        table_.erase(index);
    }

    void clear() {
        table_.clear();
    }
 };

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
        for (size_t i = 0; i < other.size_; ++i)
            insertBack(other[i]);
    }

    BlockList(BlockList &&other) noexcept
        : size_(other.size_), sent_(other.sent_), table_(std::move(other.table_))
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

    const T &operator[](size_t pos) const {
        if (pos >= size_)
            throw std::out_of_range("Index out of bounds");

        Node *node = findNode(pos, pos);
        return node->at(pos);
    }

    T &operator[](size_t pos) {
        if (pos >= size_)
            throw std::out_of_range("Index out of bounds");

        Node *node = findNode(pos, pos);
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
        table_ = std::move(other.table_);
        other.sent_ = new NodeBase;
        other.size_ = 0;
        other.table_.clear();

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
            table_.incrementAfter(0);
            return;
        }

        Node *add = new Node;
        add->pushFront(std::forward<U>(item));

        insertNodeBetween(sent_, add, head);

        if (!table_.filled()) {
            table_.incrementAll();
            table_.insert(0, add);
        } else {
            table_.update(0, 0, add);
            table_.incrementAfter(0);
        }
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

        size_t indexInNode{};
        Node *curr = findNode(pos, indexInNode);
        if (!curr->filled()) {
            curr->push(indexInNode, std::forward<U>(item));
            table_.incrementAfter(pos);
            return;
        }

        T lastItem = std::move(curr->last());
        curr->shiftRight(indexInNode);
        curr->set(indexInNode, std::forward<U>(item));

        Node *next = static_cast<Node *>(curr->next);
        if (!next->filled()) {
            next->pushFront(std::move(lastItem));
            table_.incrementAfter(pos);
        } else {
            Node *add = new Node;
            add->pushBack(std::move(lastItem));

            insertNodeBetween(curr, add, curr->next);

            table_.incrementAfter(pos);
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

    void rebuildIndexTable() {
        if (empty()) return;
        table_.clear();

        size_t step = (size_ + table_.capacity() - 1) / table_.capacity(); // равномерный шаг

        size_t currIndex{};
        size_t insertIndex{};

        Node* node = static_cast<Node*>(sent_->next);
        size_t nodeIndex{};
        while (node != sent_) {
            for (size_t i = 0; i < node->count(); ++i) {
                if (currIndex >= insertIndex && !table_.filled()) {
                    table_.insert(nodeIndex, node);
                    insertIndex += step;
                    break;
                }
                ++currIndex;
            }
            node = static_cast<Node*>(node->next);
            nodeIndex += node->count();
        }
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

        table_.insert(0, add);
    }

    Node *findNode(size_t pos, size_t &indexInNode) const {
        auto found = table_.findClosest(pos);
        Node *curr = found.node;
        size_t index = found.index;

        while (index + curr->count() <= pos) {
            index += curr->count();
            curr = static_cast<Node*>(curr->next);
        }

        indexInNode = pos - index;
        return curr;
    }

    size_t size_;
    NodeBase *sent_;
    IndexTable table_{};
};

