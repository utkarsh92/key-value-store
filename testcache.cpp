#include <iostream>
#include "LRUCache2.cpp"
using namespace std;

int main() {
  LRUCache cache(4); 
  cout<<cache.put("k0","value0")<< endl;
  cout<<cache.put("k0","value0")<< endl;
  
  cout<<cache.put("k1","value1")<< endl;
  cout<<cache.put("k2","value2")<< endl;
  cout<<cache.deleteKey("k2")<< endl;
  cout<<cache.put("k6","value6")<< endl;
  //cout << cache.get("k0") << endl; 
 cout<<cache.put("k5","value21")<<endl;
 cout<<cache.put("k6","value22")<< endl;
 cout<<cache.put("k7","value23")<< endl;
 cout<<cache.put("k8","value24")<< endl;
 cout<<cache.put("k9","value25")<< endl;

  
  cout << cache.get("k6") << endl;
  //cout << cache.get("he") << endl;
  cout << cache.get("k0") << endl;
  cout << cache.get("k1") << endl;

}
