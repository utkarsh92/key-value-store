#include <iostream>
#include <unordered_map>
#include<string.h>
using namespace std;
class Page {
	public:
	string key, value ;
	char dirty;
	Page *prev, *next;
	Page(string k, string v): key(k), value(v),dirty(2), prev(NULL), next(NULL) {}
};

class Queue {
	Page *front, *rear;
	
	bool isEmpty() {
		return rear == NULL;
	}

	public:
	Queue(): front(NULL), rear(NULL) {}
	
	Page* add_front(string key, string value) {
		Page *page = new Page(key, value);
		if(!front && !rear) {
			front = rear = page;
		}
		else {
			page->next = front;
			front->prev = page;
			front = page;
		}
		return page;
	}

	void push_front(Page *page) {
		if(page==front) {
			return;
		}
		if(page == rear) {
			rear = rear->prev;
			rear->next = NULL;
		}
		else {
			page->prev->next = page->next;
			page->next->prev = page->prev;
		}

		page->next = front;
		page->prev = NULL;
		front->prev = page;
		front = page;
	}

	void pop_back() {
		if(isEmpty()) {
			return;
		}
		if(front == rear) {
			delete rear;
			front = rear = NULL;
		}
		else {
			Page *temp = rear;
			rear = rear->prev;
			rear->next = NULL;
			delete temp;
		}
	}
	Page* back() {
		
		return rear;
	}
	
	void pop(Page* page){
		
		if(page  == rear){
			pop_back();
		}
		else if(page == front){
			front = page->next;
			front->prev = page->prev;
			delete page;
			
		}
		else{
			
			page->next->prev = page->prev;
			page->prev->next = page->next;
			delete page;
		}
		
		
	}
	
};

class LRUCache{
	int capacity, size;
	Queue *pageList;
	unordered_map<string, Page*> cacheMap;

	public:
    LRUCache(int capacity) {
    	this->capacity = capacity;
    	size = 0;
        pageList = new Queue();
        cacheMap = unordered_map<string, Page*>();
    }
    
    string get(string key) {
		
        if(cacheMap.find(key)==cacheMap.end()) {
        	return "#";
        }
        string val = cacheMap[key]->value;
        
        pageList->push_front(cacheMap[key]);
        return val;
    }
    
    void put(string key, string value) {
    	if(cacheMap.find(key)!=cacheMap.end()) {
			cacheMap[key]->dirty = 1;
    		cacheMap[key]->value = value;
    		pageList->push_front(cacheMap[key]);
    		return;
    	}

        if(size == capacity) {
			//cout<<"full"<<endl;
        	string k = pageList->back()->key;
			if(pageList->back()->dirty == 1){
				cout<<"dirty need to write back to disk first"<<endl;
				//writetodisk(key,value);
			}
        	cacheMap.erase(k);
        	pageList->pop_back();
        	size--;
        }

		
        Page *page = pageList->add_front(key, value);
        size++;
        cacheMap[key] = page;
    }
	
	void deleteKey(string key){
		
			if(cacheMap.find(key) != cacheMap.end()){
				//string k = pageList->back()->key;
				pageList->pop(cacheMap[key]);
				cacheMap.erase(key);
				
				size--;
				
			}
	}

    ~LRUCache() {
    	unordered_map<string, Page*>::iterator i1;
    	for(i1=cacheMap.begin();i1!=cacheMap.end();i1++) {
    		delete i1->second;
    	}
    	delete pageList;
    }
};