all:
	cd lib && $(MAKE)
	cd cat && $(MAKE)
	cd revwords && $(MAKE)

clean:
	cd lib && $(MAKE) clean
	cd cat && $(MAKE) clean
	cd revwords && $(MAKE) clean
