#ifndef _SEARCH_INDEX_H_
#define _SEARCH_INDEX_H_

#include "search_defs.h"

#include "RowId.h"
#include "IndexRow.h"

#include <memory>
#include <iterator>
#include <functional>

#include <iostream>

namespace searchcell {

class iterator;

template<
	template<class T> class Alloc
>
class IndexT {
	
	bool m_sorted;
	
public:

	template <class T>
	class Allocator : public Alloc<T> { };

    typedef IndexT<Alloc> Index;
	typedef IndexRowT<Alloc> IndexRow;
	typedef typename IndexRow::Ptr IndexRowPtr;
	typedef boost::shared_ptr< Index > IndexPtr;
	typedef IndexPtr Ptr;

	class iterator_impl {
	public:
	
		virtual ~iterator_impl() { }
		virtual std::pair<RowId, IndexRowPtr> getValue() = 0;
		virtual void inc() = 0;
		virtual bool isEqual(iterator_impl *_it) const = 0;
		virtual void print() const = 0;
	};
	
	typedef boost::shared_ptr<iterator_impl> iterator_impl_ptr;
	
	class iterator : public std::iterator<std::input_iterator_tag, int> {
		iterator_impl_ptr m_it;
	public:
		iterator(iterator_impl *_it): m_it(_it) { }
		virtual ~iterator() { }
	
		std::pair<RowId, IndexRowPtr> operator*() const {
			return m_it->getValue();
		}
	
		iterator operator++(int) {
		
			m_it->inc();
			return *this;
		}
		
		iterator operator++() {
			m_it->inc();
			return *this;
		}
	
		bool operator==(const iterator &_it) const {
			return m_it->isEqual( _it.m_it.get() );
		}
	
		bool operator!=(const iterator &_it) const {
			return !m_it->isEqual( _it.m_it.get() );
		}

		void print() const {
			m_it->print();
		}
	};
	
	IndexT(bool _sorted): m_sorted(_sorted) { }
	virtual ~IndexT() { }
	
	bool isSorted() const {
		return m_sorted;
	}
	
	void setSorted() { m_sorted = true; }
	virtual void sort() = 0;
	
	virtual size_t size() const = 0;
	
	virtual void put(std::pair<RowId, IndexRowPtr> _v) = 0;
	virtual iterator find(const RowId &_t) = 0;
	virtual iterator begin() = 0;
	virtual iterator end() = 0;
	
	virtual void merge(IndexPtr _src) = 0;
	virtual void clear() = 0;
	
	virtual void dump(const std::string &_dir) = 0;
	virtual void load(const std::string &_dir) = 0;

	void print() {

	  std::cout << "Index: \n";
	  auto it = begin();
	  auto endit = end();

	  while (it != endit) {
        
        std::cout << (*it).first.dump() << " -> ";
        (*it).second->print();
        std::cout << std::endl;
	  	++it;
	  }
	}
};


}


#endif
