make:
	cc65 -Osir -Cl -t cx16 -I inc xist_gfx.c    -o lib/xist_gfx.s
	cc65 -Osir -Cl -t cx16 -I inc xist_input.c  -o lib/xist_input.s
	cc65 -Osir -Cl -t cx16 -I inc xist_mem.c    -o lib/xist_mem.s
	cc65 -Osir -Cl -t cx16 -I inc xist_text.c   -o lib/xist_text.s
	cc65 -Osir -Cl -t cx16 -I inc xist_tiles.c  -o lib/xist_tiles.s
	cc65 -Osir -Cl -t cx16 -I inc xist_utils.c  -o lib/xist_utils.s
	ca65 -t cx16 lib/xist_gfx.s    -o lib/xist_gfx.o
	ca65 -t cx16 lib/xist_input.s  -o lib/xist_input.o
	ca65 -t cx16 lib/xist_mem.s    -o lib/xist_mem.o
	ca65 -t cx16 lib/xist_text.s   -o lib/xist_text.o
	ca65 -t cx16 lib/xist_tiles.s  -o lib/xist_tiles.o
	ca65 -t cx16 lib/xist_utils.s  -o lib/xist_utils.o
	ar65 r lib/xist.lib lib/xist_gfx.o lib/xist_input.o lib/xist_mem.o lib/xist_text.o lib/xist_tiles.o lib/xist_utils.o
	rm -f lib/*.o lib/*.s