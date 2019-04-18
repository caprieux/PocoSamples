ALL_OUT = stopwatch logger
ifdef DEBUG
MY_POCO_LIB_PATH = -L$(HOME)/Work/PocoSource/poco-1.9.0/lib/Darwin/x86_64
else
MY_POCO_LIB_PATH = 
endif
LIB_NAME = -lPocoFoundation

.PHONY : all clean

all: $(ALL_OUT)
#static pattern rule
$(ALL_OUT): %: %.cpp
	g++ $(MY_POCO_LIB_PATH) -o $@ $@.cpp $(LIB_NAME)

clean:
	rm -rf $(ALL_OUT)
