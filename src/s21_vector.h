#ifndef S21_VECTOR_H_
#define S21_VECTOR_H_

#include <cstddef>      // std::size_t
#include <initializer_list>
#include <stdexcept>    // std::out_of_range
#include <utility>      // std::move

// using namespace s21{

template<typename T>
class vector{

public:
using value_type = T;      
using reference = T&;        
using const_reference = const T&;   
using size_type = std::size_t;  

// конструкторы
vector();//конструктор по умолчанию
vector(size_type n);//конструктор с введенным размером по умолчанию
vector(std::initializer_list<value_type> const& items);//конструктор со списком инициализации
vector(const vector& other);// конструктор копирования
vector(vector&& other)noexcept;//конструктор перемещения
~vector();//деструктор


//Операторы присвоения
vector& operator=(const vector& other);//присвоение копированием
vector& operator=(vector&& other)noexcept;// присвоение перемещением

// ---------- Element access ----------
  reference at(size_type pos);
  const_reference at(size_type pos) const;
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;
  reference front(){return data_;};
  const_reference front() const{return data_;};
  reference back();
  const_reference back() const;
  T* data() noexcept { return data_; }
  const T* data() const noexcept { return data_; }

  // ---------- Итераторы ----------

  class iterator{
    private:
      T* ptr_;
    public:
      iterator(){ptr_ = nullptr;}
      iterator(T* ptr){ptr_ = ptr;}
      reference operator*()const {return *ptr_;}
      T* operator->{return ptr_;}
      operator++()
      operator++(int)
      operator--()
      operator--(int)
      operator+(size_type n) const
      operator-(size_type n) const
      operator-(const iterator& other) const 

      bool operator==(const iterator& other) const {}
      bool operator!=(const iterator& other) const {}      
      T* base() const { return ptr_; }
  };
  iterator begin() noexcept { return iterator(data_); }
  const_iterator begin() const noexcept { return const_iterator(data_); }
  iterator end() noexcept { return iterator(data_ + size_); }
  const_iterator end() const noexcept { return const_iterator(data_ + size_); }
  const_iterator cbegin() const noexcept { return const_iterator(data_); }
  const_iterator cend() const noexcept { return const_iterator(data_ + size_); }

    // Вместимость
  bool empty() const noexcept {return size_ == 0;};
  size_type size() const noexcept{ return size_;};
  size_type max_size() const noexcept;
  void reserve(size_type size);
  size_type capacity() const noexcept{return capasity_};
  void shrink_to_fit();// уменьшает размер до реально заполненной части

   // Модификаторы
   void clear() noexcept;
  iterator insert(const_iterator pos, const_reference value);
  iterator insert(const_iterator pos, value_type&& value);
  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);
  void push_back(const_reference value);
  void push_back(value_type&& value);
  void pop_back();
  void swap(vector& other) noexcept;
  void resize(size_type n);
  void resize(size_type n, const_reference value);


    // Part 3: insert_many
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args);

  template <typename... Args>
  void insert_many_back(Args &&...args);

  private:
T* data_ = nullptr;
size_type size_ = 0;//
size_type capasity_ = 0;
void reallocate(size_type new_capacity);  // выделить новую память и перенести элементы

};
//}  // namespace s21

#endif  // S21_VECTOR_H_