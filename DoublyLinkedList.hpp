
#ifndef MY_DOUBLY_LINKED_LIST_HPP
#define MY_DOUBLY_LINKED_LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

namespace CPSC131::DoublyLinkedList {

template <class T>
class DoublyLinkedList {
public:
    class Node {
    public:
        Node() : element_{}, prev_(nullptr), next_(nullptr) {}

        Node(T element)
            : element_(element), prev_(nullptr), next_(nullptr) {}

        Node(T element, Node* prev, Node* next)
            : element_(element), prev_(prev), next_(next) {}

        void setPrevious(Node* prev) { prev_ = prev; }
        void setPrev(Node* prev) { prev_ = prev; }

        Node* getPrevious() { return prev_; }
        Node* getPrev() { return prev_; }

        void setNext(Node* next) { next_ = next; }
        Node* getNext() { return next_; }

        void setElement(T element) { element_ = element; }
        T& getElement() { return element_; }
        T& operator*() { return element_; }

    private:
        T element_;
        Node* prev_;
        Node* next_;
    };

    class Iterator {
    public:
        Iterator() = default;

        Iterator(Node* head, Node* tail)
            : head_(head), tail_(tail), cursor_(nullptr) {}

        Iterator(Node* head, Node* tail, Node* cursor)
            : head_(head), tail_(tail), cursor_(cursor) {}

        Node* begin() { return head_; }
        Node* end() { return nullptr; }
        Node* getCursor() { return cursor_; }

        bool isAtEnd() { return cursor_ == nullptr; }

        Iterator& operator=(const Iterator& other) = default;

        bool operator==(const Iterator& other) {
            return cursor_ == other.cursor_;
        }

        bool operator!=(const Iterator& other) {
            return !(*this == other);
        }

        Iterator operator+(size_t add) {
            Iterator result = *this;
            result += add;
            return result;
        }

        Iterator operator-(size_t subtract) {
            Iterator result = *this;
            result -= subtract;
            return result;
        }

        Iterator& operator++() {
            if (cursor_) {
                cursor_ = cursor_->getNext();
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator old = *this;
            ++(*this);
            return old;
        }

        Iterator& operator--() {
            if (cursor_) {
                cursor_ = cursor_->getPrev();
            } else {
                cursor_ = tail_;
            }
            return *this;
        }

        Iterator operator--(int) {
            Iterator old = *this;
            --(*this);
            return old;
        }

        Iterator operator+=(size_t add) {
            for (size_t i = 0; i < add; ++i) {
                ++(*this);
            }
            return *this;
        }

        Iterator operator-=(size_t sub) {
            for (size_t i = 0; i < sub; ++i) {
                --(*this);
            }
            return *this;
        }

        Iterator operator+=(int add) {
            if (add >= 0) {
                return *this += static_cast<size_t>(add);
            }

            return *this -= static_cast<size_t>(
                -(static_cast<long long>(add))
            );
        }

        Iterator operator-=(int subtract) {
            if (subtract >= 0) {
                return *this -= static_cast<size_t>(subtract);
            }

            return *this += static_cast<size_t>(
                -(static_cast<long long>(subtract))
            );
        }

        T& operator*() {
            if (cursor_ == nullptr) {
                throw std::out_of_range(
                    "Cannot dereference end iterator"
                );
            }

            return cursor_->getElement();
        }

    private:
        Node* head_ = nullptr;
        Node* tail_ = nullptr;
        Node* cursor_ = nullptr;

        friend class DoublyLinkedList;
    };

    DoublyLinkedList() = default;

    DoublyLinkedList(const DoublyLinkedList& other) {
        for (Node* node = other.head_;
             node != nullptr;
             node = node->getNext()) {
            push_back(node->getElement());
        }
    }

    ~DoublyLinkedList() {
        clear();
    }

    void assign(size_t count, const T& value) {
        clear();

        for (size_t i = 0; i < count; ++i) {
            push_back(value);
        }
    }

    void assign(Iterator first, Iterator last) {
        std::vector<T> values;

        Node* current = first.getCursor();
        Node* stop = last.getCursor();

        while (current) {
            values.push_back(current->getElement());

            if (current == stop) {
                break;
            }

            current = current->getNext();
        }

        clear();

        for (const T& value : values) {
            push_back(value);
        }
    }

    Node* head() const {
        return head_;
    }

    Node* tail() const {
        return tail_;
    }

    Iterator begin() const {
        return Iterator(head_, tail_, head_);
    }

    Iterator last() const {
        return Iterator(head_, tail_, tail_);
    }

    Iterator end() const {
        return Iterator(head_, tail_, nullptr);
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    void clear() {
        while (head_) {
            Node* next = head_->getNext();
            delete head_;
            head_ = next;
        }

        tail_ = nullptr;
        size_ = 0;
    }

    Iterator insert_after(Iterator pos, const T& value) {
        if (empty() || !pos.getCursor()) {
            return push_back(value);
        }

        Node* before = pos.getCursor();
        Node* after = before->getNext();

        Node* node = new Node(value, before, after);

        before->setNext(node);

        if (after) {
            after->setPrev(node);
        } else {
            tail_ = node;
        }

        ++size_;

        return Iterator(head_, tail_, node);
    }

    Iterator insert_after(size_t pos, const T& value) {
        if (pos >= size_) {
            return push_back(value);
        }

        Iterator it = begin();

        for (size_t i = 0; i < pos; ++i) {
            ++it;
        }

        return insert_after(it, value);
    }

    Iterator erase(Iterator pos) {
        Node* node = pos.getCursor();

        if (!node) {
            throw std::range_error(
                "Invalid erase iterator"
            );
        }

        Node* before = node->getPrev();
        Node* after = node->getNext();

        if (before) {
            before->setNext(after);
        } else {
            head_ = after;
        }

        if (after) {
            after->setPrev(before);
        } else {
            tail_ = before;
        }

        delete node;
        --size_;

        return Iterator(head_, tail_, after);
    }

    Iterator push_after(Iterator pos, const T& value) {
        return insert_after(pos, value);
    }

    void push_front(const T& value) {
        Node* node = new Node(value, nullptr, head_);

        if (head_) {
            head_->setPrev(node);
        } else {
            tail_ = node;
        }

        head_ = node;
        ++size_;
    }

    Iterator push_back(const T& value) {
        Node* node = new Node(value, tail_, nullptr);

        if (tail_) {
            tail_->setNext(node);
        } else {
            head_ = node;
        }

        tail_ = node;
        ++size_;

        return Iterator(head_, tail_, node);
    }

    void pop_front() {
        if (empty()) {
            throw std::range_error(
                "Cannot pop empty list"
            );
        }

        erase(begin());
    }

    void pop_back() {
        if (empty()) {
            throw std::range_error(
                "Cannot pop empty list"
            );
        }

        erase(last());
    }

    T& front() {
        if (empty()) {
            throw std::range_error("Empty list");
        }

        return head_->getElement();
    }

    T& back() {
        if (empty()) {
            throw std::range_error("Empty list");
        }

        return tail_->getElement();
    }

    T& at(size_t index) {
        if (index >= size_) {
            throw std::range_error(
                "Index out of bounds"
            );
        }

        Node* node;

        if (index < size_ / 2) {
            node = head_;

            for (size_t i = 0; i < index; ++i) {
                node = node->getNext();
            }
        } else {
            node = tail_;

            for (size_t i = size_ - 1; i > index; --i) {
                node = node->getPrev();
            }
        }

        return node->getElement();
    }

    void reverse() {
        Node* node = head_;

        while (node) {
            Node* next = node->getNext();

            node->setNext(node->getPrev());
            node->setPrev(next);

            node = next;
        }

        std::swap(head_, tail_);
    }

    void sort() {
        // Optional: not tested in this assignment.
    }

    DoublyLinkedList<T>& operator=(
        const DoublyLinkedList<T>& other) {

        if (this == &other) {
            return *this;
        }

        DoublyLinkedList<T> copy(other);

        std::swap(head_, copy.head_);
        std::swap(tail_, copy.tail_);
        std::swap(size_, copy.size_);

        return *this;
    }

    bool operator==(DoublyLinkedList<T>& other) {
        if (size_ != other.size_) {
            return false;
        }

        Node* a = head_;
        Node* b = other.head_;

        while (a) {
            if (!(a->getElement() == b->getElement())) {
                return false;
            }

            a = a->getNext();
            b = b->getNext();
        }

        return true;
    }

    bool operator!=(DoublyLinkedList<T>& other) {
        return !(*this == other);
    }

private:
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    size_t size_ = 0;
};

} // namespace CPSC131::DoublyLinkedList

#endif
