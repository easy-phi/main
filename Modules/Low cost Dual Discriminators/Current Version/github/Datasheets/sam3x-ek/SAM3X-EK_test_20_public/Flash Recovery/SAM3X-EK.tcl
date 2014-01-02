#  ----------------------------------------------------------------------------
#          ATMEL Microcontroller Software Support  -  SDC  -
#  ----------------------------------------------------------------------------
#  Copyright (c) 2010, Atmel Corporation
#
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#  - Redistributions of source code must retain the above copyright notice,
#  this list of conditions and the disclaiimer below.
#
#  - Redistributions in binary form must reproduce the above copyright notice,
#  this list of conditions and the disclaimer below in the documentation and/or
#  other materials provided with the distribution. 
#
#  Atmel's name may not be used to endorse or promote products derived from
#  this software without specific prior written permission. 
#
#  DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
#  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
#  DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
#  OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#  ----------------------------------------------------------------------------

puts "-I- === SAM3X-EK product test software Programming ==="

puts "-I- === Init internal Flash ==="
FLASH::Init

puts "-I- === Erase internal Flash ==="
FLASH::EraseAll

puts "-I- === Send executable bin into Flash ==="
send_file {Flash} "./Test-board-project-sam3xek.bin" 0x80000 0

puts "-I- === Chang GPNVM to Boot from Flash ==="
FLASH::ScriptGPNMV 2

puts "-I- === End of Flash programming ==="