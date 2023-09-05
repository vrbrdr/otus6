#pragma once

#include <iostream>
#include <math.h>
#include <stdexcept>

namespace containers {
    template <typename T> class list2 {

        typedef list2<T> ltype;

        class _item {
          public:
            template <typename TArg>
            _item(_item* prev, _item* next, TArg& value)
                : m_prev{prev}, m_next{next}, m_value{std::move(value)} {}

          private:
            _item* m_prev;
            _item* m_next;
            T m_value;

            friend ltype;
        };

        class _iterator {
          public:
            _iterator(_item* index, bool end) : m_index{index}, m_end{end} {}

            T& operator*() const {
                return m_index->m_value;
            }

            _iterator& operator++() {
                if (m_index->m_next) {
                    m_index = m_index->m_next;
                } else {
                    m_end = true;
                }

                return *this;
            }

            _iterator& operator--() {
                if (m_end) {
                    m_end = false;
                } else if (m_index->m_prev) {
                    m_index = m_index->m_prev;
                }

                return *this;
            }

            bool operator==(const _iterator& other) const {
                return other.m_index == m_index && other.m_end == m_end;
            }

            bool operator!=(const _iterator& other) const {
                return !(other == *this);
            }

          private:
            _item* m_index;
            bool m_end;
        };

      public:
        list2() = default;

        list2(const ltype& src) {
            copy(src);
        }

        list2(ltype&& src)
            : m_size{src.m_size}, m_head{src.m_head}, m_tail{src.m_tail} {

            src.m_size = 0;
            src.m_head = nullptr;
            src.m_tail = nullptr;
        }

        ~list2() {
            empty();
        }

        ltype& operator=(const ltype& src) noexcept {
            empty();
            copy(src);
            return *this;
        }

        ltype& operator=(ltype&& src) noexcept {
            empty();

            m_size = src.m_size;
            m_head = src.m_head;
            m_tail = src.m_tail;

            src.m_size = 0;
            src.m_head = nullptr;
            src.m_tail = 0;

            return *this;
        }

        void push_back(T&& value) {
            insert_impl(m_size, value);
        }

        void push_back(const T& value) {
            insert_impl(m_size, value);
        }

        void insert(size_t position, T&& value) {
            insert_impl(position, value);
        }

        void insert(size_t position, const T& value) {
            insert_impl(position, value);
        }

        void erase(size_t position) {
            auto item = index_of(position);

            if (item->m_prev) {
                item->m_prev->m_next = item->m_next;
            } else {
                m_head = item->m_next;
            }

            if (item->m_next) {
                item->m_next->m_prev = item->m_prev;
            } else {
                m_tail = item->m_prev;
            }

            delete item;

            --m_size;
        }

        void empty() noexcept {
            while (m_head) {
                auto old_head = m_head;
                m_head = m_head->m_next;
                delete old_head;
            }

            m_head = nullptr;
            m_tail = nullptr;
            m_size = 0;
        }

        T& operator[](const size_t i) const {
            return index_of(i)->m_value;
        }

        size_t size() const noexcept {
            return m_size;
        }

        _iterator begin() const noexcept {
            return _iterator(m_head, false);
        }

        _iterator end() const noexcept {
            return _iterator(m_tail, true);
        }

      private:
        size_t m_size{0};
        _item* m_head{nullptr};
        _item* m_tail{nullptr};

      private:
        _item* index_of(size_t position) {
            if (position > m_size) {
                throw new std::out_of_range("position > m_size");
            }

            auto item = m_head;
            for (size_t i = 0; i < position; ++i, item = item->m_next) {
                ;
            }

            return item;
        }

        template <typename TArg>
        void insert_impl(size_t position, TArg& value) {
            auto pos_item = position == m_size ? nullptr : index_of(position);
            auto prev_item = position == m_size ? m_tail : pos_item->m_prev;
            auto new_item = new _item(prev_item, pos_item, value);

            if (prev_item) {
                prev_item->m_next = new_item;
            } else {
                m_head = new_item;
            }

            if (pos_item) {
                pos_item->m_prev = new_item;
            } else {
                m_tail = new_item;
            }

            ++m_size;
        }

        void copy(const ltype& src) {
            auto src_head = src.m_head;
            while (src_head) {
                push_back(src_head->m_value);
                src_head = src_head->m_next;
            }
        }
    };

} // namespace containers
