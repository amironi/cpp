#include <assert.h>
#include <iostream>
#include <map>

template <typename K, typename V> class interval_map {
  friend void IntervalMapTest();
  V m_valBegin;
  std::map<K, V> m_map;

public:
  // constructor associates whole range of K with val
  interval_map(V const &val) : m_valBegin(val) {}

  // Assign value val to interval [keyBegin, keyEnd).
  // Overwrite previous values in this interval.
  // Conforming to the C++ Standard Library conventions, the interval
  // includes keyBegin, but excludes keyEnd.
  // If !( keyBegin < keyEnd ), this designates an empty interval,
  // and assign must do nothing.
  void assign(K const &keyBegin, K const &keyEnd, V const &val) {

    if (!(keyBegin < keyEnd)) {
      return;
    }

    auto itBegin = m_map.lower_bound(keyBegin);
    auto itEnd = m_map.lower_bound(keyEnd);

    if (itBegin == m_map.end()) {
      m_map.insert(itBegin, std::make_pair(keyBegin, val));
      m_map.insert(itEnd, std::make_pair(keyEnd, m_valBegin));
      return;
    }

    if (itBegin->first == keyBegin) {
      itBegin->second = val;
    } else {
      m_map.insert(itBegin, std::make_pair(keyBegin, val));
    }

    if (itEnd == m_map.end()) {
      m_map.insert(itEnd, std::make_pair(keyEnd, m_valBegin));
      return;
    }

    if (itEnd->first == keyEnd) {
      itEnd->second = m_valBegin;
    } else {
      m_map.insert(itEnd, std::make_pair(keyEnd, m_valBegin));
    }
  }

  // look-up of the value associated with key
  V const &operator[](K const &key) const {
    auto it = m_map.upper_bound(key);
    if (it == m_map.begin()) {
      return m_valBegin;
    } else {
      return (--it)->second;
    }
  }
};

void IntervalMapTest() {
  interval_map<int, char> mMap('A');
  mMap.assign(1, 3, 'B');

  assert(mMap[-1] == 'A');
  assert(mMap[0] == 'A');
  assert(mMap[1] == 'B');
  assert(mMap[2] == 'B');
  assert(mMap[3] == 'A');
  assert(mMap[4] == 'A');
}