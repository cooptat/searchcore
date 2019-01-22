#ifndef _INDEX_FSYNCER_H_
#define _INDEX_FSYNCER_H_

#include "Index.h"
#include <iostream>
#include <fstream>
#include <array>

namespace searchcell {

template <
	class IndexT,
	template<class T> class Alloc
>
class IndexFSyncer {
public:

	typedef typename IndexT::Ptr IndexPtr;

	typedef typename IndexT::IndexRow IndexRow;
	typedef typename IndexT::IndexRowPtr IndexRowPtr;


	static IndexPtr load(const std::string &_fn) {

		IndexPtr index(  Alloc<IndexT>().allocate( 1 ) );
		Alloc<IndexT>().construct( index.get(), true );

		std::ifstream f( _fn.c_str(), std::ios::out | std::ios::binary );

		auto row_id_size = sizeof(RowId);
		char bf[255];

		while (1) {

			f.read( (char*) &bf, row_id_size);
			RowId row_id( bf );

			if (f.eof())
				break;

			size_t row_dump_size;
			f.read( (char*) &row_dump_size, sizeof(size_t));

			if (f.eof())
				break;

			std::unique_ptr<char[]> dump (new char [ row_dump_size ]);
			f.read( (char*) dump.get(), row_dump_size * sizeof(char));

			IndexRowPtr row ( Alloc<IndexRow>().allocate(1) );
			Alloc<IndexRow>().construct( row.get(), true, dump.get() );

			index->put( std::make_pair( row_id, row ) );
		}

		return index;
	}

	static void flush(const std::string &_fn, IndexPtr _index) {

		if (!_index->isSorted())
			throw std::invalid_argument("IndexFSyncer::flush cannot flush unsorted index");

		std::ofstream o( _fn.c_str(), std::ios::out | std::ios::binary );

		auto row_it = _index->begin();
		auto end = _index->end();

		auto row_id_size = sizeof(RowId);

		while (row_it != end) {

			const auto& row_id = (*row_it).first;
			std::string row_dump = (*row_it).second->dump();
			size_t row_dump_size = row_dump.size();

			o.write( (char*) &row_id, row_id_size );
			o.write( (char*) &row_dump_size, sizeof( row_dump_size ) );
			o.write( (char*) row_dump.data(), row_dump_size * sizeof(char) );

			++row_it;
		}
	}
	
	
};

}

#endif
