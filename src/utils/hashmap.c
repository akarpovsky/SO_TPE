#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>

#define TABLE_STARTSIZE 1023

#define ACTIVE 1

typedef struct {
  void* data;
  int flags;
  long key;
} hEntry;

typedef struct {
  hEntry* table;
  long size, count;
} hashmap;

/* PRIVATE IMPLEMENTATION */

static unsigned long isPrime(unsigned long val)
{
  int i, p, exp, a;
  
  for (i = 9; i--;)
  {
    a = (rand() % (val-4)) + 2;
    p = 1;
    exp = val-1;
    while (exp)
    {
      if (exp & 1)
        p = (p*a)%val;
      
      a = (a*a)%val;
      exp >>= 1;
    }
    
    if (p != 1)
      return 0;
  }
  
  return 1;
}

static int findPrimeGreaterThan(int val)
{
  if (val & 1)
    val+=2;
  else
    val++;
  
  while (!isPrime(val))
    val+=2;
  
  return val;
}

static void rehash(hashmap* hm)
{
  long size = hm->size;
  hEntry* table = hm->table;
  
  hm->size = findPrimeGreaterThan(size<<1);
  hm->table = (hEntry*)calloc(sizeof(hEntry), hm->size);
  hm->count = 0;
  
  while(--size >= 0)
    if (table[size].flags == ACTIVE)
      hashmap_insert(hm, table[size].data, table[size].key);
  
  free(table);
}

/* PUBLIC IMPLEMENTATION */

void* hashmap_new(int startsize)
{
  hashmap* hm = (hashmap*)malloc(sizeof(hashmap));
  
  if (!startsize)
    startsize = TABLE_STARTSIZE;
  else
    startsize = findPrimeGreaterThan(startsize-2);
  
  hm->table = (hEntry*)calloc(sizeof(hEntry), startsize);
  hm->size = startsize;
  hm->count = 0;
  
  return hm;
}

void hashmap_insert(void* hash, void* data, unsigned long key)
{
  hashmap* hm = (hashmap*)hash;
  long index, i, step;
  
  if (hm->size <= hm->count)
    rehash(hm);
  
  do
  {
    index = key % hm->size;
    step = (key % (hm->size-2)) + 1;
    
    for (i = 0; i < hm->size; i++)
    {
      if (!(hm->table[index].flags & ACTIVE))
      {
        hm->table[index].flags |= ACTIVE;
        hm->table[index].data = data;
        hm->table[index].key = key;
        hm->count++;
        return;
      }
      else
      {
        if (hm->table[index].key == key)
        {
          hm->table[index].data = data;
          return;
        }
      }
      
      index = (index + step) % hm->size;
    }
    
    /* it should not be possible that we EVER come this far, but unfortunately not every
       generated prime number is prime (see Carmichael numbers). */
    rehash(hm);
  }
  while (1);
}

void* hashmap_remove(void* hash, unsigned long key)
{
  hashmap* hm = (hashmap*)hash;
  long index, i, step;
  
  index = key % hm->size;
  step = (key % (hm->size-2)) + 1;
  
  for (i = 0; i < hm->size; i++)
  {
    if (hm->table[index].data)
    {
      if (hm->table[index].key == key)
      {
        if (hm->table[index].flags & ACTIVE)
        {
          hm->table[index].flags &= ~ACTIVE;
          hm->count--;
          return hm->table[index].data;
        }
        else /* in, but not active (i.e. deleted) */
          return 0;
      }
    }
    else /* found an empty place (can't be in) */
      return 0;
    
    index = (index + step) % hm->size;
  }
  /* everything searched through, but not in */
  return 0;
}

void* hashmap_get(void* hash, unsigned long key)
{
  hashmap* hm = (hashmap*)hash;
  
  if (hm->count)
  {
    long index, i, step;
    index = key % hm->size;
    step = (key % (hm->size-2)) + 1;
    
    for (i = 0; i < hm->size; i++)
    {
      if (hm->table[index].key == key)
      {
        if (hm->table[index].flags & ACTIVE)
          return hm->table[index].data;
        break;
      }
      else
        if (!hm->table[index].data)
          break;
      
      index = (index + step) % hm->size;
    }
  }
  
  return 0;
}

long hashmap_count(void* hash)
{
  return ((hashmap*)hash)->count;
}

void hashmap_delete(void* hash)
{
  hashmap* hm = (hashmap*)hash;
  
  free(hm->table);
  free(hm);
}

void hashmap_fandel(void* hash, void(*fp)(void*))
{
  hashmap* hm = (hashmap*)hash;
  
  while (--hm->size >= 0)
    if (hm->table[hm->size].flags & ACTIVE)
      fp(hm->table[hm->size].data);
  
  free(hm->table);
  free(hm);
}

/* EOF */

// typedef struct persona{
//     int numero;
//     char letra;
// }persona;

// int main()
// {
//   void* hmap = hashmap_new(1);
  
//   persona p1 = {1,'a'};
//   persona p2 = {2,'b'};
//   persona p3 = {3,'c'};
//   persona p5 = {3,'d'};

//   hashmap_insert(hmap, (void*)&p1, 1);
//   hashmap_insert(hmap, (void*)&p2, 2);
//   hashmap_insert(hmap, (void*)&p3, 3);
//   hashmap_insert(hmap, (void*)&p5, 3);
  
//   persona * p4 = malloc(sizeof(persona));
//   p4 =  hashmap_get(hmap, 3);
//   printf("p4 tiene letra %c\n", p4->letra);
//   hashmap_remove(hmap, 3);
//   p4 =  hashmap_get(hmap, 3);
//   printf("p4 tiene letra %c\n", p4->letra);

//   return 0;
// }
