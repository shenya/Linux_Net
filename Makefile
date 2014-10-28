all:
	@cd Lib && make
	@cd Client && make
	cd Server && make
install:
	@cd Client && make install
	@cd Server && make install
	@cd Lib && make install

.PHONY: clean
clean:
	@cd Lib && make clean
	@cd Client && make clean
	@cd Server && make clean
	-rm ./Obj/serv ./Obj/cli
