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
	LRUList m_cacheList;
	LRUMap m_cacheMap;
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
	void display();

	/*
		Get value by key
		Return true if key found, otherwise return false
	*/
	bool get(const KeyType &key, ValueType &value);
	
	/* 
		Put item to cache list and update positions
	*/
	void put(const KeyType &key, const ValueType &value);
};

template <typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::display() {

	std::lock_guard<std::mutex> lock(m_cache_mutex);
	std::cout << "Hash map keys table: \n";
	for_each(m_cacheMap.begin(), m_cacheMap.end(), [](std::pair<KeyType, LRUListPos> item) {
		std::cout << item.first << '\t';
	});
	std::cout << std::endl;

	std::cout << "List: \n";
	for_each(m_cacheList.begin(), m_cacheList.end(), [](CacheItem &item){
		std::cout << "[key: " << item.m_key << ",value: " << item.m_value << "]\t";
	});
	std::cout << std::endl;
}


template <typename KeyType, typename ValueType>
bool LRUCache<KeyType, ValueType>::get(const KeyType &key, ValueType &value) {

	std::lock_guard<std::mutex> lock(m_cache_mutex);

	//key not found in cache map
	if(m_cacheMap.count(key) == 0) {
		return false;
	}
	//key found in cache map
	else {
		//backup and erase current Item
		LRUListPos currentPos = m_cacheMap[key];
		CacheItem currentItem = *currentPos;
		m_cacheList.erase(currentPos);
		//move Item to the front and update cache map
		m_cacheList.push_front(*currentPos);
		m_cacheMap[key] = m_cacheList.begin();
		value = m_cacheList.front().m_value;
		return true;
	}		
}


template <typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::put(const KeyType &key, const ValueType &value) {

	std::lock_guard<std::mutex> lock(m_cache_mutex);
	
	//key not found in cache map
	if(m_cacheMap.count(key) == 0) {
		//if list is full, evict the last item in list
		if(m_cacheList.size() == m_capacity) {
			KeyType old_key = m_cacheList.back().m_key;
			m_cacheMap.erase(old_key);
			m_cacheList.pop_back();
		}
	}
	//key found in the cache map
	else {
		//move Item to the front
		LRUListPos currentPos = m_cacheMap[key];
		m_cacheList.erase(currentPos);
	}
	m_cacheList.push_front(CacheItem(key, value));
	m_cacheMap[key] = m_cacheList.begin();		
}

#endif //CACHE_H


