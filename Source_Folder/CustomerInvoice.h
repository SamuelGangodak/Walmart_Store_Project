/*
    Created by :
        Samuel Gangodak
*/

# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# ifndef CUSTOMERINVOICE_H
# define CUSTOMERINVOICE_H
# define MAX_NAME_SIZE 20
# define DEFAULT_SIZE 10
# define MAP_SIZE 100
# define INVOICE_SIZE 10

typedef struct bill_item_node{
    int item_id;
    char *item_name;
    int quantity;
    int expiry_date[3];
    float price;
    struct bill_item_node *next;
}bill_item;

typedef struct user_bill{
    bill_item *item_node;
    float total_cost;
}bill;

bill Invoice[INVOICE_SIZE];

void initialize_bill();

bill_item* create_bill_item(int id, char *name, int quantity, int expiry_date[], float price);

void print_bill_node(FILE *fptr, bill_item *node);

void print_bill(FILE *fptr, int userId);

# endif