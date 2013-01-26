#Compile with debug information or optimized.
ifeq ($(DEBUG),1)
BASE_CFLAGS += -c -g -DDEBUG=1
else
BASE_CFLAGS += -c -O3
endif

total: OaStatWeb3
        
clean: 
	rm -f *.o OaStatWeb3

OaStatWeb3: main.o OaStatWeb3.o OutputterBasic.o OutputterCtemplate.o OpenArenaConverters.o plotgenerator.o
	c++ -O -o OaStatWeb3 OaStatWeb3.o main.o OutputterBasic.o OutputterCtemplate.o OpenArenaConverters.o plotgenerator.o -lcppdb -lcppcms -lctemplate

OaStatWeb3.o: OaStatWeb3.cpp
	c++ $(BASE_CFLAGS) OaStatWeb3.cpp -o OaStatWeb3.o
        
#plotgenerator.o: plotgenerator.cpp plotgenerator.hpp
#        c++ $(BASE_CFLAGS) plotgenerator.cpp -o plotgenerator.o
        
main.o: main.cpp
	c++ $(BASE_CFLAGS) main.cpp -o main.o

OutputterBasic.o: OutputterBasic.cpp OutputterBasic.hpp
	c++ $(BASE_CFLAGS) OutputterBasic.cpp -o OutputterBasic.o

OutputterCtemplate.o: OutputterCtemplate.cpp OutputterCtemplate.hpp
	c++ $(BASE_CFLAGS) OutputterCtemplate.cpp -o OutputterCtemplate.o

OpenArenaConverters.o: OpenArenaConverters.cpp OpenArenaConverters.hpp
	c++ $(BASE_CFLAGS) OpenArenaConverters.cpp -o OpenArenaConverters.o
	
plotgenerator.o: plotgenerator.cpp plotgenerator.hpp
	c++ $(BASE_CFLAGS) plotgenerator.cpp -o plotgenerator.o