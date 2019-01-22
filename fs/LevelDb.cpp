#include "LevelDb.h"
#include <stdexcept>
#include <assert.h>
#include <iostream>

namespace searchcell {

LevelDb::LevelDb(const std::string &_path):
m_path(_path) {
	
	openDb();
}

void LevelDb::openDb() {
	
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, m_path.c_str(), &db);
	if (!status.ok()) {
		
		std::cout << "repairing: " << leveldb::RepairDB(m_path.c_str(), options).ToString() << std::endl;		
	}
	
	//status = leveldb::DB::Open(options, m_path.c_str(), &db);
	if (!status.ok()) {
		
		throw std::invalid_argument("cls::Storage::Storage cannot open database " + m_path+ " ex: " + status.ToString());
	}
	m_lvldb.reset(db);
}

void LevelDb::clear() {
	
	m_lvldb.reset();
	DestroyDB(m_path, leveldb::Options());
	openDb();
}

LevelDbIterator LevelDb::getIterator() {
	return LevelDbIterator(m_lvldb->NewIterator(leveldb::ReadOptions()));
}

leveldb::Status LevelDb::get(const std::string &_key, std::string &_value) {
	return m_lvldb->Get(leveldb::ReadOptions(), _key, &_value);
}

leveldb::Status LevelDb::put(const std::string &_key, const std::string &_value) {

	return m_lvldb->Put(leveldb::WriteOptions(), _key, _value);
}

leveldb::Status LevelDb::remove(const std::string &_key) {

	return m_lvldb->Delete(leveldb::WriteOptions(), _key);
}

leveldb::Status LevelDb::writeBatch(leveldb::WriteBatch &_batch) {
	return m_lvldb->Write(leveldb::WriteOptions(), &_batch);
}

};
