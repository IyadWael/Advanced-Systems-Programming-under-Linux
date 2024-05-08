/******************************************************************************
 *
 * Module: Heap Memory Manager
 *
 * File Name: HMM.h
 *
 * Description: Header file for the HMM based on Linux
 *
 * Author: Iyad Wael Abou-Elmagd
 *
 *******************************************************************************/

#ifndef HMM_H
#define HMM_H


#include <stdio.h>
#include <stdlib.h>


/*******************************************************************************
 *   Preprocessor Macros For initialization and Configuration of the module    *
 *******************************************************************************/
//#define _BSD_SOURCE

#define NULL_PTR  ((void*)0)

#define FREE 1
#define RESERVED 0


#define PROGRAM_INITIALIZED (1)
#define PROGRAM_UNINITIALIZED (0)

#define WORD_SIZE	8

#define ALIGNMENT 	WORD_SIZE

#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

#define SIZE_T_ALIGNED (ALIGN(sizeof(size_t)))
#define METADATA_ALIGNED (ALIGN(sizeof(metadata)))

#define NO_PAGES_TO_ALLOCATE (32)
#define PROG_BREAK_PUSH_SIZE (NO_PAGES_TO_ALLOCATE*4*1024)
#define PROG_BREAK_POP_SIZE  (500 * PROG_BREAK_PUSH_SIZE)



/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* unsigned integer Data Type */
typedef unsigned int uint32; 

/* Boolean Data Type */
typedef unsigned char boolean;

/* Boolean Values */
#ifndef FALSE
#define FALSE       (0u)
#endif
#ifndef TRUE
#define TRUE        (1u)
#endif

/*  Dynamic double linked list Declaration*/

 typedef struct metadata 
{
   size_t size;
   boolean free_status;
   struct metadata* prev;
   struct metadata* next;
} metadata;


/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/




/****************************Private Functions***********************************/



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
static  boolean HmmAlloc_allocate(void);




/******************************Public Functions*************************************/



/************************************************************************************
* Service Name: HmmAlloc
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): Number of Bytes needed to be allocated
* Parameters (inout): None
* Parameters (out): None
* Return value: Void pointer 
* Description: Allocates memory in Heap according to size
************************************************************************************/
void* HmmAlloc(size_t num_bytes);


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
metadata* split (metadata * current, int num_bytes);


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
void HmmFree(void* ptr);


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
void merge (metadata* current);


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
void* HMM_Calloc(size_t numitems, size_t size);


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
void* HMM_Realloc(void* ptr , size_t size );

#endif