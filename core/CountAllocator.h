#include "misc/Singleton.h"
#include <memory>

typedef Singleton<int> CountAllocatorAllocated;

template<class T, class Counter>
class CountAllocator : public std::allocator<T> {
public:


	typename std::allocator<T>::pointer allocate( typename std::allocator<T>::size_type n, std::allocator<void>::const_pointer hint = 0 ) {

		CountAllocatorAllocated::instance() += n * sizeof(T);
		return std::allocator<T>::allocate( n, hint );
	}

	void deallocate(typename std::allocator<T>::pointer p, typename std::allocator<T>::size_type n) {

		CountAllocatorAllocated::instance() -= n * sizeof(T);
		std::allocator<T>::deallocate( p, n );
	}
};

/*

typedef Singleton<int> IndexAllocatorAllocated;

template<class T>
class IndexAllocator : public CountAllocator<T, IndexAllocatorAllocated> { };

IndexAllocator<int>().allocate();

*/
