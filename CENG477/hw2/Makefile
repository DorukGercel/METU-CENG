all:
	g++ -O3 *.cpp -c -std=c++11 -march=native -ljpeg -lm -g
	g++ *.o -o raytracer -lpthread -ljpeg -lm -g

clean:
	rm -f *.o
