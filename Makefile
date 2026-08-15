out:
	mkdir out

funcpp: out
	g++ -std=c++2a funcpp-test.cc -I. -o out/funcpp-test

fibonnacci: out
	g++ -std=c++2a fibonnacci.cc -I. -o out/fibonnacci

clean:
	rm -r out
