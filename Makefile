out:
	mkdir out

funcpp: out
	g++ -std=c++2a funcpp-test.cc -I. -o out/funcpp-test

fibonacci: out
	g++ -std=c++2a fibonacci.cc -I. -o out/fibonacci

clean:
	rm -r out
