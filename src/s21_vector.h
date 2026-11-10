#ifndef S21_VECTOR_H_
#define S21_VECTOR_H_

#include <cstddef>      // std::size_t
#include <initializer_list>
#include <stdexcept>    // std::out_of_range
#include <utility>      // std::move

namespace s21{

template<typename T>
class vector{

public:
using value_type = T;      
using reference = T&;        
using const_reference = const T&;   
using size_type = std::size_t;  

  private:
T* data_;
size_type size_;//
size_type capasity_;
void reallocate(size_type new_capacity);  // выделить новую память и перенести элементы

public:

// конструкторы
vector(): data_(nullptr), size_(0), capasity_(0){}//конструктор по умолчанию
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
  reference front(){return data_[0];};
  const_reference front() const{return data_[0];};
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
      T* operator->()const{return ptr_;}

      iterator& operator++(){++ptr_; return *this;}
      iterator operator++(int){iterator old = *this; ptr_++;return old;}

      iterator& operator--(){--ptr_; return *this;}
      iterator operator--(int){iterator old = *this; ptr_--; return old;}

      iterator operator+(size_type n) const{return iterator(ptr_+n);}
      iterator operator-(size_type n) const{return iterator(ptr_-n);}

      size_type operator-(const iterator& other) const{return ptr_-other.ptr_;} 

      bool operator==(const iterator& other) const {return ptr_== other.ptr_;}
      bool operator!=(const iterator& other) const {return ptr_!= other.ptr_;}      
      T* base() const { return ptr_; }
  };
    class const_iterator{
    private:
      const T* ptr_;
    public:
      const_iterator(){ptr_ = nullptr;}
      const_iterator(const T* ptr){ptr_ = ptr;}
      const_iterator(const iterator& it) {ptr_ = it.base();}

      const_reference operator*()const {return *ptr_;}
      const T* operator->()const{return ptr_;}

      const_iterator& operator++(){++ptr_; return *this;}
      const_iterator operator++(int){const_iterator old = *this; ptr_++;return old;}

      const_iterator& operator--(){--ptr_; return *this;}
      const_iterator operator--(int){const_iterator old = *this; ptr_--; return old;}

      const_iterator operator+(size_type n) const{return const_iterator(ptr_+n);}
      const_iterator operator-(size_type n) const{return const_iterator(ptr_-n);}

      size_type operator-(const_iterator& other) const{return ptr_-other.ptr_;} 

      bool operator==(const const_iterator& other) const {return ptr_== other.ptr_;}
      bool operator!=(const const_iterator& other) const {return ptr_!= other.ptr_;}      
      const T* base() const { return ptr_; }
  };
  iterator begin() noexcept { return iterator(data_); }
  const_iterator begin() const noexcept { return const_iterator(data_); }

  iterator end() noexcept { return iterator(data_ + size_); }
  const_iterator end() const noexcept { return const_iterator(data_ + size_); }

  const_iterator cbegin() const noexcept { return const_iterator(data_); }
  const_iterator cend() const noexcept { return const_iterator(data_ + size_); }

    // Вместимость
  bool empty() const noexcept {return size_ == 0;}
  size_type size() const noexcept{ return size_;}
  size_type max_size() const noexcept;
  void reserve(size_type size);
  size_type capacity() const noexcept{return capasity_;}
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


};
template<typename T>
vector<T>::vector(size_type n): data_(nullptr),capasity_(0),size_(0){
  data_ = static_cast<T*>(::operator new(n*sizeof(T)));
  capasity_ = n;
  size_type i = 0;
  try{
  for(;i<n;i++){
    new(data_+i)T();
  }
}catch(...){
  for(size_type j = 0;j<i;j++){
    data_[j].~T();
  }
  ::operator delete(data_);
  data_ = nullptr;
  capasity_ = 0;
  size_ = 0;
  throw;
}
size_ = n;
}

template<typename T>
vector<T>::~vector(){
  if(data_ != nullptr){
    for(size_type i = 0;i<size_;i++){
      data_[i].~T();
    }
    ::operator delete(data_);
  }
}

template <typename T>
vector<T>::vector(std::initializer_list<value_type> const& items)
    : data_(static_cast<T*>(::operator new(items.size() * sizeof(T)))),
      size_(items.size()),
      capacity_(items.size()) {
    size_t i = 0;
    for (const auto& item : items) {
        new (data_ + i++) T(item); 
    }
}

template<typename T>
vector<T>::vector(const vector& other): data_(nullptr),capasity_(other.capasity_),size_(other.size_){
  data_ = static_cast<T*>(::operator new(capasity_*sizeof(T)));
  for(auto i = 0;i<capasity_;i++){
    new (data_+i) T(other.data_[i]);
  }
}// конструктор копирования

template<typename T>
vector<T>::vector(vector&& other)noexcept:data_(other.data_),capasity_(other.capasity_),size_(other.size_){
other.data_ = nullptr;
  other.capacity_ = 0;
other.size_ = 0;
}
}  // namespace s21

#endif  // S21_VECTOR_H_