#include <iostream>
#include "LRUCache1.cpp"
using namespace std;

int main() {
  LRUCache cache(4); 
  cache.put("k3","value3");
  //At the time of updating the cache also update the disk
  
  //cout << cache.get(1) << endl;
  cache.put("he","man");
  cache.put("k1","value1");
  cache.put("k2","value2");
  cout << cache.get("k3") << endl; 
 cache.put("k5","value22");

  
  cout << cache.get("k1") << endl;
  cout << cache.get("he") << endl;
 if(cache.get("k4") == "#"){
		cout<<"Not present in the cache get it from the disk and put it the cache also"<<endl;
		//cache.put(1,GET(1));  where GET(1)  will fetch from the disk  
	}
  cout << cache.get("k3") << endl;
  cout << cache.get("re") << endl;

}
