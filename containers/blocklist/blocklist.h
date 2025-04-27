
#include <cstddef>
#include <cstdint>
#include <iostream>
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
    static constexpr uint16_t size = 4;

    template <typename U>
    void set(size_t pos, U &&item) {
        items_[pos] = std::forward<U>(item);
    }

    T &at(size_t pos) {
        if (pos >= count_)
            throw std::out_of_range("Index out of bounds;");
        return items_[pos];
    }

    uint16_t count() const { return count_; }
    bool isFilled() const { return count_ == size; }

    template <typename U>
    void pushFront(U &&item) {
        if (count_ >= size)
            throw std::out_of_range("Index out of bounds;");

        if (count_ > 0)
            shiftRight();

        items_[0] = std::forward<U>(item);
        ++count_;
    }

    template <typename U>
    void pushBack(U &&item) {
        if (count_ >= size)
            throw std::out_of_range("Index out of bounds;");

        items_[count_] = std::forward<U>(item);
        ++count_;
    }

    template <typename U>
    void push(size_t pos, U &&item) {
        if (count_ >= size)
            throw std::out_of_range("Index out of bounds;");

        if (count_ > 0)
            shiftRight(pos);

        items_[pos] = std::forward<U>(item);
        ++count_;
    }

    void shiftRight(size_t start = 0) {
        for (int i = size - 1; i > start; --i)
            items_[i] = std::move_if_noexcept(items_[i - 1]);
    }

private:
    uint16_t count_{};
    std::array<T, size> items_{};
};

}

template <typename T>
class BlockList
{
    using NodeBase = Details::BlockListNodeBase;
    using Node     = Details::BlockListNode<T>;

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
        Node* current = findNode(pos);
        size_t blockIndex = 0;
        return current->at(pos);
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

    friend std::ostream &operator<<(std::ostream &os, const BlockList &list) {
        for (auto &el : list)
            os << el << "\n";
        return os;
    }

    size_t size()  const { return size_; }
    bool   empty() const { return sent_->next == sent_; }


    template <typename U>
    void insertFront(U &&item) {
        Node *head = static_cast<Node *>(sent_->next);
        ++size_;
        if (head == sent_) {
            initFirstNode(std::forward<U>(item));
            return;
        }

        if (!head->isFilled()) {
            head->pushFront(std::forward<U>(item));
            return;
        }

        Node *add = new Node;
        add->pushFront(std::forward<U>(item));

        add->next = head;
        add->prev = sent_;

        head->prev = add;
        sent_->next = add;
    }

    template <typename U>
    void insertBack(U &&item) {
        Node *tail = static_cast<Node *>(sent_->prev);
        ++size_;
        if (tail == sent_) {
            initFirstNode(std::forward<U>(item));
            return;
        }

        if (!tail->isFilled()) {
            tail->pushBack(std::forward<U>(item));
            return;
        }

        Node *add = new Node;
        add->pushBack(std::forward<U>(item));

        add->next = sent_;
        add->prev = sent_->prev;

        tail->next = add;
        sent_->prev = add;
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

        Node *curr = findNode(pos);
        ++size_;

        if (!curr->isFilled()) {
            curr->push(pos, std::forward<U>(item));
            return;
        }

        T lastItem = curr->at(curr->size - 1);
        curr->shiftRight(pos);
        curr->set(pos, std::forward<U>(item));

        Node *next = static_cast<Node *>(curr->next);
        if (!next->isFilled()) {
            next->pushFront(std::move(lastItem));
        } else {
            Node *add = new Node;
            add->pushBack(std::move(lastItem));

            add->next = curr->next;
            add->prev = curr;

            curr->next->prev = add;
            curr->next = add;
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
        size_ = 0;
    }

private:
    template <typename U>
    void initFirstNode(U &&item) {
        Node *add = new Node;
        add->pushFront(std::forward<U>(item));

        add->next = sent_;
        add->prev = sent_;

        sent_->next = add;
        sent_->prev = add;
    }

    Node *findNode(size_t &pos) {
        Node *res = static_cast<Node *>(sent_->next);
        while (res != sent_) {
            if (pos < res->count())
                return res;

            pos -= res->count();
            res = static_cast<Node *>(res->next);
        }
        return res;
    }

    size_t size_;
    NodeBase *sent_;
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

