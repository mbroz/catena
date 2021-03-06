#ifndef _CATENA_H_
#define _CATENA_H_

#include <stdint.h>
#include <string.h>
#include <endian.h>

/* The default unit for all length values are bytes */

/* Recommended default values */
#define H_LEN      64
#define KEY_LEN    16
/* Default values depending on instance*/
extern const uint8_t LAMBDA;
extern const uint8_t GARLIC;
extern const uint8_t MIN_GARLIC;
extern const uint8_t VERSION_ID[];

/* Modes  */
#define PASSWORD_HASHING_MODE 0
#define KEY_DERIVATION_MODE   1
#define REGULAR 0
#define CLIENT 1

/*Flap function of Catena. Possible instantiations:
*	-Catena-BRG using a Bit-Reversal Graph
* 	-Catena-DBG using a Double-Butterfly Graph
*/
void Flap(const uint8_t x[H_LEN], const uint8_t lambda, const uint8_t garlic, 
	const uint8_t *salt, const uint8_t saltlen, uint8_t h[H_LEN]);

/* Returns -1 if an an error occurred, otherwise 0. */
int Catena(uint8_t *pwd,   const uint32_t pwdlen,
	   const uint8_t *salt,  const uint8_t  saltlen,
	   const uint8_t *data,  const uint32_t datalen,
	   const uint8_t lambda, const uint8_t  min_garlic,
	   const uint8_t garlic, const uint8_t hashlen,  uint8_t *hash);


/* API that assumes that the three parameter pwd, salt, and header
 * are all null-terminated string.
 * Returns -1 if an an error occurred, otherwise 0.
 */
int Naive_Catena(char *pwd,  const char *salt, const char *data,
		 uint8_t hash[H_LEN]);


/* Returns -1 if an an error occurred, otherwise 0. */
int Simple_Catena(uint8_t *pwd,   const uint32_t pwdlen,
		   const uint8_t *salt,  const uint8_t  saltlen,
		   const uint8_t *data,  const uint32_t datalen,
		   uint8_t hash[H_LEN]);


/* Expensive and memory consuming  password hashing part.
 * Returns -1 if an an error occurred, otherwise 0.
 */
int Catena_Client(uint8_t *pwd,   const uint32_t pwdlen,
		  const uint8_t *salt,  const uint8_t  saltlen,
		  const uint8_t *data,  const uint32_t datalen,
		  const uint8_t lambda, const uint8_t  min_garlic,
		  const uint8_t garlic, const uint8_t  hashlen,
		  uint8_t x[H_LEN]);

/*  Computes the final step of the password hashing process. Requieres the
 *  output of Catena_Client(...) as input
 *  Returns -1 if an an error occurred, otherwise 0.
 */
int Catena_Server(const uint8_t garlic, const uint8_t x[H_LEN],
		  const uint8_t hashlen, uint8_t *hash);

/* Client independent update form an old hash */
void CI_Update(const uint8_t *old_hash,  const uint8_t lambda,
		   const uint8_t *salt,  const uint8_t saltlen,
	       const uint8_t old_garlic, const uint8_t new_garlic,
	       const uint8_t hashlen, uint8_t *new_hash);

/* Client independent update form an old, keyed hash */
void CI_Keyed_Update(const uint8_t *old_hash,  const uint8_t lambda,
			const uint8_t *salt,		const uint8_t saltlen,
			const uint8_t old_garlic,	const uint8_t new_garlic,
			const uint8_t hashlen,		const uint8_t *key,
			const uint64_t uuid,		uint8_t *new_hash);

/* Mode of operation that derives a key from a password */
void Catena_KG(uint8_t *pwd,   const uint32_t pwdlen,
	       const uint8_t *salt,  const uint8_t saltlen,
	       const uint8_t *data,  const uint32_t datalen,
	       const uint8_t lambda, const uint8_t  min_garlic,
	       const uint8_t garlic, uint32_t keylen,
	       const uint8_t key_id, uint8_t *key);


/* Encrypts the password hash with H(key || uuid || key) where
   key denots a KEY_LEN-byte key and uuid denots a *UNIQUE* user ID */
void Catena_Keyed_Hashing(uint8_t *pwd,   const uint32_t pwdlen,
			  const uint8_t *salt,  const uint8_t  saltlen,
			  const uint8_t *data,  const uint32_t datalen,
			  const uint8_t lambda, const uint8_t  min_garlic,
			  const uint8_t garlic, const uint8_t  hashlen,
			  const uint8_t *key,   const uint64_t uuid,
			  uint8_t *chash);


//PHS requires the password to be const
#ifndef OVERWRITE

/* Returns -1 if an an error occurred, otherwise 0. */
int PHS(void *out, size_t outlen,  const void *in, size_t inlen,
	const void *salt, size_t saltlen, unsigned int t_cost,
	unsigned int m_cost);

#endif //end OVERWRITE

#endif
