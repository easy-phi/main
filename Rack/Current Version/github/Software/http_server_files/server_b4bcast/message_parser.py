#######-------------------------------------------------------------------------------------
#######Easy-phi project: http://easy-phi.ch/
#######Developer : Raphael Houlmann, University of Geneva 2014
#######Licensed under GPL, for details report to: http://easy-phi.ch/index.php/about/licensing
#######Do NOT remove this notice
#######-------------------------------------------------------------------------------------
import re
import logging
logging.basicConfig(filename='/var/log/easy-phi.log',level=logging.DEBUG)
from SCPI_functions import *

class node():
    def __init__(self, command):
        self.command = command
        self.short_command = ""
        self.child = []
        self.procedure = ""
        
        
class parser():
    def __init__(self, threads, error_queue):
        self.threads = threads
        self.error_queue = error_queue
        self.SCPI_Functions = SCPI_Functions(self.threads, self.error_queue)
        
        self.root = node("root")
        for i in self.SCPI_Functions.command_list:
            separated_cmd = i.split(":")
            level = self.root
            for k in range(len(separated_cmd)):
                exist = 0
                for j in range(len(level.child)):
                    if (level.child[j].command == separated_cmd[k].lower()) : 
                        level = level.child[j]
                        exist = 1
                        break
                if (exist == 0): 
                    a = node(separated_cmd[k].lower())
                    level.child.append(a)
                    level = level.child[len(level.child)-1]
                    for c in separated_cmd[k]: 
                        if c.isupper(): level.short_command += c.lower()
                    if re.search('\?', separated_cmd[k]): level.short_command += "?"
    
                    if (k == len(separated_cmd)-1): 
                        level.procedure = self.SCPI_Functions.command_list[i]
                        level = self.root

    def parse_message(self, msg):
        if self.SCPI_Functions.debug_mode: logging.debug(msg)

        msg = msg.split()
        
        ################check right number of arguments
        arg = ""
        if re.search('\?', msg[0]): 
            if len(msg) != 1: 
                self.error_queue.put("Error invalid number of arguments: %s" % msg)
                if self.SCPI_Functions.debug_mode: logging.debug("Error invalid number of arguments: %s" % msg)
                return ("Error invalid number of arguments: %s" % msg)
        else:
            if len(msg) >= 3: 
                self.error_queue.put("Error invalid number of arguments: %s" % msg)
                if self.SCPI_Functions.debug_mode: logging.debug("Error invalid number of arguments: %s" % msg)
                return("Error invalid number of arguments: %s" % msg)    
            if len(msg) == 2: 
                arg = msg[1]                                  
                if re.search('\?', arg): 
                    self.error_queue.put("Error invalid argument: %s" % arg)
                    if self.SCPI_Functions.debug_mode: logging.debug("Error invalid argument: %s" % arg)
                    return ("Error invalid argument: %s" % arg)


        #########################search command in tree    
        separated_cmd = msg[0].split(":")
        level = self.root
        exist = 0
        for k in range(len(separated_cmd)):
            exist = 0
            for j in range(len(level.child)):
                if (level.child[j].command == separated_cmd[k].lower()) or (level.child[j].short_command == separated_cmd[k].lower()) : 
                    level = level.child[j]
                    
                    exist = 1
                    break
        if (exist == 0) or (level.procedure == ""): 
            self.error_queue.put("Error command not found: %s" % msg)
            if self.SCPI_Functions.debug_mode: logging.debug("Error command not found")
            return "Error command not found"    

        if re.search('\?', msg[0]):
            exec("answer = self.SCPI_Functions." + level.procedure)
            if self.SCPI_Functions.debug_mode: logging.debug(answer)
            return answer
        else:
            exec("self.SCPI_Functions." + level.procedure)
            if self.SCPI_Functions.debug_mode: logging.debug("OK")
            return "OK"    

