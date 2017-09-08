
all:
	g++ -c ./jsoncpp-master/src/lib_json/*.cpp -I./jsoncpp-master/include
	g++ -c zjson.cpp -I./ -I./jsoncpp-master/include
	ar rcs libzjson.a *.o
	cp zjson.h ../../inc
	cp -rf ./jsoncpp-master/include/json ../../inc
	mv libzjson.a ../../lib
clean:
	rm *.o
