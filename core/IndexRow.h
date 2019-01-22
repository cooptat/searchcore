#ifndef _SEARCH_INDEX_ROW_H_
#define _SEARCH_INDEX_ROW_H_

#include "search_defs.h"
#include <vector>
#include <set>
#include <json/json.h>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

#include <algorithm>
#include <iostream>

namespace searchcell {

template<
	template<class T> class Alloc
>
class IndexRowT {
public:

  typedef std::shared_ptr< IndexRowT<Alloc> > Ptr;
  typedef std::vector<DocId, Alloc<DocId> > OccursVector;

private:
	bool m_sorted;
	OccursVector m_occurs;
public:
	
	IndexRowT(bool _sorted, const OccursVector &_occurs):
		m_sorted(_sorted) {
		
		m_occurs.assign(_occurs.begin(), _occurs.end());
	}
	
	void intersect(const IndexRowT &_row) {
		
		if (!m_sorted || !_row.m_sorted)
			throw std::invalid_argument("IndexRow::intersect rows must be sorted");
		
		int a_pos = 0;
		int b_pos = 0;
		OccursVector res;

		while (1) {
		
			if (a_pos >= m_occurs.size())
				break;
		
			if (b_pos >= _row.m_occurs.size())
				break;
				
			if ( m_occurs[a_pos] == _row.m_occurs[b_pos] ) {
			
				res.push_back(  m_occurs[a_pos] );
				a_pos++;
				b_pos++;
			}
			else if ( m_occurs[a_pos] < _row.m_occurs[b_pos] )
				a_pos++;
			else
				b_pos++;
		}
		
		m_occurs = res;
	}
	
	void getDocIds(OccursVector &_docs) const {
		
		_docs = m_occurs;
	}

	void getDocIds(std::vector<DocId> &_docs) const {
		
		for (auto o : m_occurs) {
			
			_docs.push_back(o);
		}
	}
	
	void getDocIds(std::set<DocId> &_docs) const {
		for (auto o : m_occurs) {
			
			_docs.insert(o);
		}
	}
	
	void add(DocId _occur) {
		
		m_occurs.push_back(_occur);
		if (m_sorted) {
			std::sort(m_occurs.begin(), m_occurs.end());
		}
	}
	
	void sort() {
		
		std::sort(m_occurs.begin(), m_occurs.end());
		m_sorted = true;
	}
	
	IndexRowT(bool _sorted):
		m_sorted(_sorted) {
	}
	
	IndexRowT(bool _sorted, const std::vector<char> &_dump):
		m_sorted(_sorted) {
		
		const DocId *arr = reinterpret_cast<const DocId*>( _dump.data() );
		
		auto noccurs = _dump.size()/sizeof(DocId);
		m_occurs.reserve(noccurs);

		DocId zero;
		//m_occurs.assign( noccurs, zero );
		for (auto i = 0; i<noccurs; ++i)
		  m_occurs.push_back(zero);
		
		memcpy(m_occurs.data(), arr, _dump.size() );
		
		//std::cout << "Restored occurs " << noccurs << " blob size " << _dump.size() << std::endl;
		/*std::copy(
			&m_occurs[0],
		  &m_occurs[0] + _dump.size() * sizeof(DocId),
			const_cast<char*> ( reinterpret_cast<const char *>( arr ) ) );
		*/
		//m_occurs.assign(arr, arr + _dump.size()/sizeof(DocId) );
	}
	
	std::vector<char> dump() const {
	
		std::vector<char> d;
		
		auto nchars = m_occurs.size() * sizeof(DocId) / sizeof(char);
		d.reserve( nchars );
		d.assign( nchars, 0 );
		
/*		std::copy(
			&d[0],
		  &d[0] + m_occurs.size() * sizeof(DocId),
			const_cast<char*> ( reinterpret_cast<const char *>(m_occurs.data() ) ));
	*/		
		memcpy(d.data(), m_occurs.data(), nchars);
			
			//std::cout << "IndexRow::dump occurs " << m_occurs.size() << " blob size: " << d.size() <<std::endl;
		
		return d;
	}
	
	void print() const {

		for (auto o : m_occurs) {
			std::cout << o << " ";
		}
	}

	void merge(IndexRowT *_row) {
		
		if (!m_sorted) {
			
			for (auto o : _row->m_occurs) {
				m_occurs.push_back(o);
			}
			return;
		}
		
		if (!_row->m_sorted) {
			_row->sort();
		}
		
		OccursVector res_occurs;
		std::merge(m_occurs.begin(), m_occurs.end(), _row->m_occurs.begin(), _row->m_occurs.end(), std::back_inserter(res_occurs));
		m_occurs = res_occurs;
	}
	
	size_t size() const {
		return m_occurs.size();
	}
};

};

#endif

