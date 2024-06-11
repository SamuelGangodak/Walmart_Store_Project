/*
    Created by :
        Samuel Gangodak
*/

# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include "CustomerInvoice.h"
# include "Store_Management.h"
# include "Map.h"

int aisle_index=0;

item* create_item(int id, char *name, int quantity, int expiry_date[], int threshold_quantity, float price) {
    item *new_Item=(item*)malloc(sizeof(item));
    new_Item->item_id=id;
    new_Item->item_name=(char*)malloc(MAX_NAME_SIZE);
    strcpy(new_Item->item_name, name);
    new_Item->quantity=quantity;
    for(int i=0; i<3; i++) {
        new_Item->expiry_date[i]=expiry_date[i];
    }
    new_Item->threshold_quantity=threshold_quantity;
    new_Item->price=price;
    new_Item->next=NULL;
    return new_Item;
}

aisle* create_aisle(int id, char *product_type) {
    aisle *new_aisle=(aisle*)malloc(sizeof(aisle));
    new_aisle->aisle_id=id;
    new_aisle->product_type=(char*)malloc(MAX_NAME_SIZE);
    strcpy(new_aisle->product_type, product_type);
    new_aisle->head=NULL;
    return new_aisle;
}

void insert_item(int index,item* item_node) {
    item *curr=aisle_array[index]->head;
    if(curr==NULL || curr->item_id>item_node->item_id) {
        item_node->next=aisle_array[index]->head;
        aisle_array[index]->head=item_node;
    }
    else {
        while(curr->next!=NULL && curr->next->item_id<item_node->item_id) {
            curr=curr->next;
        }
        item_node->next=curr->next;
        curr->next=item_node;
    }
}

void add_item(char *product_type, int id, char *name, int quantity, int expiry_date[], int threshold_quantity, float price) {
    item *new_Item=create_item(id, name, quantity, expiry_date, threshold_quantity, price);
    int index=(new_Item->item_id/10000)-1;
    if(aisle_array[index]==NULL) {
        aisle_array[index]=create_aisle(index+1, product_type);
        aisle_array[index]->head=new_Item;
    }
    else {
        insert_item(index, new_Item);
    }
}

void print_Item(FILE *fptr, item* curr) {
    if(curr!=NULL) {
        int aisle_id=(curr->item_id)/10000;
        fprintf(fptr, "Aisle_id : %d\tItem_id : %d\tPrice : %.2f\t", aisle_id, curr->item_id, curr->price);
        fprintf(fptr, "Item : %s\tQuantity : %d\t", curr->item_name, curr->quantity);
        fprintf(fptr, "Threshold quantity : %d\t", curr->threshold_quantity);
        fprintf(fptr, "Expiry date : ");
        for(int i=0; i<2; i++) {
            fprintf(fptr, "%d-", curr->expiry_date[i]);
        }
        fprintf(fptr, "%d\n", curr->expiry_date[2]);
    }
}

void delete_item(FILE *fptr, int id) {
    int index=(id/10000)-1;
    if(aisle_array[index]==NULL) {
        fprintf(fptr, "No item of aisle_id:%d exists\n", index+1);
    }
    else {
        item *curr=aisle_array[index]->head;
        item *prev=NULL;
        while(curr!=NULL && curr->item_id<id) {
            prev=curr;
            curr=curr->next;
        }
        if(curr==NULL || curr->item_id!=id) {
            fprintf(fptr, "No item with id %d exists\n", id);
        }
        else {
            if(prev==NULL) {
                aisle_array[index]->head=curr->next;
            }
            else {
                prev->next=curr->next;
            }
            free(curr->item_name);
            free(curr);
        }
    }
}

void add_aisle(char *product_type) {
    int i=0;
    while(i<aisle_index && aisle_array[i]!=NULL && strcmp(aisle_array[i]->product_type, product_type)!=0) i++;
    if(i==aisle_index) {
        aisle_array[aisle_index]=create_aisle(aisle_index+1, product_type);
        aisle_index++;
    }
    else if(aisle_array[i]==NULL) {
        aisle_array[i]=create_aisle(i+1, product_type);
    }
}

void delete_aisle(FILE *fptr, int id) {
    int index=id-1;
    if(aisle_array[index]==NULL) {
        fprintf(fptr, "No aisle with id %d exists\n", index+1);
    }
    else {
        while(aisle_array[index]->head!=NULL) {
            item *temp=aisle_array[index]->head;
            aisle_array[index]->head=(aisle_array[index]->head)->next;
            free(temp);
        }
        free(aisle_array[index]);
        aisle_array[index]=NULL;
    }
}

boolean compare_dates(int expiry_date[], int required_date[], int curr_date[]) {
    boolean ret_val=FALSE;
    if(curr_date[2]<expiry_date[2] && expiry_date[2]<required_date[2]) {
        ret_val=TRUE; 
    }
    else if(expiry_date[2]==required_date[2] ) {
        if(expiry_date[1]==required_date[1]) {
            if(expiry_date[0]<=required_date[0]) {
                if(curr_date[2]<expiry_date[2]) {
                    ret_val=TRUE;
                }
                else if(curr_date[2]==expiry_date[2] ) {
                    if(curr_date[1]==expiry_date[1] && expiry_date[0]>=curr_date[0]) {
                        ret_val=TRUE;
                    }
                    else if(curr_date[1]<expiry_date[1]) {
                        ret_val=TRUE;
                    }
                }
            }
        }
        else if(expiry_date[1]<required_date[1]) {
            if(curr_date[2]<expiry_date[2]) {
                ret_val=TRUE;
            }
            else if(curr_date[2]==expiry_date[2] ) {
                if(curr_date[1]==expiry_date[1] && expiry_date[0]>=curr_date[0]) {
                    ret_val=TRUE;
                }
                else if(curr_date[1]<expiry_date[1]) {
                    ret_val=TRUE;
                }
            }
        }
    }
    return ret_val;
}

void check_availability(FILE* fptr, int item_id, int req_quantity, int delivery_date[], int curr_date[]) {
    int index=(item_id/10000)-1;
    if(aisle_array[index]!=NULL) {
        item *curr=aisle_array[index]->head;
        while(curr!=NULL && curr->item_id<item_id) curr=curr->next;
        if(curr!=NULL && curr->item_id==item_id) {
            if(curr->quantity>req_quantity) {
                if(compare_dates(curr->expiry_date, delivery_date, curr_date)) fprintf(fptr, "Status : Not Available\n");
                else fprintf(fptr, "Status : Avaliable\n");
                print_Item(fptr, curr);
            }
            else {
                fprintf(fptr, "Status : Not Avaliable\n");
                print_Item(fptr, curr);
            }
        }
        else {
            fprintf(fptr, "No item with id : %d exists\n", item_id);
        }
    }
}

void inspect_quantity(FILE *fptr) {
    fprintf(fptr, "\n\nItems needed to be ordered : \n\n");
    for(int i=0; i<aisle_index; i++) {
        if(aisle_array[i]!=NULL) {
            item *curr=aisle_array[i]->head;
            while(curr!=NULL) {
                if(curr->quantity<curr->threshold_quantity) {
                    print_Item(fptr, curr);
                }
                curr=curr->next;
            }
        }
    }
}

void find_date(int curr_date[], int required_date[], int day) {
    if(curr_date[1]==1 || curr_date[1]==3 || curr_date[1]==5 || curr_date[1]==7 || curr_date[1]==8 || curr_date[1]==10 || curr_date[1]==12) {
        required_date[0]=(curr_date[0]+day)%31;
        if(required_date[0]==0) required_date[0]=31;
        if(curr_date[0]+day>31) {
            required_date[1]=(curr_date[1]+1)%12;
            if(required_date[1]==0) required_date[1]=12;
            if(curr_date[1]==12) {
                required_date[2]=curr_date[2]+1;
            }
        }
        else {
            required_date[1]=curr_date[1];
            required_date[2]=curr_date[2];
        }
    }
    else if(curr_date[1]==2) {
        if(curr_date[2]%4==0) {
            required_date[0]=(curr_date[0]+day)%29;
            if(required_date[0]==0) required_date[0]=29;
            if(curr_date[0]+day>29) {
                required_date[1]=(curr_date[1]+1)%12;
                required_date[2]=(curr_date[2]);
            }
            else {
                required_date[1]=curr_date[1];
                required_date[2]=curr_date[2];
            }
        }
        else {
            required_date[0]=(curr_date[0]+day)%28;
            if(required_date[0]==0) required_date[0]=28;
            if(curr_date[0]+day>28) {
                required_date[1]=(curr_date[1]+1)%12;
                required_date[2]=curr_date[2];
            }
            else {
                required_date[1]=curr_date[1];
                required_date[2]=curr_date[2];
            }
        }
    }
    else {
        required_date[0]=(curr_date[0]+day)%30;
        if(required_date[0]==0) required_date[0]=30;
        if(curr_date[0]+day>30) {
            required_date[1]=(curr_date[1]+1)%12;
            if(required_date[1]==0) required_date[1]=12;
            required_date[2]=curr_date[2];
        }
        else {
            required_date[1]=curr_date[1];
            required_date[2]=curr_date[2];
        }
    }
}

void inspect_readyToEat(FILE *fptr, int curr_date[]) {
    item *curr=aisle_array[3]->head;
    int required_date[3];
    find_date(curr_date, required_date, 7);
    while(curr!=NULL) {
        if(compare_dates(curr->expiry_date, required_date, curr_date)) {
            print_Item(fptr, curr);
            fprintf(fptr, "(WARNING expiring within one week)\n\n");
        }
        else {
            print_Item(fptr, curr);
            fprintf(fptr, "\n");
        }
        curr=curr->next;
    }
}

void inspect_dairyProduct(FILE *fptr, int curr_date[]) {
    item *curr=aisle_array[0]->head;
    int required_date[3];
    find_date(curr_date, required_date, 1);
    while(curr!=NULL) {
        if(compare_dates(curr->expiry_date, required_date, curr_date)) {
            print_Item(fptr, curr);
            fprintf(fptr, "(WARNING expiring within one day)\n\n");
        }
        else {
            print_Item(fptr, curr);
            fprintf(fptr, "\n");
        }
        curr=curr->next;
    }
}

void merge_aisle(char *product_type, int id1, int id2) {
    int index1=id1-1;
    int index2=id2-1;
    aisle *curr_aisle=aisle_array[index1];
    strcpy(curr_aisle->product_type, product_type);
    item *curr=curr_aisle->head;
    if(curr==NULL) {
        curr_aisle->head=aisle_array[index2]->head;
        free(aisle_array[index2]);
        aisle_array[index2]=NULL;
    }
    else {
        while(curr->next!=NULL) curr=curr->next;
        curr->next=aisle_array[index2]->head;
        free(aisle_array[index2]);
        aisle_array[index2]=NULL;
    }
}

void print_aisle(FILE *fptr, aisle *a) {
    item *curr=a->head;
    if(a->head==NULL) {
        fprintf(fptr, "No item exists\n\n");
    }
    while(curr!=NULL) {
        print_Item(fptr, curr);
        curr=curr->next;
    }
}

item* search_item(int item_id, int req_quantity) {
    item* ret_val=NULL;
    int index=(item_id/10000)-1;
    item *curr=aisle_array[index]->head;
    while(curr!=NULL && curr->item_id<item_id) curr=curr->next;
    if(curr!=NULL) {
        if(curr->item_id==item_id) {
            if(curr->quantity>req_quantity) {
                ret_val=curr;
            }
        }
    }
    return ret_val;
}

void purchase_item(int item_id, int req_quantity, int userId) {
    int index=(userId/10000)-1;
    item *ret_item=search_item(item_id, req_quantity);
    if(ret_item!=NULL) {
        bill_item *new_item=create_bill_item(ret_item->item_id, ret_item->item_name, req_quantity, ret_item->expiry_date, ret_item->price);
        bill_item *curr=Invoice[index].item_node;
        if(curr==NULL) {
            Invoice[index].item_node=new_item;
        }
        else {
            while(curr->next!=NULL) {
                curr=curr->next;
            }
            curr->next=new_item;
        }
        ret_item->quantity=ret_item->quantity-req_quantity;
        Invoice[index].total_cost=Invoice[index].total_cost+(ret_item->price)*req_quantity;
    }
}

void purchase_item_user(int id1, int q1, int id2, int q2, int id3, int q3, int id4, int q4, int id5, int q5, int userId) {
    purchase_item(id1, q1, userId);
    purchase_item(id2, q2, userId);
    purchase_item(id3, q3, userId);
    purchase_item(id4, q4, userId);
    purchase_item(id5, q5, userId);
}

int main() {
    FILE *fptr1=fopen("../Input_Folder/input.txt", "r");
    FILE *fptr2=fopen("../Output_Folder/output.txt", "w");
    FILE *fptr3=fopen("../Output_Folder/Bill_output.txt", "w");
    FILE *fptr4=fopen("../Input_Folder/Purchase_input.txt", "r");
    FILE *fptr5=fopen("../Output_Folder/aisle_Info.txt", "w");
    FILE *fptr6=fopen("../Output_Folder/AvailabilityCheck.txt", "w");

    if(fptr1==NULL) printf("Cannot read input.txt\n");
    else if(fptr2==NULL) printf("Cannot open output.txt\n");
    else if(fptr3==NULL) printf("cannot open Bill_output.txt\n");
    else if(fptr4==NULL) printf("cannot read Purchase_input.txt");
    else if(fptr5==NULL) printf("cannot open aisle_Info.txt");
    else if(fptr6==NULL) printf("cannot open AvailabilityCheck.txt");
    else {

        initialize_bill();
        initialize_map();

        char product_type[MAX_NAME_SIZE];
        while(fscanf(fptr1,"%s", product_type)!=EOF) {
            fgetc(fptr1);
            int id, quantity, expiry_date[3], threshold_q;
            float price;
            fscanf(fptr1,"%d", &id);
            fgetc(fptr1);
            char name[MAX_NAME_SIZE];
            fscanf(fptr1,"%s", name);
            fgetc(fptr1);
            fscanf(fptr1,"%d", &quantity);
            fgetc(fptr1);
            for(int i=0; i<3; i++) {
                fscanf(fptr1,"%d", &expiry_date[i]);
                fgetc(fptr1);
            }
            fscanf(fptr1,"%d", &threshold_q);
            fgetc(fptr1);
            fscanf(fptr1,"%f", &price);
            fgetc(fptr1);
            add_aisle(product_type);
            add_item(product_type, id, name, quantity, expiry_date, threshold_q, price);
        }

        fprintf(fptr5, "____________Printing aisle____________\n\n");
        for(int i=0; i<aisle_index; i++) {
            fprintf(fptr5, "Product Type : %s\n", aisle_array[i]->product_type);
            print_aisle(fptr5, aisle_array[i]);
            fprintf(fptr5, "\n\n");
        }

        int curr_date[3];
        printf("Enter today's date : ");
        for(int i=0; i<3; i++) {
            scanf("%d", &curr_date[i]);
        }

        int flag=1;
        while(flag) {
            printf("\n\nEnter the key for corresponding option : \n");
            printf("1 : Print a particular aisle info\n");
            printf("2 : Print a particular item info\n");
            printf("3 : Print all aisle info\n");
            printf("4 : Add an aisle\n");
            printf("5 : Add an item\n");
            printf("6 : Delete an aisle\n");
            printf("7 : Delete an item\n");
            printf("8 : Check availability of a particular item\n");
            printf("9 : Inspect for quantity of items\n");
            printf("10 : Inspect dairy products\n");
            printf("11 : Inspect ready-to-eat items\n");
            printf("12 : Merge aisle\n");
            printf("13 : Generate bill for users\n");
            printf("14 : Calculate frequency of items\n");
            printf("(Enter -1 to Exit)\n\n");
            int key;
            printf("Enter the key : ");
            scanf("%d", &key);
            int id, quantity, expiry_date[3], threshold_q;
            float price;
            char product_type[MAX_NAME_SIZE], name[MAX_NAME_SIZE];
            int date[3];
            int aisle_id1, aisle_id2;
            char str[MAX_NAME_SIZE];
            int id1, q1, id2, q2, id3, q3, id4, q4, id5, q5;
            int userId;
            switch(key) {
                case 1 : 
                         printf("Enter the aisle id : ");
                         scanf("%d", &id);
                         fprintf(fptr5, "____________Printing aisle %d____________\n\n", id);
                         fprintf(fptr5, "\n\nProduct Type : %s\n", aisle_array[id-1]->product_type);
                         print_aisle(fptr5, aisle_array[id-1]);
                         fprintf(fptr5, "\n\n");
                         break;
                case 2 : 
                         printf("Enter the item id : ");
                         scanf("%d", &id);
                         fprintf(fptr2, "____________Printing item with id <%d>____________\n\n", id);
                         item* curr=search_item(id, 0);
                         print_Item(fptr2, curr);
                         break;
                case 3 : 
                         fprintf(fptr5, "____________Printing aisle____________\n\n");
                         for(int i=0; i<aisle_index; i++) {
                             if(aisle_array[i]!=NULL) {
                                fprintf(fptr5, "Product Type : %s\n", aisle_array[i]->product_type);
                                print_aisle(fptr5, aisle_array[i]);
                                fprintf(fptr5, "\n\n");
                             }
                         }
                         break;
                case 4 : 
                         printf("Enter the product type : ");
                         scanf("%s", product_type); 
                         add_aisle(product_type);
                         break; 
                case 5 : 
                         printf("Enter product type : ");
                         scanf("%s", product_type);
                         printf("Enter id : ");
                         scanf("%d", &id);
                         printf("Enter name : ");
                         scanf("%s", name);
                         printf("Enter quantity : ");
                         scanf("%d", &quantity);
                         printf("Enter expiry_date : ");
                         for(int i=0; i<3; i++) {
                            scanf("%d", &expiry_date[i]);
                         }
                         printf("Enter threshold quantity : ");
                         scanf("%d", &threshold_q);
                         printf("Enter price : ");
                         scanf("%f", &price);
                         add_item(product_type, id, name, quantity, expiry_date, threshold_q, price);
                         break;
                case 6 : 
                         printf("Enter the aisle id to be deleted : ");
                         scanf("%d", &id);
                         fprintf(fptr5, "\n\n\n____________Deleting aisle %d____________\n", id);
                         delete_aisle(fptr5, id); 
                         for(int i=0; i<aisle_index; i++) {
                             if(aisle_array[i]!=NULL) {
                                 fprintf(fptr5, "\n\nProduct Type : %s\n", aisle_array[i]->product_type);
                                 print_aisle(fptr5, aisle_array[i]);
                             }
                             else {
                                 fprintf(fptr5, "\n<NULL>\n\n");
                             }
                         }
                         break;
                case 7 : 
                         printf("Enter the item id to be deleted : ");
                         scanf("%d", &id);
                         fprintf(fptr5, "\n\n\n____________Deleting item with id <%d>____________\n", id);
                         delete_item(fptr5, id); 
                         for(int i=0; i<aisle_index; i++) {
                             if(aisle_array[i]!=NULL) {
                                 fprintf(fptr5, "\n\nProduct Type : %s\n", aisle_array[i]->product_type);
                                 print_aisle(fptr5, aisle_array[i]);
                             }
                             else {
                                 fprintf(fptr5, "\n<NULL>\n\n");
                             }
                         }
                         break;
                case 8 : 
                         printf("Enter the item id whose availability is to be checked : ");
                         scanf("%d", &id);
                         printf("Enter the required quantity : ");
                         scanf("%d", &quantity);
                         printf("Enter the expected delivery date : ");
                         for(int i=0; i<3; i++) {
                            scanf("%d", &date[i]);
                         }
                         fprintf(fptr6, "____________Checking for availability of item with id <%d>____________\n\n", id);
                         check_availability(fptr6, id, quantity, date, curr_date);
                         fprintf(fptr6, "\n\n\n");
                         break;
                case 9 : 
                         fprintf(fptr2, "\n____________Inspecting for quantity of items____________\n");
                         inspect_quantity(fptr2);
                         break;
                case 10: 
                         fprintf(fptr2, "\n\n\n____________Inspecting dairy products____________\n\n");
                         inspect_dairyProduct(fptr2, curr_date);
                         break;
                case 11: 
                         fprintf(fptr2, "\n\n\n____________Inspecting ready-to-eat products____________\n\n");
                         inspect_readyToEat(fptr2, curr_date);
                         break;
                case 12: 
                         printf("Enter the aisle ids to be merged : ");
                         scanf("%d %d", &aisle_id1, &aisle_id2);
                         printf("Enter the name of the merged aisle : ");
                         scanf("%s", str);
                         fprintf(fptr5, "\n\n\n____________Merging aisle %d to aisle %d____________\n", aisle_id2, aisle_id1);
                         merge_aisle(str, aisle_id1, aisle_id2);
                         for(int i=0; i<aisle_index; i++) {
                             if(aisle_array[i]!=NULL) {
                                 fprintf(fptr5, "\n\nProduct Type : %s\n", aisle_array[i]->product_type);
                                 print_aisle(fptr5, aisle_array[i]);
                             }
                             else {
                                 fprintf(fptr5, "\n<NULL>\n\n");
                             }
                         }
                         break;
                case 13: 
                         fprintf(fptr2, "\n\n\n____________Purchasing items____________\n\n");
                         while(fscanf(fptr4, "%d %d %d %d %d %d %d %d %d %d %d", &id1, &q1, &id2, &q2, &id3, &q3, &id4, &q4, &id5, &q5, &userId)!=EOF) {
                             fgetc(fptr4);
                             purchase_item_user(id1, q1, id2, q2, id3, q3, id4, q4, id5, q5, userId);
                         }
                         for(int i=0; i<aisle_index; i++) {
                             if(aisle_array[i]!=NULL) {
                                 fprintf(fptr2, "\n\nProduct Type : %s\n", aisle_array[i]->product_type);
                                 print_aisle(fptr2, aisle_array[i]);
                             }
                             else {
                                 fprintf(fptr2, "\n<NULL>\n\n");
                             }
                         }
                         print_bill(fptr3, 12345);
                         print_bill(fptr3, 23456);
                         print_bill(fptr3, 34567);
                         print_bill(fptr3, 45678);
                         print_bill(fptr3, 56789);
                         print_bill(fptr3, 67890); 
                         break;
                case 14: 
                         for(int i=0; i<6; i++) {
                             update_frequency(Invoice[i].item_node);
                         }
                         fprintf(fptr2, "\n\n\n____________Printing the frequencies____________\n\n");
                         print_frequency(fptr2, 11234);
                         print_frequency(fptr2, 21345);
                         print_frequency(fptr2, 15232);
                         print_frequency(fptr2, 44212);
                         print_frequency(fptr2, 51233);
                         print_frequency(fptr2, 24345);
                         print_frequency(fptr2, 41456);
                         print_frequency(fptr2, 42881);
                         print_frequency(fptr2, 42456);
                         print_frequency(fptr2, 55234);
                         print_frequency(fptr2, 12234);
                         break;
                case -1: 
                         flag=0;
                         break;
                default: 
                         printf("Enter a valid key \n");
            }

        }
        // fprintf(fptr2, "\n____________Inspecting for qunatity of items____________\n");
        // inspect_quantity(fptr2);

        // int id;
        // printf("Enter the item id to be deleted : ");
        // scanf("%d", &id);
        // fprintf(fptr5, "\n\n\n____________Deleting item with id <%d>____________\n", id);
        // delete_item(fptr5, id);
        // for(int i=0; i<aisle_index; i++) {
        //     fprintf(fptr5, "\n\nProduct Type : %s\n", aisle_array[i]->product_type);
        //     print_aisle(fptr5, aisle_array[i]);
        // }

        // printf("Enter the item id whose availability is to be checked : ");
        // scanf("%d", &id);
        // int quantity;
        // printf("Enter the required quantity : ");
        // scanf("%d", &quantity);
        // int curr_date[3];
        // printf("Enter today's date : ");
        // for(int i=0; i<3; i++) {
        //     scanf("%d", &curr_date[i]);
        // }
        // int date[3];
        // printf("Enter the expected delivery date : ");
        // for(int i=0; i<3; i++) {
        //     //printf(".");
        //     scanf("%d", &date[i]);
        // }
        // fprintf(fptr6, "\n\n\n____________Checking for availability of item with id <%d>____________\n\n", id);
        // check_availability(fptr6, id, quantity, date, curr_date);

        // fprintf(fptr2, "\n\n\n____________Inspecting dairy products____________\n\n");
        // inspect_dairyProduct(fptr2, curr_date);

        // fprintf(fptr2, "\n\n\n____________Inspecting ready-to-eat products____________\n\n");
        // inspect_readyToEat(fptr2, curr_date);

        

        

        // printf("Enter the aisle id to be deleted : ");
        // scanf("%d", &id);
        // fprintf(fptr5, "\n\n\n____________Deleting aisle %d____________\n", id);
        // delete_aisle(fptr5, id); 
        // for(int i=0; i<aisle_index; i++) {
        //     if(aisle_array[i]!=NULL) {
        //         fprintf(fptr5, "\n\nProduct Type : %s\n", aisle_array[i]->product_type);
        //         print_aisle(fptr5, aisle_array[i]);
        //     }
        //     else {
        //         fprintf(fptr5, "\n<NULL>\n\n");
        //     }
        // }

        // int aisle_id1, aisle_id2;
        // printf("Enter the aisle ids to be merged : ");
        // scanf("%d%d", &aisle_id1, &aisle_id2);
        // char str[MAX_NAME_SIZE];
        // printf("Enter the name of the merged aisle : ");
        // scanf("%s", str);
        // fprintf(fptr5, "\n\n\n____________Merging aisle %d to aisle %d____________\n", aisle_id2, aisle_id1);
        // merge_aisle(str, aisle_id1, aisle_id2);
        // for(int i=0; i<aisle_index; i++) {
        //     if(aisle_array[i]!=NULL) {
        //         fprintf(fptr5, "\n\nProduct Type : %s\n", aisle_array[i]->product_type);
        //         print_aisle(fptr5, aisle_array[i]);
        //     }
        //     else {
        //         fprintf(fptr5, "\n<NULL>\n\n");
        //     }
        // }
    }
    fclose(fptr1);
    fclose(fptr2);
    fclose(fptr3);
    fclose(fptr4);
    return 0;
}