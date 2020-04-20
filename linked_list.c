#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_types.h"


typedef static struct operandNode * optr; /*pointer to operandNode object*/

/*operandNode data type. includes operMemAddress field for IC counter value 
(int) and next field to point to next object in linked list*/
typedef  static struct operandNode
{		   
   int operMemAddress
   optr next
}
operandNode;

/*label data type. includes vars for label name, label type, if label is used as entry, address in memory,pointer to operandNode list which contains all operands' IC values that are using this label (in use only for external labels)*/
typedef static struct label
{			   
	char name[MAX_LABEL_SIZE+1]; /* because there is '/0' in the end */
	labelType labelType
	int isEntry
	int counterValue
	optr operatorNodePtr
} 
label;

typedef static struct labelNode * lptr; /*pointer to labelNode object*/

/*labelNode data type. includes label object and a pointer to the next labelNode object in list*/
typedef static struct labelNode
{		 
   label label;
   lptr next;
} 
labelNode;

/*creates a new operNode and adds to the end of the list.
expects an IC counter value (int)
returns 0 on success, 1 on failure*/
int addOperNode(optr *operPtr, int memAddr)
{
    optr p1,p2;
    optr t;

    t = (optr) malloc(sizeof(operandNode));

    if(!t)
    {
	printf("\n Space allocation error");
	return 1;
    }
    t->operMemAddress = memAddr;
    t->next = NULL;

    p1 = *operPtr;

    if(!p1)
	*operPtr = t;
   else 
   {
      while (p1)
      {
	 p2 = p1;
	 p1 = p1->next;
      }
      p2->next = t;
   }
	return 0;
}

/*creates a new labelNode and adds to the end of the list.
expects pointer to label list head pointer, pointer to label name string, labelType and label value
returns 0 on success, 1 on failure*/
int addLabelToList(lptr *labelPtr, char *labelName, labelType labelType, int labelValue)
{
   lptr p1,p2;
   lptr t;
 

   t = (lptr) malloc(sizeof(labelNode));
   if(!t)
   {
	printf("\n Space allocation error");
	return 1;
   }
   p1 = *labelPtr;
   strcpy(t->label.name, labelName);
   t->label.isEntry = 0;
   t->label.labelType = labelType;
   t->label.counterValue = labelValue;
   t->label.operatorNodePtr = NULL;
   t->next = NULL;

  if(!p1)
  {
	*labelPtr = t;
  }
  else 
  {
      while (p1)
      {
	 if(!strcmp(p1->label.name, t->label.name))
         {
	    printf("Error in line %d: Label %s already exist in labels table, action aborted\n",lineNumber, t->label.name);
	    free(t);
            return 1;
	 }
		p2 = p1;
		p1 = p1->next
     }
     p2->next = t
  }
  return 0;
}

/*goes over the label list and adds for each DATA label the final value of IC counter after the first pass
 expects pointer to label list*/
void updateDataLabels(lptr h, int IC)
{
   while(h)
   {
      if(h->label.labelType == DATA)
	  h->label.counterValue += IC;

      h = h->next;
   }
}

/*search for a label of a given name
expects a pointer to the label list and a pointer to the label name
returns pointer to label or NULL if not found*/
label *findLabel(lptr h, char *labelName)
{
   while(h)
   {
       if(!strcmp(h->label.name, labelName))
	  return &(h->label);
	
       h = h->next
   }
   return 0;
}


/* free memory allocation for operandNode data. expects pointer to head node*/
void freeOperNodes(optr h)
{
   optr t;
   while(h)
   {
	t = h->next
	free(h);
	h = t;
   }
}

/* free memory allocation for labels data. expects pointer to head node*/
void freeLabelList(lptr h)
{
    lptr t;
    while(h)
    {
	t = h->  next;
	freeOperNodes(h->label.operatorNodePtr);
	free(h);
	h = t;
   }
}


