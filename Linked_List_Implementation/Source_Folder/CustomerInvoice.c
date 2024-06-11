/*
    Created by :
        Samuel Gangodak 
*/

# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include"CustomerInvoice.h"

void initialize_bill() {
   for(int i=0; i<INVOICE_SIZE; i++) {
        Invoice[i].item_node=NULL;
        Invoice[i].total_cost=0.00;
   }
}

bill_item* create_bill_item(int id, char *name, int quantity, int expiry_date[], float price) {
    bill_item *new_Item=(bill_item*)malloc(sizeof(bill_item));
    new_Item->item_id=id;
    new_Item->item_name=(char*)malloc(MAX_NAME_SIZE);
    strcpy(new_Item->item_name, name);
    new_Item->quantity=quantity;
    for(int i=0; i<3; i++) {
        new_Item->expiry_date[i]=expiry_date[i];
    }
    new_Item->price=price;
    new_Item->next=NULL;
    return new_Item;
}

void print_bill_node(FILE *fptr, bill_item *node) {
    fprintf(fptr, "Item ID : %d\t", node->item_id);
    fprintf(fptr, "Item Name : %s\t", node->item_name);
    fprintf(fptr, "Expiry Date : ");
    for(int i=0; i<2; i++) {
        fprintf(fptr, "%d-", node->expiry_date[i]);
    }
    fprintf(fptr, "%d\t", node->expiry_date[2]);
    fprintf(fptr, "Price per quantity: %.2f\t", node->price);
    fprintf(fptr, "Quantity : %d\t", node->quantity);
    fprintf(fptr, "Total price : %.2f/-\n", (node->price)*(node->quantity));
}

void print_bill(FILE *fptr, int userId) {
    int index=(userId/10000)-1;
    fprintf(fptr, "_______________________________________________________INVOICE_______________________________________________________\n");
    fprintf(fptr, "\nUSER ID : %d\n\n", userId);
    fprintf(fptr, "Items bought : \n");
    bill_item *curr=Invoice[index].item_node;
    while(curr!=NULL) {
        print_bill_node(fptr, curr);
        curr=curr->next;
    }
    fprintf(fptr, "\n\nTotal cost : %.2f\n\n", Invoice[index].total_cost);
}

