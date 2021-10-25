//---------------------------------------------------------------------------

#pragma hdrstop
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <functional>
#include <climits>
#include <stdexcept>
#include "njsbcrypt.h"

#include "casqbcryptsim.h"
//---------------------------------------------------------------------------

using namespace casq;

//---------------------------------------------------------------------------
bcryptsim::bcryptsim(unsigned char cost )
	: m_cost{cost}
{

}
//---------------------------------------------------------------------------
bcryptsim::~bcryptsim() {}

//---------------------------------------------------------------------------
template<std::size_t N>
void bcryptsim::generateInitVector(uint8_t (&IV_buff)[N])
{
	using bytes_randomizer = std::independent_bits_engine<std::default_random_engine, 8, unsigned int>;
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine rd(seed1);
//	rd.seed(seed1);
	bytes_randomizer bytes(rd);

	std::generate(std::begin(IV_buff), std::end(IV_buff), std::ref(bytes));
}
//---------------------------------------------------------------------------
std::string bcryptsim::GenerateSalt(unsigned char rounds)
{
	if (rounds > 0) {
		m_cost = rounds;
	}
	char *_rslt = new char[128];
	u_int8_t data[16];
	generateInitVector(data);
	// generate the salt
	bcrypt_gensalt('b', m_cost, data, _rslt);
	// output the result
	m_salt = _rslt;
	delete[] _rslt;
	return m_salt;
}
//---------------------------------------------------------------------------
std::string bcryptsim::GenerateHash( const char* plaintxt, const char* salt )
{
	if( ! strlen(plaintxt) || ! strlen(salt) )
		throw std::invalid_argument("2 string parameters required");
	try {
		char *_rslt = new char[128];
		bcrypt(plaintxt, strlen(plaintxt), salt, _rslt );
		m_hash = _rslt;
		delete[] _rslt;
		return m_hash;
	} catch (...) {
		throw std::runtime_error("unknown exception caught");
	}

}
//---------------------------------------------------------------------------
std::string bcryptsim::GenerateHash(const char* txt, const unsigned char rounds )
{
	if ( ! strlen( txt ) )
		throw std::invalid_argument("No string provided");
	if ( rounds < 4 || rounds > 32 )
		throw std::invalid_argument( "cost/rounds should be between 4 and 32" );

	try {
		// generate a salt
		std::string salt = GenerateSalt(rounds);
		// generate a hash
		m_hash = GenerateHash( txt, salt.c_str() );
		return m_hash;
	} catch (...) {
		throw std::runtime_error("unknown exception caught");
	}


}
//---------------------------------------------------------------------------
std::string bcryptsim::GenerateHash(const char* txt )
{
	if (!strlen(txt))
		throw std::invalid_argument("1 string parameter required");
	try {
		// generate a salt
		std::string salt = GenerateSalt();
		// generate a hash
		m_hash = GenerateHash( txt, salt.c_str() );
		return m_hash;
	} catch (...) {
		throw std::runtime_error("unknown exception caught");
	}
}
//---------------------------------------------------------------------------
bool bcryptsim::Compare( const char* plain, const char* hash )
{
	if( !strlen(plain) || !strlen(hash) )
		throw std::invalid_argument("2 string parameters required");
	try {
		char salt[30] = "\0";
		strncpy( salt, hash, 29 );
		GenerateHash( plain, salt );
		return strcmp( m_hash.c_str(), hash) == 0;
	} catch (...) {
		throw std::runtime_error("unknown exception caught");
	}
}

//---------------------------------------------------------------------------
