
testcache: 
	g++ -o testcache testcache.cpp

run: 
	./testcache
clean:
	rm -f ./testcache