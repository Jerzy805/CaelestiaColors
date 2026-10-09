all:
	clear && g++ -std=c++23 -Wall -Wextra -Wpedantic main.cpp -o ctheme

clean:
	rm -f *.out *.x *.o ctheme