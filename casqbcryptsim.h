//---------------------------------------------------------------------------
#ifndef CASQBCRYPTSIMH
#define CASQBCRYPTSIMH

#include <string>

//---------------------------------------------------------------------------

namespace casq {
  class bcryptsim {
    public:
      bcryptsim() {};
      bcryptsim(unsigned char cost);
    
      ~bcryptsim();
    
      // generate a salt
      std::string GenerateSalt(const unsigned char rounds = 0);
    
      // generate a hash
      // with plaintext and salt text
      std::string GenerateHash( const char* txt, const char* salt );
      // with plaintext and number of rounds - random salt generated
      std::string GenerateHash( const char* txt, const unsigned char rounds );
      // with plaintext - random salt generated and instatiated rounds
      std::string GenerateHash( const char* txt );
    
      // compare plaintxt with encrypted hash
      bool Compare( const char* plain, const char* hash );
    
      unsigned char GetCost() const { return m_cost };
      const std::string GetLastSalt() const { return m_salt; };
      const std::string GetLastHash() const { return m_hash; };
    
    private:
      
      template<std::size_t N>
      void generateInitVector(uint_t (&IV_buff)[N]);
    
    private:
      unsigned char m_cost{10};     // cost set to a default of 10
      std::string m_salt{""};
      std::string m_hash{""};
    
  };  // end class bcryptsim
};    // end namespace casq
#endif
