all: compile
	
compile:
	cd lib && $(MAKE)
	cd cat && $(MAKE)
	cd revwords && $(MAKE)
	cd filter && $(MAKE)
	cd bufcat && $(MAKE)
	cd buffilter && $(MAKE)

clean:
	cd lib && $(MAKE) clean
	cd cat && $(MAKE) clean
	cd revwords && $(MAKE) clean
	cd filter && $(MAKE) clean
	cd bufcat && $(MAKE) clean
	cd buffilter && $(MAKE) clean
