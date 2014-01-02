rem ###############################################################################
rem  FLASH programming script file exemple
rem ###############################################################################

@echo on

sam-ba.exe COM1 at91sam3x8-ek prog_flash_binaries.tcl > logfile.log 2>&1

notepad logfile.log

