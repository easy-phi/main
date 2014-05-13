#######-------------------------------------------------------------------------------------
#######Easy-phi project: http://easy-phi.ch/
#######Developer : Raphael Houlmann, University of Geneva 2014
#######Licensed under GPL, for details report to: http://easy-phi.ch/index.php/about/licensing
#######Do NOT remove this notice
#######-------------------------------------------------------------------------------------
import logging
logging.basicConfig(filename='/var/log/easy-phi.log',level=logging.DEBUG)


class SCPI_Functions():
    def __init__(self, threads, error_queue):
        self.threads = threads
        self.error_queue = error_queue
        self.debug_mode = False
        self.command_list = {
            "*CLS"                       : "SetCore(arg)",
            "*ESE"                       : "SetCore(arg)",
            "*OPC"                       : "SetCore(arg)",
            "*RST"                       : "SetCore(arg)",
            "*SRE"                       : "SetCore(arg)",
            "*WAI"                       : "SetCore(arg)",
            "*ESE?"                      : "GetCore(arg)",
            "*ESR?"                      : "GetCore(arg)",
            "*IDN?"                      : "GetCore(arg)",
            "*OPC?"                      : "GetCore(arg)",
            "*SRE?"                      : "GetCore(arg)",
            "*STB?"                      : "GetCore(arg)",
            "*TST?"                      : "GetCore(arg)",
            
            "RAck:Size? "                : "GetRackSize(arg)",


            "SYSTem:DEBUGmode"           : "SetDebug(arg)",
            "SYSTem:DEBUGmode?"          : "GetDebug(arg)",
            "SYSTem:ERRor?"              : "GetError(arg)",
            "SYSTem:ERRor?"              : "GetError(arg)",
            "SYSTem:ERRor:NEXT?"         : "GetNextError(arg)",
            "SYSTem:ERRor:COUNt?"        : "GetErrorCnt(arg)",
            "SYSTem:VERSion?"            : "GetSystVers(arg)",
            "SYSTem:NUMber:SLots?"       : "GetSystNumberOfSlots(arg)"
        }

        for i in range(len(self.threads)):
            self.command_list["SLot:"+str(i + 1)+":VERSion?"] =      ("Getslotversion(%d, arg)" % i),
            self.command_list["SLot:"+str(i + 1)+":TYpe?"] =         ("Getslottype(%d, arg)" % i)
            self.command_list["SLot:"+str(i + 1)+":NAme?"] =         ("Getslotname(%d, arg)" % i)
            self.command_list["SLot:"+str(i + 1)+":DESCription?"] =  ("Getslotdescription(%d, arg)" % i)
            self.command_list["SLot:"+str(i + 1)+":SERialnumber?"] = ("Getslotserialnumber(%d, arg)" % i)
            self.command_list["SLot:"+str(i + 1)+":STATus?"] =       ("Getslotnstatus(%d, arg)" % i)
            self.command_list["SLot:"+str(i + 1)+":LOcked?"] =        ("Getslotnlocked(%d, arg)" % i)
            self.command_list["SLot:"+str(i + 1)+":LOcked"] =        ("Setslotnlocked(%d, arg)" % i)
            self.command_list["SLot:"+str(i + 1)+":DEBUGmode"] =     ("Setslotdebugmode(%d, arg)" % i)
            self.command_list["SLot:"+str(i + 1)+":DEBUGmode?"] =    ("Getslotdebugmode(%d, arg)" % i)


            
    def SetCore(self, arg):        logging.info("DO SOME STUFF (core)")
    def GetCore(self, arg):        return "core command received"

    def GetRackSize(self, arg):        return str(len(self.threads))
   
    def Getslotversion(self, s, arg):       return self.threads[s].version
    def Getslottype(self, s, arg):          return self.threads[s].type
    def Getslotname(self, s, arg):          return self.threads[s].name      
    def Getslotdescription(self, s, arg):   return self.threads[s].description
    def Getslotserialnumber(self, s, arg):  return self.threads[s].serial_number       
    def Getslotnstatus(self, s, arg):       return self.threads[s].status
    def GetSystVers(self, arg):          return("Easy-phi rack: %d slots" % len(self.threads))
    def GetSystNumberOfSlots(self, arg): return str(len(self.threads))
    def GetError(self, arg):             return str(self.error_queue.qsize())
    def GetErrorCnt(self, arg):          return str(self.error_queue.qsize())
    def Getslotnlocked(self, s, arg):       return str(self.threads[s].lock)

    def Setslotnlocked(self, s, arg):
        if (arg == "0"):
            self.threads[s].lock = "unlocked"
        else:    
            self.threads[s].lock = "locked"

    def SetDebug(self, arg):
        if (arg == "0"):
            self.debug_mode = False
        else:    
            self.debug_mode = True

    def GetDebug(self, arg): 
        if self.debug_mode:
            return "1"
        else:   
            return "0"
            
    def GetNextError(self, arg):
        if self.error_queue.qsize() == 0:
            return("No new error in queue")
        else:
            return("Error number %d: %s" % (self.error_queue.qsize(), self.error_queue.get()))

    def Setslotdebugmode(self, s, arg):
        if (arg == "0"):
            self.threads[s].debug = 0
            logging.info("Debug mode if OFF for slot: %d" % s )
        else:    
            self.threads[s].debug = 1
            logging.info("Debug mode if ON for slot: %d" % s )

    def Getslotdebugmode(self, s, arg): return str(self.threads[s].debug)
            