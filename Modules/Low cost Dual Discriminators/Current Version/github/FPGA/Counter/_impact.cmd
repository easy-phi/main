setPreference -pref StartupClock:AUTO_CORRECTION
setPreference -pref AutoSignature:FALSE
setPreference -pref KeepSVF:FALSE
setPreference -pref ConcurrentMode:FALSE
setPreference -pref UseHighz:FALSE
setPreference -pref svfUseTime:FALSE
setPreference -pref SpiByteSwap:AUTO_CORRECTION
setPreference -pref AutoInfer:TRUE
setPreference -pref SvfPlayDisplayComments:FALSE
setMode -bs
setMode -bs
setMode -bs
setMode -bs
setCable -port auto
Identify -inferir 
identifyMPM 
assignFile -p 1 -file "P:/EasyPhi/Modules/Low cost Universal Input/Current Version/github/FPGA/Counter/counter_top.bit"
Program -p 1 
setMode -bs
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
setMode -bs
saveProjectFile -file "P:\EasyPhi\Modules\Low cost Universal Input\Current Version\github\FPGA\spi_master_slave\trunk\syn/\auto_project.ipf"
setMode -bs
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
setMode -bs
saveProjectFile -file "P:\EasyPhi\Modules\Low cost Universal Input\Current Version\github\FPGA\spi_master_slave\trunk\syn/\auto_project.ipf"
setMode -bs
setMode -bs
deleteDevice -position 1
deleteDevice -position 1
deleteDevice -position 1
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
