#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include "boost/multiprecision/cpp_int.hpp"
#include "boost/random.hpp"
#include <chrono>
#include <random>
#include <sstream>

using namespace boost::multiprecision;
using namespace std;
using namespace boost::random;

cpp_int big_256 = cpp_int("59803105945714204243013599476134"/*"115792089237316195423570985008687907853269984665640564039457584007913129639935"*/);
cpp_int MOD = big_256;


cpp_int powerStrings(string sa, string sb);
cpp_int powerLL(cpp_int x,cpp_int n);


//key generation functions
cpp_int S_and_M(cpp_int base,cpp_int exp);
string Dec_to_Bin(cpp_int n);
bool miller_rabin(cpp_int n, cpp_int t);
bool Fermat(cpp_int n,cpp_int iterations);
cpp_int modulo(cpp_int base,cpp_int e,cpp_int mod);
cpp_int * Prime_and_gen(cpp_int q);

// encryption function
cpp_int * encryption(cpp_int plain_txt,cpp_int * key);
// decryption function
cpp_int decryption(cpp_int * cipher, cpp_int * key);


int main()
{
	cout << "please enter a message you would like to send:" << endl;
	cin >> s;
	cout << "***********************************************************************\n";

   // the higest prime that works is to the 18th digit

	typedef boost::random::independent_bits_engine<std::mt19937, 256, cpp_int> generator_type;
   //std::default_random_engine generator(rd());
    
   generator_type gen;
   

   //cout << "this prime:" << randpri << endl;

	cpp_int q = static_cast<cpp_int>("315653714003066801");

	cout << "calculating..." << endl;

	auto start = std::chrono::high_resolution_clock::now();

	cpp_int * key = Prime_and_gen(q);	// generates public key (prime, alpha, and alpha^private_key)
	big_256 = key[0];

	auto end = std::chrono::high_resolution_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	cout << "The time it took to make keys is " << elapsed.count() << " milliseconds.\n";

	std::vector<cpp_int> arr;
	cout << "the prime number that was used was: " << key[0] << endl;
	cout << "These are the numbers represnted as numbers in ASCII\n";
	cout << "***********************************************************************\n";
	for(size_t i = 0; i < s.size(); i++)
	{
		cout << (cpp_int)s[i] << endl;
		arr.push_back(((cpp_int)s[i]));
	}
	cout << "***********************************************************************\n";
	cout << "These values are encrypted. Theses are the value and key cipher text pairs\n";
	cout << "***********************************************************************\n";
	vector<cpp_int *> encr;

	start = std::chrono::high_resolution_clock::now();

	for(size_t i = 0; i < arr.size(); i++){

		cpp_int * cipher = encryption(arr[i],key);	// takes a value and encrypts it with public key
		cout << "value:" << cipher[0] << " key:"<<  cipher[1] << endl;
		encr.push_back(cipher);
		
	}
	end = std::chrono::high_resolution_clock::now();

	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	cout << "The time it took to encrypt is " << elapsed.count() << " milliseconds.\n";

	cout << "***********************************************************************\n";

	vector<cpp_int> decmes;

	start = std::chrono::high_resolution_clock::now();

	for(size_t i = 0; i < encr.size();i++ ){
	 	//cout <<  "value:" << *encr[i] << " key:" << encr[i][1] << endl;
	 	decmes.push_back(decryption(encr[i], key));
	 }
	 end = std::chrono::high_resolution_clock::now();

	 elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

	cout << "The time it took to dencrypt is " << elapsed.count() << " milliseconds.\n";

	 string temp = "";
	 cout << "The value/key pairs are turned back into their ASCII values\n";
	 cout << "***********************************************************************\n";
	 for(size_t i = 0; i < decmes.size(); i++){
	 	temp += (char)decmes[i];
	 	cout << decmes[i] << endl;
	}
	cout << "***********************************************************************\n";
	cout << "And the orignal value is returned." << endl;
	cout << "***********************************************************************\n";
	cout << temp << endl;
	//cpp_int dmsg = decryption(cipher, key);		// takes the encrypted message, and decrypts it by using private_key
	return 0;
}

// 256 bits key
// implment mcpp_inter rabins primality test
bool miller_rabin(cpp_int n, cpp_int t){

	std::random_device rd;
    
   std::default_random_engine generator(rd());
    
   std::uniform_int_distribution<cpp_int>distribution(1, big_256);

	// write n - 1 = 2^s * r
	cpp_int even = n - 1;

	cpp_int s = 0;
	while(even % 2 == 0){	// detemrines what s is
		 even /= 2;
		 s++;
	}
	cpp_int r = even;	// when even stops being even, r what the power is multiplied by

	for(cpp_int i = 0; i < t; i++){

		cpp_int a = 2 + distribution(generator) % (n - 2);

		cpp_int y = modulo(a, r, n);

		if((y != 1) && (y != n - 1)){

			cpp_int j = 1;
			while( (j <= s - 1) && (y != n - 1)){

				y = (y * y) % n;

				if( y == 1 )
					return false;
				j++;
			}

			if(y != n - 1)
				return false;
		}

	}

	return true;
}
// does fermats primality test on n
bool Fermat(cpp_int n, cpp_int iterations){

	if(n == 1)	// if n is 1, then return false because 1 is not prime
		return false;
	for(int i = 0; i < iterations; i++){	// takes random value between prime value and takes it to power of n - 1. If not = to 1, then not prime

		cpp_int x = rand() % (n - 2) + 2;

		if( modulo(x, n - 1, n) != 1)
			return false;
		cout << i << " in fermats" << endl;
	}
	

	return true;
}
// turns decimal numbers to binary
string Dec_to_Bin(cpp_int n){
	cpp_int binary{"0"}, remainder, product{"1"};

  	while (n != 0) {
    remainder = n % 2;
    binary = binary + (remainder * product);
    n = n / 2;
    product *= 10;
  	}
  	stringstream tmp;

    tmp << binary;
    
 	return tmp.str();

}
// this algorithm calculates exponential calculations in less time 
cpp_int S_and_M(cpp_int base,cpp_int exp){
	
	string binexp = Dec_to_Bin(exp);
	
	cpp_int org_val = 0;
	string::iterator it = binexp.begin();
	while(it != binexp.end()){			//iterating string from left to right
		
		if((it == binexp.begin()) && (*it == '1'))	// if on first iteration, then just store 
			org_val = base;

		if (*it == '0')
			org_val *= org_val;
		else if(it != binexp.begin())
			org_val	= ((org_val * org_val) * base);	//take current value square it and then multiply by base
			
		it++;
	}

	return org_val;
}
// does modulo operation with given parameters
cpp_int modulo(cpp_int base,cpp_int e,cpp_int mod) {
   cpp_int a = 1;
   cpp_int b = base;
   while (e > 0) {
      if (e % 2 == 1)
         a = (a * b) % mod;
         b = (b * b) % mod;
         e = e / 2;
   }
   return a % mod;
}
// this function returns a q and a p that can be used for calculation
cpp_int * Prime_and_gen(cpp_int q){
	
	//take q which is a random q that is prime, and compute p = 2q + 1. This finds another prime that is possibly 

	// the generators of p are the factors of P-1. Hence, then we can pick a generator for p
	
	std::random_device rd;
    
   std::default_random_engine generator(rd());
    
   std::uniform_int_distribution<cpp_int>distribution(1, big_256);

   cpp_int p = (2 * q) + 1;

   int i = 0;
	while(!miller_rabin(p, 100000)){
		cout << i << endl;
		p = (2 * q) + 1; // this finds a prime number related to q
		q++;
		i++;
	}

	MOD = p;
	//test if g^q mod p is equal to p - 1
	cpp_int g = 2;

	stringstream ss;
	ss << g;
	string temp_g;
	ss >> temp_g;

	while(modulo(g,q,p) != p-1)
		g++;

	// g is the first possible generator. It was wrong for prime number 41

	cpp_int *ptr = new cpp_int[4];	// creates a ptr to size 3 array to store 3 values for public key

	cpp_int a = distribution(generator) % big_256;
	
	ss << a;
	string temp_a;
	ss >> temp_a;


	ptr[0] = p;
	ptr[1] = g;
	ptr[2] = powerStrings(temp_g,temp_a);/*(S_and_M(g,a) % p)*/;
	ptr[3] = a;
	
	
	return ptr;
}
// encrypts the plain_txt decimal number into cipher text
cpp_int * encryption(cpp_int plain_txt,cpp_int * key){

	//srand(time(Ncpp_int));
	//std::random_device rd;
    
	size_t seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
	cout << "seed = " << seed << endl;
    
   boost::random::mt19937 engine(seed);

   typedef boost::random::independent_bits_engine<std::mt19937, 256, cpp_int> generator_type;
   //std::default_random_engine generator(rd());
    
   generator_type gen;
   cpp_int k = gen() % big_256;

	//cpp_int alpha = powerStrings(k_1str,k_str);

	cpp_int alpha = modulo(key[1],k,key[0]);	// calculates part_1 of ciphertext		α^k(modp)

	//cpp_int delta = powerStrings(first,"1");
	cpp_int delta = modulo((plain_txt * S_and_M(key[2], k)), 1, key[0]);	// calculates part_2 of ciphertext		msg * (α^priv_key)^k (mod p)

	cpp_int * p = new cpp_int[2];

	p[0] = alpha;
	p[1] = delta;

	return p;
}
// decrypts the cipher text decimal number back to the original value
cpp_int decryption(cpp_int * cipher,cpp_int * key){

	cpp_int alpha = cipher[0];
	cpp_int p = key[0];
	cpp_int priv_key = key[3];
	cpp_int msg_2 = cipher[1];

	// did not use S_and_M function for encryption
	cpp_int msg_1 = modulo(alpha,((p-1) - priv_key), p)/*S_and_M(alpha,((p - 1)- priv_key)) % p*/;	// calculates part 1 of decryption		alpha^(p - 1 - priv_key) (mod p)

	cpp_int msg = (msg_1 * msg_2) % p;
	return msg;
}


// Returns modulo exponentiation for two numbers
// represented as long long int. It is used by
// powerStrings(). Its complexity is log(n)
cpp_int powerLL(cpp_int x,cpp_int n)
{
   cpp_int result = 1;
    while (n) {
        if (n & 1)
            result = result * x % MOD;
        n = n / 2;
        x = x * x % MOD;
    }
    return result;
}
 
// Returns modulo exponentiation for two numbers
// represented as strings. It is used by
cpp_int powerStrings(string sa, string sb)
{
    // We convert strings to number
 
   cpp_int a = 0, b = 0;
 
    // calculating  a % MOD
    for (int i = 0; i < sa.length(); i++)
        a = (a * 10 + (sa[i] - '0')) % MOD;
 
    // calculating  b % (MOD - 1)
    for (int i = 0; i < sb.length(); i++)
        b = (b * 10 + (sb[i] - '0')) % (MOD - 1);
 
    // Now a and b are long long int. We
    // calculate a^b using modulo exponentiation
    return powerLL(a, b);
}
 

