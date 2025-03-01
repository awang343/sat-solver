all:
	$(MAKE) -C src/solution-a all
	$(MAKE) -C src/solution-b all
	mkdir -p bin
	cp src/solution-a/bin/main bin/solution-a
	cp src/solution-b/bin/main bin/solution-b

clean:
	$(MAKE) -C src/solution-a clean
	$(MAKE) -C src/solution-b clean
	[ -d bin ] && rm -rf bin
