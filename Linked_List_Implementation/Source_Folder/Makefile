all : CustomerInvoice.o Store_Management.o Map.o
	gcc CustomerInvoice.o Store_Management.o Map.o -o final.exe

CustomerInvoice.o : CustomerInvoice.c 
	gcc -c CustomerInvoice.c 

Map.o : Map.c 
	gcc -c Map.c 

Store_Management.o : Store_Management.c 
	gcc -c Store_Management.c

clean : 
	del *.o *.exe