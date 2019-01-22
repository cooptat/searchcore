#ifndef _SEARCH_MEM_INDEX_H_
#define _SEARCH_MEM_INDEX_H_

#include "search_defs.h"
#include "Index.h"
#include <typeinfo>
#include <iostream>
#include <fstream>

namespace searchcell {

template<
	template<class T> class Alloc
>
class MemIndexT : public IndexT<Alloc> {
	
public:
	typedef IndexT<Alloc> Index;
	typedef typename Index::Ptr IndexPtr;
	
	typedef MemIndexT<Alloc> MemIndex;
	
	typedef IndexRowT<Alloc> IndexRow;
	typedef typename IndexRow::Ptr IndexRowPtr;
	
	typedef typename std::map< RowId, IndexRowPtr>::iterator map_iterator;
	typedef boost::shared_ptr< MemIndex > MemIndexPtr;
	typedef MemIndexPtr Ptr;

	typedef typename Index::iterator iterator;
	typedef typename Index::iterator_impl iterator_impl;

	std::map<
		RowId,
		IndexRowPtr,
		std::less<RowId>,
		Alloc< std::pair< const RowId, IndexRowPtr > > > m_rows;
	
	class mem_index_iterator_impl : public iterator_impl {
		
		map_iterator m_it;
	public:
		mem_index_iterator_impl(map_iterator _it): m_it(_it) {
		}
		virtual ~mem_index_iterator_impl() { }
		virtual std::pair< RowId, IndexRowPtr > getValue() {
			return *m_it;
		}
		virtual void inc() {
			++m_it;
		}
		virtual bool isEqual(iterator_impl *_it) const {
			
			mem_index_iterator_impl *it = dynamic_cast<mem_index_iterator_impl*> (_it);
			return m_it == it->m_it;
		}

		virtual void print() const {

			std::cout << "mem_index_iterator_impl key:" << m_it->first.dump() << std::endl;
		}
	};
	
	MemIndexT(bool _sorted): Index(_sorted) {	
	}
	
	virtual ~MemIndexT() { }

	virtual void put(std::pair<RowId, IndexRowPtr> _v) {
		
		auto map_it = m_rows.find(_v.first);
		if (map_it != m_rows.end()) {
			map_it->second = _v.second;
			return;
		}
		
		m_rows.insert(_v);
	}
	
	virtual iterator find(const RowId &_t) {
		
		auto map_it = m_rows.find(_t);
		if (map_it == m_rows.end())
			return end();

		return iterator(new mem_index_iterator_impl( map_it ));
	}
	
	virtual iterator begin() {
		return iterator(new mem_index_iterator_impl( m_rows.begin() ));
	}
	
	virtual iterator end() {
		return iterator(new mem_index_iterator_impl( m_rows.end() ));
	}
	
	virtual size_t size() const {
		return m_rows.size();
	}
	
	virtual void sort() {
		
		for (auto map_it : m_rows) {
			map_it.second->sort();
		}
		Index::setSorted();
	}
	
	virtual void merge(IndexPtr _src) {
		
		auto src_it = _src->begin();
		auto src_end = _src->end();		
		
		while (src_it != src_end) {
			
			auto rows_it = m_rows.find( (*src_it).first );
			if (rows_it == m_rows.end()) {
				m_rows.insert( *src_it );
				++src_it;
				continue;
			}
			
			rows_it->second->merge( (*src_it).second.get() );	
			++src_it;
		}
	}
	
	virtual void clear() {
		m_rows.clear();
	}
	
	virtual void dump(const std::string &_fn) {
		
		if (!Index::isSorted())
			throw std::invalid_argument("MemIndex::dump cannot flush unsorted index");
		
		std::ofstream o( _fn.c_str(), std::ios::out | std::ios::binary );
		auto row_it = MemIndex::begin();
		auto end = MemIndex::end();

		auto row_id_size = sizeof(RowId);

		while (row_it != end) {

			const auto& row_id = (*row_it).first;
			auto row_dump = (*row_it).second->dump();
			size_t row_dump_size = row_dump.size();

			o.write( (char*) &row_id, row_id_size );
			o.write( (char*) &row_dump_size, sizeof( row_dump_size ) );
			o.write( (char*) row_dump.data(), row_dump_size * sizeof(char) );

			++row_it;
		}
	}
	
	virtual void load(const std::string &_fn) {
				
		std::ifstream f( _fn.c_str(), std::ios::out | std::ios::binary );

		auto row_id_size = sizeof(RowId);
		//char bf[255];

		while (1) {

			RowId row_id;
			f.read( (char*) &row_id, row_id_size);
			//( bf );

			if (f.eof())
				break;

			size_t row_dump_size;
			f.read( (char*) &row_dump_size, sizeof(size_t));
			
			if (f.eof())
				break;

			std::vector<char> dump;
			dump.assign(row_dump_size, 0);
			
			f.read( (char*) dump.data(), row_dump_size * sizeof(char));

			IndexRowPtr row ( Alloc<IndexRow>().allocate(1) );
			Alloc<IndexRow>().construct( row.get(), true, dump );

			put( std::make_pair( row_id, row ) );
		}
		
		//std::cout << "MemIndex loaded " << size() << std::endl;
	}
};

}

#endif
