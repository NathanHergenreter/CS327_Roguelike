rlg: main.cpp dungeon_gen.o dungeon_gen.h  heap.o heap.h macros.h path_finder.o path_finder.h colors.h commands.o commands.h character.o character.h dungeon.o dungeon.h monster_parser.o monster_parser.h dice.h object_parser.o object_parser.h
	g++ main.cpp dungeon_gen.o heap.o path_finder.o commands.o character.o dungeon.o monster_parser.o object_parser.o -o rlg -lncurses

dungeon_gen.o: dungeon_gen.cpp
	g++ -c dungeon_gen.cpp -Wall -Werror -g -o dungeon_gen.o

heap.o: heap.cpp
	g++ -c heap.cpp -Wall -Werror -g -o heap.o

path_finder.o: path_finder.cpp
	g++ -c path_finder.cpp -Wall -Werror -g -o path_finder.o

commands.o: commands.cpp
	g++ -c commands.cpp -Wall -Werror -g -o commands.o

character.o: character.cpp
	g++ -c character.cpp -Wall -Werror -g -o character.o

dungeon.o: dungeon.cpp
	g++ -c dungeon.cpp -Wall -Werror -g -o dungeon.o

monster_parser.o: monster_parser.cpp
	g++ -c monster_parser.cpp -Wall -Werror -g -o monster_parser.o

object_parser.o: object_parser.cpp
	g++ -c object_parser.cpp -Wall -Werror -g -o object_parser.o

clean:
	rm -f dungeon_gen.o save_loader.o heap.o path_finder.o commands.o character.o dungeon.o monster_parser.o object_parser.o valgrind.log *~
