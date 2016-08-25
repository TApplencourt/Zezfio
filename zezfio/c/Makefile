all: gz2ar.so

#http://stackoverflow.com/questions/5088460/flags-to-enable-thorough-and-verbose-g-warnings
var=-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wsign-conversion  -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused -Wfatal-errors
gz2ar.so: gz2ar.c
	gcc $(var) -fPIC -shared -o gz2ar.so gz2ar.c -lz

clean:
	rm gz2ar.so