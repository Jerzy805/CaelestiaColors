all:
	clear && g++ -std=c++23 -Wall -Wextra -Wpedantic main.cpp color.hpp -o ctheme

clean:
	rm -f *.out *.x *.o ctheme