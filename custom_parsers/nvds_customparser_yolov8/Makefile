CXX:= g++ -std=c++17

NVCC:=/usr/local/cuda/bin/nvcc

CFLAGS:= -Wall -std=c++11 -shared -fPIC -Wno-error=deprecated-declarations
CFLAGS+= -I/opt/nvidia/deepstream/deepstream/sources/includes -I/usr/local/cuda/include

LIBS+= -lnvinfer_plugin -lnvinfer -lnvparsers -L/usr/local/cuda/lib64 -lcudart -lcublas -lstdc++fs
LFLAGS:= -shared -Wl,--start-group $(LIBS) -Wl,--end-group

INCS:= $(wildcard *.hpp)

SRCFILES+= $(wildcard *.cpp)

TARGET_LIB:= libnvdsinfer_custom_parser_yolov8.so
TARGET_OBJS:= $(SRCFILES:.cpp=.o)

all: $(TARGET_LIB)

%.o: %.cpp $(INCS) Makefile
	$(CXX) -c $(COMMON) -o $@ $(CFLAGS) $<

%.o: %.cu $(INCS) Makefile
	$(NVCC) -c -o $@ --compiler-options '-fPIC' $<

$(TARGET_LIB) : $(TARGET_OBJS)
	$(CXX) -o $@ $(TARGET_OBJS) $(LFLAGS)

clean:
	rm -rf $(TARGET_LIB)
	rm -rf $(TARGET_OBJS)
