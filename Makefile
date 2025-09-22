
all:
	$(MAKE) -C src/KUBridge
	$(MAKE) -C src/SystemCtrlForUser
	$(MAKE) -C src/SystemCtrlForKernel
	$(MAKE) -C src/VshCtrl
	$(MAKE) -C src/SysclibForUser
	$(MAKE) -C src/idsRegeneration
	$(MAKE) -C src/idsRegeneration_driver
	$(MAKE) -C src/libintraFont_stub
	$(MAKE) -C src/libpspav_stub
	$(MAKE) -C src/libpspftp_stub
	$(MAKE) -C src/guglue
	$(MAKE) -C src/ansi-c
	$(MAKE) -C src/colordebugger
	$(MAKE) -C src/screenprinter
	$(MAKE) -C src/mini2d
	$(MAKE) -C src/iplsdk
	$(Q)mkdir -p libs
	$(Q)mkdir -p include/iplsdk
	$(Q)cp src/pre-built/*.a libs/
	$(Q)cp src/KUBridge/*.a libs/
	$(Q)cp src/SystemCtrlForUser/*.a libs
	$(Q)cp src/SystemCtrlForKernel/*.a libs
	$(Q)cp src/VshCtrl/*.a libs
	$(Q)cp src/SysclibForUser/*.a libs
	$(Q)cp src/idsRegeneration/*.a libs
	$(Q)cp src/idsRegeneration_driver/*.a libs
	$(Q)cp src/libintraFont_stub/*.a libs
	$(Q)cp src/libpspav_stub/*.a libs
	$(Q)cp src/libpspftp_stub/*.a libs
	$(Q)cp src/guglue/*.a libs
	$(Q)cp src/ansi-c/*.a libs
	$(Q)cp src/colordebugger/*.a libs
	$(Q)cp src/screenprinter/*.a libs
	$(Q)cp src/mini2d/*.a libs
	$(Q)cp src/iplsdk/*.a libs
	$(Q)cp src/iplsdk/*.h include/iplsdk

clean:
	$(Q)rm -rf libs
	$(Q)rm -rf include/iplsdk
	$(MAKE) -C src/KUBridge clean
	$(MAKE) -C src/SystemCtrlForUser clean
	$(MAKE) -C src/SystemCtrlForKernel clean
	$(MAKE) -C src/VshCtrl clean
	$(MAKE) -C src/SysclibForUser clean
	$(MAKE) -C src/idsRegeneration clean
	$(MAKE) -C src/idsRegeneration_driver clean
	$(MAKE) -C src/libintraFont_stub clean
	$(MAKE) -C src/libpspav_stub clean
	$(MAKE) -C src/libpspftp_stub clean
	$(MAKE) -C src/guglue clean
	$(MAKE) -C src/ansi-c clean
	$(MAKE) -C src/colordebugger clean
	$(MAKE) -C src/screenprinter clean
	$(MAKE) -C src/mini2d clean
	$(MAKE) -C src/iplsdk clean
