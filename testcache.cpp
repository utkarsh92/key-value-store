#include <iostream>
#include "LRUCache2.cpp"
using namespace std;

int main() {
  LRUCache cache(4); 
  cache.put("k0","value0");
  cache.put("k0","value0");
  //At the time of updating the cache also update the disk
  
  //cout << cache.get(1) << endl;
  cache.put("he","man");
  
  cache.put("k1","value1");
  cache.put("k2","value2");
  cache.deleteKey("k2");
  cache.put("k6","value6");
  cout << cache.get("k0") <<"0 value"<< endl; 
 cache.put("k5","value21");
 cache.put("k6","value22");
 cache.put("k7","value23");
 cache.put("k8","value24");
 cache.put("k9","value25");

  
  cout << cache.get("k6") << endl;
  cout << cache.get("he") << endl;
 if(cache.get("k4") == "#"){
		cout<<"Not present in the cache get it from the disk and put it the cache also"<<endl;
		//cache.put(1,GET(1));  where GET(1)  will fetch from the disk  
	}
  cout << cache.get("k3") << endl;
  cout << cache.get("re") << endl;

}
