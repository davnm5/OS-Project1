all: shell twc

shell: src/shell.c
	gcc -W src/shell.c -o bin/shell

twc: src/twc.c
	gcc -W src/twc.c -o bin/twc -pthread

clean:
	rm -rf bin/*