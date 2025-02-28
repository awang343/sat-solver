all:
	$(MAKE) -C solution-a all
	$(MAKE) -C solution-b all
	mkdir -p bin
	cp solution-a/bin/main bin/solution-a
	cp solution-b/bin/main bin/solution-b

clean:
	$(MAKE) -C solution-a clean
	$(MAKE) -C solution-b clean
	[ -d bin ] && rm -rf bin
