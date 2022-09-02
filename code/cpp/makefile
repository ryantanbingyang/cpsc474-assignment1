CPP=g++
CPPFLAGS=-Wall -g3

TestCribbage: test_cribbage.o rank.o ordered_ranks.o character_suits.o hand.o game.o history.o utility.o policy.o greedy.o c_utility.o my_policy.o
	${CPP} ${CPPFLAGS} -o $@ $^

rank.o: rank.cpp
ordered_ranks.o: ordered_ranks.cpp rank.h
character_suits.o: character_suits.cpp
hand.o: hand.cpp hand.h card.h card.cpp game.h
history.o: history.cpp history.h game.h hand.h
game.o: game.cpp game.h card.h card.cpp hand.h deck.h deck.cpp ordered_ranks.h character_suits.h rank.h history.h combinations.h combinations.cpp
utility.o: utility.cpp utility.h
c_utility.o: c_utility.c c_utility.h
policy.o: policy.cpp policy.h game.h history.h ordered_ranks.h character_suits.h utility.h c_utility.h
greedy.o: greedy.cpp greedy.h policy.h game.h history.h ordered_ranks.h character_suits.h utility.h c_utility.h
c_greedy.o: c_greedy.c c_greedy.h c_cribbage.h
c_cribbage.o: c_cribbage.cpp c_cribbage.h hand.h history.h game.h
my_policy.o: my_policy.cpp my_policy.h policy.h greedy.h
test_cribbage.o: test_cribbage.cpp game.h policy.h greedy.h my_policy.h

clean:
	rm -f *.o TestCribbage
