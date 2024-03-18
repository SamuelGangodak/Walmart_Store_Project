/*
    Created by :
        Samuel Gangodak
*/

# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# ifndef MAP_H
# define MAP_H
# include"CustomerInvoice.h"
# define MAX_NAME_SIZE 20
# define DEFAULT_SIZE 10
# define MAP_SIZE 100
# define INVOICE_SIZE 10

typedef struct item_map{
    bill_item *item;
    int count;
}item_map;

item_map map[MAP_SIZE][MAP_SIZE];

void initialize_map();

int hash_function(int item_id);

void update_frequency(bill_item *head);

void print_frequency(FILE *fptr, int item_id);

# endif