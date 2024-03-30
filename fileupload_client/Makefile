# 源文件目录
SRC_DIR := socket utility

# 源文件后缀
SRC_EXT := cpp

# 生成的可执行文件名
TARGET := client

# 编译器
CXX := g++
# 编译选项
CXXFLAGS := -std=c++11 -Wall -Wextra -O2 -I. -Wno-missing-field-initializers

# 生成所有源文件的对象文件列表
SRCS := $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.$(SRC_EXT)))
OBJS := $(patsubst %.$(SRC_EXT), %.o, $(SRCS)) $(TARGET).o

.PHONY: clean rmlog

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean: rmlog
	rm -f $(OBJS) $(TARGET)

rmlog:
	rm -rf ./log