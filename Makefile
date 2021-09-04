all:
	g++ -lm -o example -Werror -Wall -std=c++20 \
    -Isrc/ \
    -Ithird_party/gcem/include/ \
	-Ithird_party/itsy_bitsy/include/\
    examples/example.cpp third_party/SoftPosit/build/Linux-x86_64-GCC/softposit.a -Ithird_party/SoftPosit/source/include
scratch:
	g++ -lm -o scratch -Werror -Wall -std=c++20 \
    -Isrc/ \
    -Ithird_party/gcem/include/ \
	-Ithird_party/itsy_bitsy/include/\
    scratch.cpp third_party/SoftPosit/build/Linux-x86_64-GCC/softposit.a -Ithird_party/SoftPosit/source/include
build_softposit:
	$(MAKE) -C third_party/SoftPosit/build/Linux-x86_64-GCC/
clean_softposit:
	$(MAKE) clean -C third_party/SoftPosit/build/Linux-x86_64-GCC/
debug:
	g++ -g -lm -o example -Werror -Wall -std=c++20 \
    -Isrc/ \
    -Ithird_party/gcem/include/ \
	-Ithird_party/itsy_bitsy/include/\
    example.cpp third_party/SoftPosit/build/Linux-x86_64-GCC/softposit.a  -Ithird_party/SoftPosit/source/include -O0
build_tests:
	g++ -lm -o test -fpermissive -std=c++20 \
    -Isrc/ \
    -Ithird_party/gcem/include/ \
	-Ithird_party/itsy_bitsy/include/\
    -Ithird_party/Catch2/\
    third_party/Catch2/catch_amalgamated.cpp tests/tests.cpp third_party/SoftPosit/build/Linux-x86_64-GCC/softposit.a  -Ithird_party/SoftPosit/source/include -O3
build_profile_tests:
	g++ -lm -g -pg -o test -fpermissive -std=c++20 \
    -Isrc/ \
    -Ithird_party/gcem/include/ \
	-Ithird_party/itsy_bitsy/include/\
    -Ithird_party/Catch2/\
    third_party/Catch2/catch_amalgamated.cpp tests/tests.cpp third_party/SoftPosit/build/Linux-x86_64-GCC/softposit.a  -Ithird_party/SoftPosit/source/include -O3
valgrind_test:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./test