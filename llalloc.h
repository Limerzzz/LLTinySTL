#ifndef __LLALLOC__
#define __LLALLOC__

#include <climits>
#include <cstddef>  // for ptrdiff_t,size_t
#include <cstdlib>
#include <iostream>
#include <new>  // for placement new

namespace LL {

template <class T>
inline T* _allocate(ptrdiff_t size, T*) {
    std::set_new_handler(0);
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (tmp == 0) {
        std::cerr << "out of memory" << std::endl;
        exit(0);
    }
    return tmp;
}

template <class T>
inline void _deallocate(T* buffer) {
    if(buffer == nullptr)   return;
    ::operator delete(buffer);
}
template <class T0, class T2>
inline void _construct(T0* p, const T2& value) {
    new (p) T0(value);
}
template <class T>
inline void _destroy(T* ptr) {
    ptr->~T();
}

template <class T>
class allocator {
   public:
    // the standard interface
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_referrence = const T&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    // rebind allocator of type U
    template <class U>
    struct rebind {
        using other = allocator<U>;
    };
    pointer static  allocate(size_type n, const void* hint = 0) {
        return _allocate((difference_type)n, (pointer)0);
    }
    void static deallocate(pointer p) { _deallocate(p); }

    // the follow functions was remove in c++ 20
    void construct(pointer p, const T& value) { _construct(p, value); }
    void destory(pointer p) { _destroy(p); }

    pointer address(reference x) { return (pointer)&x; }
    const_pointer const_address(const_referrence x) {
        return (const_pointer)&x;
    }
};
template <class T, class Alloc = allocator<T>>
class simple_alloc : public allocator<T> {
   public:
    using typename allocator<T>::pointer;
    static pointer allocate() { return _allocate(1, static_cast<pointer>(0)); }
    static void deallocate(pointer p) { return _deallocate(p); }
};
}  // namespace LL

#endif
