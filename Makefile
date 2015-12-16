
all: tests doxygen-docs
    
tests:
	cd test; make clean; make

doxygen-docs: 
	rm -Rf docs/*; doxygen Doxyfile

clean:
	cd test; make clean
	rm -Rf docs/*

