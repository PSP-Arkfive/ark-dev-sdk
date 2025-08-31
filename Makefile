
all:
	$(MAKE) -C src/KUBridge
	$(MAKE) -C src/SystemCtrlForUser
	$(MAKE) -C src/SystemCtrlForKernel
	$(MAKE) -C src/VshCtrl
	$(MAKE) -C src/SysclibForUser
	$(MAKE) -C src/libintraFont_stub
	$(MAKE) -C src/libpspav_stub
	$(MAKE) -C src/libpspftp_stub
	$(MAKE) -C src/guglue
	$(MAKE) -C src/ansi-c
	$(MAKE) -C src/colordebugger
	$(MAKE) -C src/screenprinter
	$(MAKE) -C src/mini2d
	$(Q)mkdir -p libs
	$(Q)cp src/pre-built/*.a libs/
	$(Q)cp src/KUBridge/*.a libs/
	$(Q)cp src/SystemCtrlForUser/*.a libs
	$(Q)cp src/SystemCtrlForKernel/*.a libs
	$(Q)cp src/VshCtrl/*.a libs
	$(Q)cp src/SysclibForUser/*.a libs
	$(Q)cp src/libintraFont_stub/*.a libs
	$(Q)cp src/libpspav_stub/*.a libs
	$(Q)cp src/libpspftp_stub/*.a libs
	$(Q)cp src/guglue/*.a libs
	$(Q)cp src/ansi-c/*.a libs
	$(Q)cp src/colordebugger/*.a libs
	$(Q)cp src/screenprinter/*.a libs
	$(Q)cp src/mini2d/*.a libs

clean:
	$(Q)rm -rf libs
	$(MAKE) -C src/KUBridge clean
	$(MAKE) -C src/SystemCtrlForUser clean
	$(MAKE) -C src/SystemCtrlForKernel clean
	$(MAKE) -C src/VshCtrl clean
	$(MAKE) -C src/SysclibForUser clean
	$(MAKE) -C src/libintraFont_stub clean
	$(MAKE) -C src/libpspav_stub clean
	$(MAKE) -C src/libpspftp_stub clean
	$(MAKE) -C src/guglue clean
	$(MAKE) -C src/ansi-c clean
	$(MAKE) -C src/colordebugger clean
	$(MAKE) -C src/screenprinter clean
	$(MAKE) -C src/mini2d clean
