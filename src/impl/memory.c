#include "../include/memory.h"

void *memset(void *s, int c,  unsigned int len){
    unsigned char* p=s;
    while(len--)
    {
        *p++ = (unsigned char)c;
    }
    return s;
}

void memcpy(void *dest, void *src, size_t n) {
   size_t i;
   char *src_char = (char *)src;
   char *dest_char = (char *)dest;
   for (i=0; i<n; i++)
      dest_char[i] = src_char[i];
}
char heap[heap_size];
memory* head = NULL;

void *malloc(int size)
{
    if (head == NULL)
    {
        head = (memory*) &heap[0];
        head->size = heap_size - sizeof(memory);
        head->isFree = true;
        head->next = NULL;
    }//create first memory block 

    memory* current = head;
    memory* previous = NULL;
    
    while (current != NULL && !(current->isFree && current->size >= size)) 
    {
        previous = current;
        current = current->next;
    } // check, if free memory block available


    if(current !=NULL) // if available
    {
        current->isFree = false; //make current is not free

        if (current->size > size + sizeof(memory))
        {
            memory *new = (memory *) ((char *)current + size + sizeof(memory));
            new->size = current->size - size - sizeof(memory);
            new->isFree = true;//mark new memory block as free.
            current->size = size;
            new->next = current->next;
            current->next = new;
        }
        return (void *)current + sizeof(memory);

    }
    return NULL;
}


void m_free(void *ptr)
{
   if (ptr == NULL)
   {
      return;
   }


   memory * current = head;
   memory * previous = NULL;

   while (current != NULL && (char *)current + sizeof(memory) != ptr)
   {
      previous = current;
      current = current -> next;
   }

   if (current != NULL)
   {
      current->isFree = true;

      if (previous != NULL && previous->isFree)
      {
         previous->size += current->size + sizeof(memory);
         previous->next = current->next;
         current = previous;
      }
        
      if (current->next != NULL && current->next->isFree)
      {
         current->size += current->next->size + sizeof(memory);
         current->next = current->next->next;
      }
   }
}