#ifndef _SRH_LVLDB_STORAGE_H_
#define _SRH_LVLDB_STORAGE_H_

#include "leveldb/db.h"
#include <boost/shared_ptr.hpp>
#include <memory>

namespace searchcell {

typedef boost::shared_ptr<leveldb::Iterator> LevelDbIterator;

class LevelDb {

	typedef std::unique_ptr<leveldb::DB> LevelDBPtr;

	LevelDBPtr m_lvldb;
	std::string m_path;
	
	void openDb();
	
public:
	
	typedef typename boost::shared_ptr<LevelDb> Ptr;
	
	LevelDb(const std::string &_path);
	LevelDbIterator getIterator();
	
	leveldb::Status get(const std::string &_key, std::string &_value);
	leveldb::Status put(const std::string &_key, const std::string &_value);
	leveldb::Status remove(const std::string &_key);
	leveldb::Status writeBatch(leveldb::WriteBatch &_batch);
	
	void clear();	
};

};

#endif
