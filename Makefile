all: clean
	$(MAKE) -C KUBridge
	$(MAKE) -C SystemCtrlForUser
	$(MAKE) -C SystemCtrlForKernel
	@cp KUBridge/*.a .
	@cp SystemCtrlForUser/*.a .
	@cp SystemCtrlForKernel/*.a .

clean:
	-rm -f *.a
	$(MAKE) -C KUBridge clean
	$(MAKE) -C SystemCtrlForUser clean
	$(MAKE) -C SystemCtrlForKernel clean


