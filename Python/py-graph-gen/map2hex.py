with open('map.txt') as p:
    dump = p.read().split('\n')
    height = len(dump)
    width = len(dump[0])
    bindump = ''.join(dump).strip()

hexdump = []
for i in range(0, len(bindump), 4):
    hexdump.append('%X' % int(bindump[i:i+4], 2))

with open('hex.txt', 'w') as h:
    h.write('#define MAP 0x' + ''.join(hexdump) + '\n')
    h.write('#define MAP_W {}\n#define MAP_H {}'.format(width, height))
