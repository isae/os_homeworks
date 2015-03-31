all: compile
	
compile:
	cd lib && $(MAKE)
	cd bufcat && $(MAKE)

clean:
	cd lib && $(MAKE) clean
	cd bufcat && $(MAKE) clean
