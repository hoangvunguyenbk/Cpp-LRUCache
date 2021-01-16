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

public:

	/*
		Constuctor
	*/
	LRUCache(int size)
	: m_capacity(size) {}

	/* 
		Print out value of the cache
	*/
	void display() {
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

	/*
		Get value by key
		Return true if key found, otherwise return false
	*/
	bool get(const KeyType &key, ValueType &value) {

		std::lock_guard<std::mutex> lock(m_cache_mutex);
	
		//key not present in cache map
		if(m_cacheMap.count(key) == 0) {
			return false;
		}
		//key found in cache map
		else {
			//Backup and erase current Item
			LRUListPos currentPos = m_cacheMap[key];
			CacheItem currentItem = *currentPos;
			m_cacheList.erase(currentPos);
			//Move Item to the front and update cache map
			m_cacheList.push_front(*currentPos);
			m_cacheMap[key] = m_cacheList.begin();
			value = m_cacheList.front().m_value;
			return true;
		}		
	}

	/* 
		Put item to cache list and update positioning
	*/
	void put(const KeyType &key, const ValueType &value) {
		std::lock_guard<std::mutex> lock(m_cache_mutex);
		
		//key not present in cache map
		if(m_cacheMap.count(key) == 0) {
			//If size of List if exeed, erase last item in List
			if(m_cacheList.size() == m_capacity) {
				KeyType old_key = m_cacheList.back().m_key;
				m_cacheMap.erase(old_key);
				m_cacheList.pop_back();
			}
		}
		//key found in the cache map
		else {
			//Move Item to the front
			LRUListPos currentPos = m_cacheMap[key];
			m_cacheList.erase(currentPos);
		}
		m_cacheList.push_front(CacheItem(key, value));
		m_cacheMap[key] = m_cacheList.begin();		
	}

protected:

	std::mutex m_cache_mutex;
};

#endif //CACHE_H


