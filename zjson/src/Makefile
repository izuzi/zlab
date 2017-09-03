
all:
	g++ -c ./jsoncpp-master/src/lib_json/*.cpp -I./jsoncpp-master/include
	g++ -c zjson.cpp -I./ -I./jsoncpp-master/include
	ar rcs libzjson.a *.o
	cp zjson.h libzjson.a ../
clean:
	rm *.o
