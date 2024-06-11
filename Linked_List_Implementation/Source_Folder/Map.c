/*
    Created by :
        Samuel Gangodak
*/

# include<stdio.h>
# include<stdlib.h>
# include<string.h>
# include<limits.h>
# include"Map.h"

void initialize_map() {
    for(int i=0; i<MAP_SIZE; i++) {
        for(int j=0; j<MAP_SIZE; j++) {
            map[i][j].item=NULL;
            map[i][j].count=0;
        }
    }
}

int hash_function(int item_id) {
    int ans=0;
    for(int i=5; i>=1; i--) {
        ans=ans+(i)*((item_id)%10);
        item_id=(item_id)/10;
    }
    return ans%MAP_SIZE;
}

void update_frequency(bill_item *head) {
    bill_item *curr=head;
    while(curr!=NULL) {
        bill_item *node=head;
        int row=hash_function(curr->item_id);
        map[row][0].item=curr;
        while(node!=NULL) {
            if(node->item_id!=curr->item_id) {
                int col=hash_function(node->item_id);
                map[row][col].item=node;
                (map[row][col].count)++;
            }
            node=node->next;
        }
        curr=curr->next;
    }
}

void print_frequency(FILE *fptr, int item_id) {
    int index=hash_function(item_id);
    fprintf(fptr, "Printing for item %s -: \n", map[index][0].item->item_name);
    int maxi=INT_MIN;
    int upper_limit=INT_MAX;
    int flag=1;
    while(flag) {
        for(int j=1; j<MAP_SIZE ; j++) {
            if(map[index][j].count>maxi && map[index][j].count<=upper_limit) maxi=map[index][j].count;
        }
        if(maxi==INT_MIN || maxi==0 || upper_limit==0) {
            flag=0;
        }
        else {
            for(int j=0; j<MAP_SIZE; j++) {
                if(map[index][j].count==maxi) {
                    fprintf(fptr, "Item : %s\t\tItem_id : %d\t\tFrequency : %d\n", map[index][j].item->item_name, map[index][j].item->item_id, map[index][j].count);
                }
            }
            upper_limit=maxi-1;
            maxi=INT_MIN;
        }
    }
    fprintf(fptr, "\n\n");
}