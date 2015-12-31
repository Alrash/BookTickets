objects = main.o MyDB.o DealRecv.o

server : $(objects)
	g++ -L/usr/lib/mysql -lmysqlclient -o server $(objects)

main.o : main.cpp
MyDB.o : MyDB.cpp
DealRecv.o : DealRecv.cpp
