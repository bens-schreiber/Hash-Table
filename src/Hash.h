/*
 *  Yet another Hash table interface
 * 
 *  Contributors:
 *      Aaron S. Crandall <acrandal@wsu.edu> - 2019
 *      Sean Kallaher <seanp@kallaher.org> - 2019
 * 
 */

#ifndef __Hash_H
#define __Hash_H

#include <math.h>

// Hash class interface notes
// ******************PUBLIC OPERATIONS*********************
// int size( )                              --> Quantity of (non-deleted) elements in hash
// V& operator[]( const K& k )              --> Returns the value with key k
// bool insert( const pair<K, V>& pair )    --> Adds pair to hash, true if successful
// void erase( const K& k )                 --> Removes all any (if any) entries with key k
// void clear( )                            --> Empties the hash
// int bucket_count()                       --> Returns the number of buckets allocated (size of the hash vector)
// float load_factor( )                     --> Returns the load factor of the hash


// void ~Hash( )       --> Destructor
// Hash( )             --> Basic constructor

//
//  Hash is an abstract base class for other Hash implementations to inherit from
//   Expected subclasses include: ChainingHash - uses a vector of lists
//                                ProbingHash - linear probing on a vector
//  This interface is based upon, and expects similar behavior to the C++11 STL unordered_map
//
template <typename K, typename V>
class Hash
{
private:

public:
    virtual ~Hash() = 0;

    virtual int size() = 0;

    virtual V &operator[](const K& key) = 0;

    virtual bool insert(const std::pair<K, V>& pair) = 0;

    virtual void erase(const K& key) = 0;

    virtual void clear() = 0;

    virtual int bucket_count()= 0;

    virtual float load_factor() = 0;

// *************** Private /internal function implementation ******* //

private:
    virtual int hash(const K& key) = 0;

};

// This is required to make Hash a pure virtual (abstract) class
template <typename K, typename V>
Hash<K, V>::~Hash() {}


#endif
