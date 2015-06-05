all: compile
	
compile:
	cd lib && $(MAKE)
	cd cat && $(MAKE)
	cd revwords && $(MAKE)
	cd filter && $(MAKE)
	cd bufcat && $(MAKE)
	cd buffilter && $(MAKE)
	cd simplesh && $(MAKE)
	cd filesender && $(MAKE)
	cd bipiper && $(MAKE)

clean:
	cd lib && $(MAKE) clean
	cd cat && $(MAKE) clean
	cd revwords && $(MAKE) clean
	cd filter && $(MAKE) clean
	cd bufcat && $(MAKE) clean
	cd buffilter && $(MAKE) clean
	cd simplesh && $(MAKE) clean
	cd filesender && $(MAKE) clean
	cd bipiper && $(MAKE) clean
