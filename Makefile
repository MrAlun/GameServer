SocketTest : main.o sock_wrap.o lightthread.o TimeSpan.o
	g++ -o SocketTest main.o sock_wrap.o lightthread.o TimeSpan.o -pthread
main.o : main.cpp
	g++ -c main.cpp
sock_wrap.o : sock_wrap.cpp sock_wrap.h
	g++ -c sock_wrap.cpp
lightthread.o : lightthread.cpp lightthread.h
	g++ -c lightthread.cpp
TimeSpan.o : TimeSpan.cpp TimeSpan.h
	g++ -c TimeSpan.cpp

clean : 
	rm *.o SocketTest
