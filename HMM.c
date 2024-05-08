/******************************************************************************
 *
 * Module: Heap Memory Manager
 *
 * File Name: HMM.c
 *
 * Description: Source file for the HMM based on Linux
 *
 * Author: Iyad Wael Abou-Elmagd
 *
 *******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "HMM.h"

 metadata* head = NULL_PTR;
 metadata* free_list = NULL_PTR;
 metadata* tail = NULL_PTR;
 char* end = NULL_PTR;


 static int count = 0;

 int old_malloc = FALSE;


/************************************************************************************
* Service Name: HmmAlloc_allocate
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): Nothing
* Parameters (inout): None
* Parameters (out): None
* Return value: boolean 
* Description:Initializes the Heap and Pushes Program break Block Size
************************************************************************************/

void* HmmAlloc(size_t num_bytes)
{
   
    
    
    void* return_ptr = NULL_PTR;

/*******In Case Entered Size of Zero Bytes********/
    if(num_bytes == 0)
    {
      
      return return_ptr;
    }

/*****If Count = 0 (First Time using HmmAlloc) then Initialize the Program*********/
      if(count == 0)
    {
        
         boolean ret = HmmAlloc_allocate();
         if(ret == FALSE)
         {
          
            return return_ptr;
         }
         
    }
     
    num_bytes = ALIGN(num_bytes);

    metadata* current;

/***Choose Start point in Traversing over the Linked-list***/
    if(old_malloc == FALSE)
    {
         current = head;
    }
    else if(old_malloc == TRUE)
    {
        current = free_list;
    }
    
    
/********Traverse Over The list********/
    while(current != NULL_PTR)
    {
       
      /**Get Tail Pointer*/
        if(current -> next == NULL_PTR)
      {
       tail = current;
      }


       /* If Node size equals to the needed Size exactly */
      if(current -> size == (num_bytes + sizeof(metadata)) && (current -> free_status == FREE) )
      {
        
        current -> free_status == RESERVED;
        
        /******Check if it is the last node in order to Push Program break******/
        if(current -> next == NULL_PTR)
        {
          HmmAlloc_allocate();
        }

        return (void*) (current+1);
      }


      /* If Node size greater than the needed Size  */
      else if(current -> size >= (num_bytes + sizeof(metadata)) && (current -> free_status == FREE))
      {
         
        current = split(current, num_bytes);

        /***Check on Split Fnctiion in case it is Failed*/
        if(current -> free_status == FREE)
        {
          
          current = current -> next;
        } 

        else
        {
      
        return (void*) (current+1);
  
        }

      }

      /* If Node size smaller than the needed Size  */
      else if((current -> size <= (num_bytes + sizeof(metadata))) || (current -> free_status == RESERVED))
      {
      
        current = current -> next;
      }

    }

      old_malloc = TRUE;
      
      
      boolean ret = HmmAlloc_allocate();
     if(ret == FALSE)
         {
      
            return NULL_PTR;
         }

         if(num_bytes <= PROG_BREAK_PUSH_SIZE)
         {
           current = free_list;
           

           while(current != NULL_PTR)
    {
        

        if(current -> next == NULL_PTR)
      {
       tail = current;
      }

      if(current -> size == (num_bytes + sizeof(metadata)) && (current -> free_status == FREE) )
      {
       
        current -> free_status == RESERVED;
        
        if(current -> next == NULL_PTR)
        {
          HmmAlloc_allocate();
        }

        return (void*) (current+1);
      }

      else if(current -> size >= (num_bytes + sizeof(metadata)) && (current -> free_status == FREE))
      {
        
        current = split(current, num_bytes);
        if(current -> free_status == FREE)
        {
         
          current = current -> next;
        } 
        else
        {
      
        return (void*) (current+1);
        
        }

      }

      else if((current -> size <= (num_bytes + sizeof(metadata))) || (current -> free_status == RESERVED))
      {
      
        current = current -> next;
      }
    }


         }

         else
         {
            int i;
            int n = (num_bytes / PROG_BREAK_PUSH_SIZE);
            for(i=0;i<n;i++)
            {
              HmmAlloc_allocate(); 
            }
             
             while(current != NULL_PTR)
    {
       

        if(current -> next == NULL_PTR)
      {
       tail = current;
      }

      if(current -> size == (num_bytes + sizeof(metadata)) && (current -> free_status == FREE) )
      {
        printf("Entered cond.4 \n");
        current -> free_status == RESERVED;
        
        if(current -> next == NULL_PTR)
        {
          HmmAlloc_allocate();
        }

        return (void*) (current+1);
      }

      else if(current -> size >= (num_bytes + sizeof(metadata)) && (current -> free_status == FREE))
      {
         
        current = split(current, num_bytes);
        if(current -> free_status == FREE)
        {
          
          current = current -> next;
        } 
        else
        {
      
        return (void*) (current+1);
       
        }

      }

      else if((current -> size <= (num_bytes + sizeof(metadata))) || (current -> free_status == RESERVED))
      {
        

        current = current -> next;
      }

    }
         }

          return return_ptr;

}



/************************************************************************************
* Service Name: HmmAlloc_allocate
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): Nothing
* Parameters (inout): None
* Parameters (out): None
* Return value: boolean 
* Description:Initializes the Heap and Pushes Program break Block Size
************************************************************************************/

static  boolean HmmAlloc_allocate(void)
{     

    
    metadata* Current_Prog_brk;
     Current_Prog_brk = (metadata*) sbrk(0);
    
     free_list = (metadata*)sbrk(PROG_BREAK_PUSH_SIZE);

     Current_Prog_brk = (metadata*) sbrk(0);
    
     if(free_list == (void*)1)
     {
        
        return FALSE;
     }

      

     free_list -> size = (PROG_BREAK_PUSH_SIZE - sizeof(METADATA_ALIGNED));
     free_list -> next = NULL_PTR;
     free_list -> free_status = FREE;
     
     if(count == 0)
     {
        free_list -> prev = NULL_PTR;
        head = free_list;
     }
     else
     {
        
        free_list -> prev = tail;
     }

     count++;
     return TRUE;

     
}



/************************************************************************************
* Service Name: split
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): Pointer on The Node needed to be Splited , Num of Bytes needed
* Parameters (inout): None
* Parameters (out): None
* Return value: metadata pointer 
* Description: Splits a node of Bigger size to two nodes
************************************************************************************/

metadata* split (metadata * current, int num_bytes)
{
    
  size_t size_needed = sizeof(metadata) + num_bytes;
  
  if((current->size - size_needed) <= sizeof(metadata))
  {
  
    return current; 
  }

  metadata* header1 = current; 
  size_t oldsize = current->size; 
  metadata* next = current->next; 
  current = current +1; 
  char *ptr = (char*) current; 
  ptr = ptr + num_bytes; 

  current = (metadata*)ptr; 

  metadata* header2 = current;
  header2->size = oldsize - (num_bytes + sizeof(metadata)); 
  header1->size = oldsize - header2->size; 
  header1->next = header2; 
  header2->prev = header1; 

  if(next !=NULL)
  {
    
    next->prev = header2; 
  }
  header2->next = next; 
  header1->free_status = RESERVED; 
  header2->free_status = FREE; 
  
  return header1; 

}



/************************************************************************************
* Service Name: HmmFree
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): Void Pointer 
* Parameters (inout): None
* Parameters (out): None
* Return value: void pointer 
* Description: Free the node Specified by the ptr
************************************************************************************/

 void HmmFree(void* ptr)
 {
   if(ptr == NULL_PTR)
   {  
    return;
   }

   ptr = ptr - sizeof(metadata); 
   metadata * current = head; 

  while(current -> next != NULL_PTR)
   {
     current = current -> next;
   }

   tail = current;

   while(current != NULL_PTR)
   {
    if((current == ptr) && (current -> free_status == RESERVED))
    {
        current -> free_status == FREE;
        merge(current);
        return;
    }

    current = current -> next;

   }
   
   if( (tail -> free_status == FREE) && (tail -> size >= PROG_BREAK_POP_SIZE ) )
   {
       
          metadata* sacrifice = tail;
          
           tail = tail -> prev;
           tail -> next = NULL_PTR;
           sbrk(-(sacrifice -> size));
           sacrifice = NULL_PTR;
   }

   return; 
 }



/************************************************************************************
* Service Name: merge
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in):  Pointer on the Current node
* Parameters (inout): None
* Parameters (out): None
* Return value: void  
* Description: Merge the node to any adjacent node(IF FREE)
************************************************************************************/

 void merge (metadata* current)
 {

     uint32 totalsize; 


  if(current == head)
  {
    if((current-> next -> free_status) == FREE)
    {
      totalsize = current->size + current->next->size; 
      current->next = current->next->next; 
      if(current->next != NULL_PTR)
      {
         current->next->prev = current; 
      }
      current->size = totalsize; 
    }
    return; 
  }
  
  if(current == tail)
  {
    if(current->prev->free_status == FREE)
    {
      totalsize = current->size + current->prev->size; 
      current = current->prev; 
      current->next = NULL; 
      current->size = totalsize; 
    }
    return;
  }

  if(current->prev != NULL_PTR)
  {
      if((current->prev->free_status) == FREE)
      {
        totalsize = current->size + (current->prev->size); 
        current = current->prev; 
        current->next = (current->next->next); 
        if(current->next != NULL)
        {
           current->next->prev = current; 
        }
      current->size = totalsize; 
      }
  }

  if(current->next != NULL_PTR)
  {
    if(current->next->free_status == FREE)
    {
      totalsize = current->size + current->next->size;
      current->next = current->next->next; 
      if(current->next != NULL)
      {
        current->next->prev = current; 
      }
      current->size = totalsize; 
    }
  }
  return;

 }



/************************************************************************************
* Service Name: HMM_Calloc
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in):  Number of items, Size of item
* Parameters (inout): None
* Parameters (out): None
* Return value: void pointer  
* Description: Allocates memory as HmmAlloc but with Zero initialization
************************************************************************************/

 void* HMM_Calloc(size_t numitems, size_t size)
 {
   int num_bytes = (numitems*size);

    metadata* return_ptr = (metadata*)HmmAlloc(num_bytes);

    if(return_ptr == NULL_PTR)
    {
      return NULL_PTR;
    }

    int* end_address = (int*) ((char*)return_ptr + num_bytes);
    int* address = (int*) return_ptr;

    while(address <= end_address)
    {
      
      *(address) = 0;
       address++;

    }

    return return_ptr;
 }



/************************************************************************************
* Service Name: HMM_Realloc
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in):  Pointer , New needed size for the block
* Parameters (inout): None
* Parameters (out): None
* Return value: void pointer  
* Description: reallocates memory and return a new pointer at the start of the new node
************************************************************************************/

 void* HMM_Realloc(void* ptr , size_t size )
 {
  metadata* current = (metadata*)ptr;
   void* return_ptr = NULL_PTR;

   if(ptr == NULL_PTR)
   {
    printf("Entered Null pointetr to realloc\n");
    return_ptr =  HmmAlloc(size);
    return return_ptr;
   }

  if(size == 0)
  {
    printf("Entered size  = 0 \n");
    HmmFree(ptr);  
    return_ptr =  HmmAlloc(0);
    return return_ptr;
  }

  int needed_size = size - (current->size);

  // if( (current -> next ->free_status == FREE) && (current -> next ->size >= needed_size ) )
  // {
  //   printf("Realloc wants to merge \n");
  //   if(current -> next ->size == needed_size )
  //   {
  //     merge(ptr);
  //     return ptr;
  //   }
  //   else
  //   {
  //     return_ptr = split (ptr,needed_size);
  //     merge(return_ptr);
  //     return return_ptr;

  //   }
         
  //  }
   //else
  {
    printf("Call malloc \n");
       return_ptr = HmmAlloc(size);
       return return_ptr;
  }

 }