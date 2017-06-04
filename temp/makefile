Auther = dede
ifndef MyCC
	MyCC = g++
endif
ifndef Filename
	Filename=test
endif

ifndef ExtendName
	ExtendName=.cpp
endif

.PHONY: clean

all: exe
	echo success
	
    
clean:
	rm -f $(Filename).i $(Filename).o $(Filename).exe

preCompile: $(Filename)$(ExtendName)
	echo preCompile excute
	$(MyCC) -E $(Filename)$(ExtendName) -o $(Filename).i

compile: preCompile
	echo compile excute
	$(MyCC) -c $(Filename).i -o $(Filename).o

exe: compile
	echo 'exe excute'
	$(MyCC) -o $(Filename).exe $(Filename).o
 




