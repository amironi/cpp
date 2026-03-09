/* 
 * File:   lrucache.hpp
 * Author: Alexander Ponomarev
 *
 * Created on June 20, 2013, 5:09 PM
 */

#ifndef _LRUCACHE_HPP_INCLUDED_
#define	_LRUCACHE_HPP_INCLUDED_

#include <unordered_map>
#include <list>
#include <cstddef>
#include <stdexcept>

namespace cache {

template<typename K, typename V>
class lru_cache {
  public:
	lru_cache(size_t max_size) :
		_max_size(max_size) {
	}
	
	void max() {
		if (map_.size() > _max_size) {
			map_.erase(list_.back());
			fast_.erase(list_.back());
			list_.pop_back();
		}
	}

	void updateList(map_it& it) {
		list_.remove(fast_[it]);
		list_.push_front( it );
		fast_[it] = list_.front();
	}

	void put(const K& key, const V& value) {
		map_[key] = value;

		auto it = map_.find(key);

		updateList(it)
		max(it);
	}
	
	V& get(const K& key) {
		auto it = map_.find(key);
		if (it == map_.end()) {
			throw range_error("There is no such key in cache");
		} 

		updateList(it);
		return it->second;
	}
	
private:
	list<unordered_map<K, V>::iterator> list_;
	unordered_map<K, V> map_;

	unordered_map<unordered_map<K, V>::iterator , list<unordered_map<K, V>::iterator>::itr > fast_;

	size_t _max_size;
};

} // namespace cache

#endif	/* _LRUCACHE_HPP_INCLUDED_ */