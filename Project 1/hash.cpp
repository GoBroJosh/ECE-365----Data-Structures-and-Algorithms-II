/* Joshua Go
DSA II - Prof. Sable
Programming Assignment #1 */
#include "hash.h"

//Array of Primes
int primearray[] = {98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

// Return a prime number at least as large as size.
// Uses a precomputed sequence of selected prime numbers.
unsigned int hashTable::getPrime(int size){
  int elements = sizeof(primearray)/sizeof(int);
  for (int i = 0; i < elements; i++) {
    if (size <= primearray[i]){
      return primearray[i];
    }
  }
}

// The constructor initializes the hash table.
// Uses getPrime to choose a prime number at least as large as
// the specified size for the initial size of the hash table.
hashTable::hashTable(int size){
  int prime = getPrime(size);
  data.resize(prime);
  capacity = prime;
  filled = 0;
  /*Resetting the vector. Done as a redunancy.
  False should be set automatically when hash table is instantiated*/
  for(int i = 0; i < data.size(); i++){
    data[i].isOccupied = false;
    data[i].isDeleted = false;
  }
}

// The hash function.
int hashTable::hash(const std::string &key){
  /*slower hash function, but should separate strings better (ex "ab" from "ba")*/
  int seed = 131;
  unsigned long hash = 0;
  for(int i = 0; i < key.length(); i++)
  {
    hash = (hash * seed) + key[i];
  }
   return hash % capacity;
}

// The rehash function; makes the hash table bigger.
// Returns true on success, false if memory allocation fails.
bool hashTable::rehash(){
  std::vector<hashItem> data2 = data;
  int prime2 = getPrime(2*capacity);
  try {
    data.resize(prime2);
  }
  catch (std::bad_alloc) {
    return false;
  }
  capacity = prime2;
  filled = 0;
  //Resetting the vector
  for (int i = 0; i < data.size(); i++){
    data[i].isOccupied = false;
    data[i].isDeleted = false;
  }

  for (int i = 0; i < data2.size(); i++){
    /* Note: I'm aware I could have done without the "==true"
    and replaced the second term with an exclamation mark at the beginning.
    It was done this way just for ease of reading my code if I had to go back
    to it for debugging, etc.*/
    if (data2[i].isOccupied == true && data2[i].isDeleted == false){
      insert(data2[i].key, data2[i].pv);
    }
  }
  return true;
}

// Insert the specified key into the hash table.
// If an optional pointer is provided,
// associate that pointer with the key.
// Returns 0 on success,
// 1 if key already exists in hash table,
// 2 if rehash fails.
int hashTable::insert(const std::string &key, void *pv){
  int index = hash(key);
  if ((2*filled) >= capacity){
    if(rehash() == false){
      return 2;
    }
  }
  while (data[index].isOccupied == true){
    if (data[index].key == key){
      if (data[index].isDeleted == false){
        return 1;
      }
      if(data[index].isDeleted == true){
        data[index].isDeleted = false;
        return 0;
      }
    }
    if (index != (capacity - 1)){
      index++;
    } else {
      index = 0;
    }
  }
  data[index].key = key;
  data[index].isOccupied = true;
  data[index].isDeleted = false;
  data[index].pv = pv;
  filled++;
  return 0;
}

// Search for an item with the specified key.
// Return the position if found, -1 otherwise.
int hashTable::findPos(const std::string &key){
  int index = hash(key);
  while(data[index].isOccupied){
    if (data[index].key == key){
      if (data[index].isDeleted == false){
        return index;
      }
    }
    if (index != (capacity - 1)) {
      index++;
    } else {
      index = 0;
    }
  }
  return -1;
}

// Check if the specified key is in the hash table.
// If so, return true; otherwise, return false.
bool hashTable::contains(const std::string &key){
  if(findPos(key) == -1){
    return false;
  } else {
    return true;
  }
}
