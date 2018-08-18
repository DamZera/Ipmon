all: client server


## SERVER PART

server: 
	$(MAKE) -C Serv_ipmon


## CLIENT PART

client:
	$(MAKE) -C Clt_ipmon

clean:
	$(MAKE) -C Serv_ipmon clean
	$(MAKE) -C Clt_ipmon clean
