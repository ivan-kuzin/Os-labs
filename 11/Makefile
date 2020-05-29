all:
	gcc programm_server.c -o programm_server
	gcc programm_cilent.c -o programm_cilent
	./programm_server 3005 &
	echo " 100 \n -1000 \n 1000 \n" > input.txt
	sleep 1
	./programm_cilent 127.0.0.1:3005 < input.txt
	rm input.txt