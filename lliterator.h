#ifndef __LLITERATOR__
#define __LLITERATOR__

#include <cstddef>

namespace LL {
/**
 * 5 categories of iterator
 * ! Why not a enum,Because themplate arguments need a type, those categories is
 * ! served for template categories as a flag.
 */
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

/**
 *  the global iterator template
 *  ! it serve for contianer's ietrator.
 *  ! A iterator of container should inherit it.
 */
template <class Catrgory, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
    using iterator_category = Catrgory;
    using value_type = T;
    using pointer = Pointer;
    using reference = Reference;
    using difference_type = Distance;
};

/**
 *  iterator traits
 *  ! it server for algorithm, which can use it get the type information.
 *  ! we need to konw xx_traits also are served for algorithm.
 */
template <class I>
struct iterator_traits {
    using iterator_category = typename I::iterator_category ;
    using value_type = typename I::value_type;
    using difference_type = typename I::difference_type;
    using pointer = typename I::pointer;
    using reference = typename I::reference;
};
// specialization of iterator_traits
template <class I>
struct iterator_traits<I*> {
    using iterator_category =
        random_access_iterator_tag;  // naive pointer is random access.
    using value_type = I;  // Although the iterator type is a const type, we
                           // also need normal type to definite variable.
    using difference_type = ptrdiff_t;
    using pointer = I*;
    using reference = I&;
};
// specialization of iterator_traits
template <class I>
struct iterator_traits<const I*> {
    using iterator_category = random_access_iterator_tag;
    using value_type = I;  // Although the iterator type is a const type, we
                           // also need normal type to definite variable.
    using difference_type = ptrdiff_t;
    using pointer = const I*;
    using reference = const I&;
};
// return the category of iterator.
template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(
    const Iterator&) {
    using category = typename iterator_traits<Iterator>::iterator_category;
    return category();
}

/**
 * distance() return the distance of two iterator.
 *  __distance() is  the implement of distance().
 *  because sepicalization template function is not allowed. So, we need to use
 *  a flag arugemnts to distinguish different iterator, thus  speical fucnntion
 *  is served.
 */
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(
    InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}
template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last,
           random_access_iterator_tag) {
    return last - first;
}
template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type distance(
    InputIterator first, InputIterator last) {
    return __distance(first, last, iterator_category(first));
}

// template class: reverse iterator
template <class Iterator>
class reverse_iterator {
   private:
    Iterator current;  // record forward iterator
   public:
    using iterator_category = typename iterator_traits<Iterator>::iterator_category;
    using value_type = typename iterator_traits<Iterator>::value_type;
    using difference_type = typename iterator_traits<Iterator>::difference_type;
    using pointer = typename iterator_traits<Iterator>::pointer;
    using reference = typename iterator_traits<Iterator>::reference;

    using self = reverse_iterator<Iterator>;

   public:
    reverse_iterator() {}
    reverse_iterator(Iterator i) : current(i) {}
    self& operator=(const self& rhs) { current = rhs.current; }
    reference operator*() const {
        auto tmp = current;
        return *(--tmp);
    }
    pointer operator->() const { return &(operator*()); }
    self& operator++() {
        --current;
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        --current;
        return tmp;
    }
    self& operator--() {
        ++current;
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        ++current;
        return tmp;
    }
    self& operator+=(difference_type n) {
        current -= n;
        return *this;
    }
    self operator+(difference_type n) { return static_cast<self>(current - n); }
    self& operator-=(difference_type n) {
        current += n;
        return *this;
    }
    self operator-(difference_type n) { return static_cast<self>(current + n); }
    reference operator[](difference_type n) const { return *(*this + n); }
    // return forward iterator
    Iterator base() const { return current; }
};

template <class Iter>
bool operator==(const reverse_iterator<Iter> lhs, const reverse_iterator<Iter> rhs){
    return lhs.base() == rhs.base();
}
template <class Iter>
bool operator!=(const reverse_iterator<Iter> lhs, const reverse_iterator<Iter> rhs){
    return lhs.base() != rhs.base();
}
template <class Iter>
bool operator<(const reverse_iterator<Iter> lhs, const reverse_iterator<Iter> rhs){
    return lhs.base() < rhs.base();
}
template <class Iter>
bool operator<=(const reverse_iterator<Iter> lhs, const reverse_iterator<Iter> rhs){
    return lhs.base() <= rhs.base();
}
template <class Iter>
bool operator>(const reverse_iterator<Iter> lhs, const reverse_iterator<Iter> rhs){
    return lhs.base() > rhs.base();
}
template <class Iter>
bool operator>=(const reverse_iterator<Iter> lhs, const reverse_iterator<Iter> rhs){
    return lhs.base() >= rhs.base();
}

/**
 * advance() make iterator advance n step.
 */
template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {
    __advance(i, n, iterator_category(i));
}
template <class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
    if (n >= 0)
        while (n--) ++i;
    else
        while (n++) --i;
}
template <class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& i, Distance n,
                      random_access_iterator_tag) {
    i += n;
}
}  // namespace LL
#endif
