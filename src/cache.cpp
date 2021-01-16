#include "cache.h"

template <typename KeyType, typename ValueType>

bool LRUCache<KeyType, ValueType>::get(const KeyType &key, ValueType &value) {

	std::lock_guard<std::mutex> lock(m_cache_mutex);

	//key not present in cache map
	if(m_cacheMap.count(key) == 0) {
		return -1;
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
		return value;
	}
}
/* 
	Put item to cache list and update positioning
*/

template <typename KeyType, typename ValueType>
void LRUCache<KeyType, ValueType>::put(const KeyType &key, const ValueType &value) {

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
