
all: clean
	$(MAKE) -C KUBridge
	$(MAKE) -C SystemCtrlForUser
	$(MAKE) -C SystemCtrlForKernel
	$(MAKE) -C VshCtrl
	$(MAKE) -C iplsdk
	$(MAKE) copy_libs

copy_libs:
	@cp KUBridge/*.a .
	@cp SystemCtrlForUser/*.a .
	@cp SystemCtrlForKernel/*.a .
	@cp VshCtrl/*.a .
	@cp iplsdk/*.a .


clean:
	-rm -f *.a
	$(MAKE) -C KUBridge clean
	$(MAKE) -C SystemCtrlForUser clean
	$(MAKE) -C SystemCtrlForKernel clean
	$(MAKE) -C VshCtrl clean
	$(MAKE) -C iplsdk clean
