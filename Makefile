#Compile with debug information or optimized.
ifeq ($(DEBUG),1)
BASE_CFLAGS += -c -g -DDEBUG=1 -Wall -std=c++11
else
BASE_CFLAGS += -c -O3 -Wall -std=c++11
endif

total: OaStatWeb3
        
clean: 
	rm -f *.o OaStatWeb3

OASTAT_WEB_OFILES=main.o OaStatWeb3.o OutputterBasic.o OutputterCtemplate.o OpenArenaConverters.o plotgenerator.o
OaStatWeb3: $(OASTAT_WEB_OFILES)
	$(CXX) -O -o OaStatWeb3 $(OASTAT_WEB_OFILES) -lcppdb -lcppcms -lctemplate

%.o : %.cpp
	$(CXX) -MD ${BASE_CFLAGS} -o $@ $<
	@mkdir -p .$(CROSS)deps; cp $*.d .$(CROSS)deps/$*.P; \
             sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
                 -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> .$(CROSS)deps/$*.P; \
             rm -f $*.d

-include .$(CROSS)deps/*.P
