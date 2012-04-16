
#ifndef _HASHMAP_H
#define _HASHMAP_H

/* erstellt eine neue Hashmap in der angegebenen Größe;
   die Hashmap vergrößert sich selbstständig bei Bedarf,
   die Startgröße sollte trotzdem in der richtigen Dimension liegen */
extern void* hashmap_new(int startsize);

/* fügt ein neues Element in die Hashmap ein */
extern void hashmap_insert(void*, void* data, unsigned long key);

/* löscht das Element zu dem Schlüssel aus der Hashmap und gibt es zurück */
extern void* hashmap_remove(void*, unsigned long key);

/* gibt das Element mit dem gegebenen Schlüssel zurück */
extern void* hashmap_get(void*, unsigned long key);

/* gibt die Anzahl der bereits gespeicherten Elemente zurück */
extern long hashmap_count(void*);

/* löscht die Hashmapstrukturen, allerdings nicht deren Inhalt */
extern void hashmap_delete(void*);

/* löscht die Hashmap fanatisch, inklusive aller Elemente, für die jeweils die gegebene
   Funktion gerufen wird */
extern void hashmap_fandel(void*, void(void*));

#endif