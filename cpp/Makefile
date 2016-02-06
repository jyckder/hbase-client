THRIFT_INCLUDE =  /usr/local/include/thrift
LIB_DIR =  /usr/local/lib


GEN_SRC = ./gen-cpp/Hbase.cpp \
       ./gen-cpp/Hbase_types.cpp \
       ./gen-cpp/Hbase_constants.cpp


default:  test.cpp hbcli.cpp
	g++ -g -o test -I${THRIFT_INCLUDE} -I./gen-cpp -I./ -L${LIB_DIR} -lthrift test.cpp hbcli.cpp  ${GEN_SRC} 
