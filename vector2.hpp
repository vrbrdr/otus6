#pragma once

#include <iostream>
#include <math.h>
#include <stdexcept>

namespace containers {
    template <typename T> class vector2 {

        typedef vector2<T> vtype;

        class _iterator {
          public:
            _iterator(const vtype* parent, size_t index)
                : m_parent{parent}, m_index{index} {}

            T& operator*() const {
                return (*m_parent)[m_index];
            }

            _iterator& operator++() {
                ++m_index;
                return *this;
            }

            _iterator& operator--() {
                --m_index;
                return *this;
            }

            bool operator==(const _iterator& other) const {
                return other.m_index == m_index && other.m_parent == m_parent;
            }

            bool operator!=(const _iterator& other) const {
                return !(other == *this);
            }

          private:
            const vtype* m_parent;
            size_t m_index;
        };

      public:
        vector2() = default;

        vector2(const vtype& src) {
            copy(src);
        }

        vector2(vtype&& src)
            : m_size{src.m_size}, m_storage_size{src.m_storage_size},
              m_storage{src.m_storage} {

            src.m_size = 0;
            src.m_storage_size = 0;
            src.m_storage = nullptr;
        }

        ~vector2() {
            empty();
        }

        vtype& operator=(const vtype& src) noexcept {
            empty();
            copy(src);
            return *this;
        }

        vtype& operator=(vtype&& src) noexcept {
            empty();
            m_size = src.m_size;
            m_storage_size = src.m_storage_size;
            m_storage = src.m_storage;

            src.m_storage = nullptr;
            src.m_size = 0;
            src.m_storage_size = 0;

            return *this;
        }

        void push_back(T&& value) {
            push_back_impl(value);
        }

        void push_back(const T& value) {
            push_back_impl(value);
        }

        void insert(size_t position, T&& value) {
            insert_impl(position, value);
        }

        void insert(size_t position, const T& value) {
            insert_impl(position, value);
        }

        void erase(size_t position) {
            if (position > m_size) {
                throw new std::out_of_range("position > m_size");
            }

            m_storage[position].~T();
            move_tail_left(position + 1, 1);
            --m_size;
        }

        void empty() noexcept {
            resize(0);
        }

        T& operator[](const size_t i) const {
            if (i >= m_size) {
                throw new std::out_of_range("i >= size");
            }

            return m_storage[i];
        }

        size_t size() const noexcept {
            return m_size;
        }

        _iterator begin() const noexcept {
            return _iterator(this, 0);
        }

        _iterator end() const noexcept {
            return _iterator(this, m_size);
        }

      private:
        size_t m_size{0};
        size_t m_storage_size{0};
        T* m_storage{nullptr};

      private:
        void increase_size() {
            resize(m_storage_size ? m_storage_size * 2 : 10);
        }

        void resize(size_t new_storage_size) {
            T* new_storage;

            if (new_storage_size) {
                new_storage = static_cast<T*>(
                    ::operator new(sizeof(T) * new_storage_size));
            } else {
                new_storage = nullptr;
            }

            auto min_size = std::min(new_storage_size, m_size);

            if (m_storage) {
                for (size_t i = 0; i < min_size; ++i) {
                    new_storage[i] = m_storage[i];
                }

                for (size_t i = min_size; i < m_size; ++i) {
                    m_storage[i].~T();
                }

                ::operator delete(m_storage);
            }

            m_storage = new_storage;
            m_storage_size = new_storage_size;
            m_size = min_size;
        }

        void move_tail_left(size_t position, size_t shift_size) {
            for (size_t i = position; i < m_size; ++i) {
                m_storage[i - shift_size] = m_storage[i];
            }
        }

        void move_tail_right(size_t position, size_t shift_size) {
            size_t i = m_size;

            do {
                --i;
                m_storage[i + shift_size] = m_storage[i];
            } while (i > position);
        }

        template <typename TArg> void push_back_impl(TArg& value) {
            if (m_size >= m_storage_size) {
                increase_size();
            }

            new (m_storage + m_size) T(std::move(value));
            m_size++;
        }

        template <typename TArg>
        void insert_impl(size_t position, TArg& value) {
            if (position > m_size) {
                throw new std::out_of_range("position > m_size");
            }

            if (m_size >= m_storage_size) {
                increase_size();
            }

            move_tail_right(position, 1);
            new (m_storage + position) T(std::move(value));
            ++m_size;
        }

        void copy(const vtype& src) {
            resize(src.m_storage_size);

            for (size_t i = 0; i < src.m_size; ++i) {
                new (m_storage + i) T(src.m_storage[i]);
            }

            m_size = src.m_size;
        }
    };

} // namespace containers
