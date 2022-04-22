# ELgamal

## This is the documentation for the ELgamal Project.

To first start out, my project is partially done. My encryption, decryption, and public key making are successful. The only problem is my random prime number generating

because I could not create a random prime generator. I have just used the random prime number that was used in class for the example of ELGamal. that number can simply be changed by find the value q in the main function, then divide that number by half, take the floor and subtract by 1. That is to ensure that the prime numbers generators can be determined.

# DEPENDENCIES

just the Boost++ framework. I used this library for the "big number" capabilities and for the generation of random big numbers.

# CHALLENGES:

There were many challenge. The first was the use of C++. I am aware that most of my classmates used python, or easier programming languages. I although wanted to use C++ because I feel like it would make me a better programmer in the long run.

Determine how to use really big numbers was a big one. C++ highest built in type is a long long that can only calculate about 15 to 16 decimal spaces. This is surely not enough range for encryption. I then was able to use the BOOST++ library that had the multi-precision library. I had tried using the many types that it offered such as cpp_int128_t but none were big enough. Luckily, the cpp_int type was available in the library and dynamically allocates as much space is needed at runtime so I did not have to worry about it.

The next problem was computation time. Even though I could use large values, my computer was slow at computing them. While I did implement square and multiply algorithm, my program was not able to compute the large random values returned so I had to use modular reduction to reduce the number size.

A challenge I solved was using the primarily test. Even-though I had implemented Fermat's primality test. It would take too long to use and require too many iterations for it not even to be determine if the number was prime some of the times. Using Miller-rabins primarily test was a lot better. But still not completely worth-wild. It occasionally does not provide the right answers for values that are 256 bits. However, that because the safety iterations can only go up to about 100000. That takes about 10 seconds too compute. Going to a million times that time by 10 fold and that is not feasible.

# POSSIBLE IMPROVMENTS:

The only improvement I could make would be to use a primality test that is not iterative. Or at least a more advanced primality test that is more efficient.

The second would be to create a working prime number generator. The problem was finding a number generator that could produce up to 256 bit values. The rand() number generator did not produce the numbers that I wanted and the large number generator produced with the Boost library did not have a very good seed value.

all in all the project was very difficult. While my program is a little buggy, It preforms. I will be trying to hammer out any more bugs I find.


# HOW TO BUILD AND RUN:

To build and run all you have to do is run the following command in the directory "Elgamal_finished"

pwd && g++ -c src/*.cpp -I include -std=c++11 -g -Wall -m64 && g++ *.o -o bin/Elgamal -L lib && cd bin && ./Elgamal

The program should then execute.
