#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <unordered_map>
#include <thread>
#include <list>
#include <mutex>

template <typename KeyType, typename ValueType>
class LRUCache {

private:
	struct CacheItem {
		CacheItem(const KeyType &key, const ValueType &value)
		:m_key(key), m_value(value) {}

		KeyType m_key;
		ValueType m_value;
	};

	typedef std::list<CacheItem> LRUList;
	typedef typename std::list<CacheItem>::iterator LRUListPos;
	typedef std::unordered_map<KeyType, LRUListPos> LRUMap;

	int m_capacity;
	LRUList m_cache_list;
	LRUMap m_cache_map;
	std::mutex m_cache_mutex;

public:
	/*
		Constuctor
	*/
	LRUCache(int size)
	: m_capacity(size) {}

	/*
		Virtual Destructor
	*/
	virtual ~LRUCache() {}

	/* 
		Print out value of the cache
	*/
	void Display();

	/*
		Get value by key
		Return true if key found, otherwise return false
	*/
	bool Get(const KeyType &key, ValueType &value);
	
	/* 
		Put item to cache list and update positions
	*/
	void Put(const KeyType &key, const ValueType &value);
};

template <typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::Display() {

	std::lock_guard<std::mutex> lock(m_cache_mutex);
	std::cout << "Hash map keys table: \n";
	for_each(m_cache_map.begin(), m_cache_map.end(), [](std::pair<KeyType, LRUListPos> item) {
		std::cout << item.first << '\t';
	});
	std::cout << std::endl;

	std::cout << "List: \n";
	for_each(m_cache_list.begin(), m_cache_list.end(), [](CacheItem &item){
		std::cout << "[key: " << item.m_key << ",value: " << item.m_value << "]\t";
	});
	std::cout << std::endl;
}


template <typename KeyType, typename ValueType>
bool LRUCache<KeyType, ValueType>::Get(const KeyType &key, ValueType &value) {

	std::lock_guard<std::mutex> lock(m_cache_mutex);

	//key not found in cache map
	if(m_cache_map.count(key) == 0) {
		return false;
	}
	//key found in cache map
	else {
		//backup and erase current Item
		LRUListPos current_pos = m_cache_map[key];
		CacheItem currentItem = *current_pos;
		m_cache_list.erase(current_pos);
		//move Item to the front and update cache map
		m_cache_list.push_front(*current_pos);
		m_cache_map[key] = m_cache_list.begin();
		value = m_cache_list.front().m_value;
		return true;
	}		
}


template <typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::Put(const KeyType &key, const ValueType &value) {

	std::lock_guard<std::mutex> lock(m_cache_mutex);
	
	//key not found in cache map
	if(m_cache_map.count(key) == 0) {
		//if list is full, evict the last item in list
		if(m_cache_list.size() == m_capacity) {
			KeyType old_key = m_cache_list.back().m_key;
			m_cache_map.erase(old_key);
			m_cache_list.pop_back();
		}
	}
	//key found in the cache map
	else {
		//move Item to the front
		LRUListPos current_pos = m_cache_map[key];
		m_cache_list.erase(current_pos);
	}
	m_cache_list.push_front(CacheItem(key, value));
	m_cache_map[key] = m_cache_list.begin();		
}

#endif //CACHE_H


