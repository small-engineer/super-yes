all: yes

yes: yes.c
	cc yes.c -o yes

clean:
	rm -f yes

.PHONY: all clean