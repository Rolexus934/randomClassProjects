#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// define limits
#define MAX_NAME 100
#define MAX_LINE 100
typedef struct
{
    int id;
    char name[MAX_NAME];
    int numberOfUnits;
    int price;
} Product;

// counts the number of products on the database
int parseNumberOfProducts(FILE *fptr);
// takes a line from database.txt and fills the Products array
void parseProductString(char *productString, Product *products);
// parse the raw file strings and puts the information on the array
void parseProducts(Product *products, FILE *fptr, int nProducts);
// adds a product
int findPosById(Product *products, int id, int n);

int main()
{
    // starting file read
    FILE *fptr = fopen("database.txt", "r");

    // initializing the database
    int numberOfProducts = parseNumberOfProducts(fptr);
    Product products[numberOfProducts];
    parseProducts(products, fptr, numberOfProducts);
    fclose(fptr);

    // setting up modificaciones.txt
    FILE *modificacionesPtr = fopen("modifications.txt", "w");
    // da menu
    int ins;
    do
    {
        int id;

        printf("Bienvenido al menú de administración de inventario, introduce un número del 1 al 5 para seleccionar una instrucción del menú. \n");
        printf("1.- Agregar productos a la base de datos\n");
        printf("2.- Vender productos de la base de datos\n");
        printf("3.- Devolver productos a la base de datos\n");
        printf("4.- Consultar un producto de la base de datos\n");
        printf("5.- Salir del programa\n");
        scanf("%d", &ins);

        switch (ins)
        {
        case 1:
        {
            printf("Ingrese el ID del producto: \n");
            scanf("%d", &id);

            int pos = findPosById(products, id, numberOfProducts);
            int quantity;
            if (pos == -1)
            {
                printf("ERROR: No existe ningún elemento con el id proporcionado \n");
            }
            else
            {
                printf("Ingrese la cantidad de Productos a añadir: ");
                scanf("%d", &quantity);
                products[pos].numberOfUnits += quantity;

                // add transaction to modificaciones.txt
                fprintf(modificacionesPtr, "%d 'C' %d\n", products[pos].id, quantity);
            }
        }
        break;
        case 2:
        {
            printf("Ingrese el ID del producto: \n");
            scanf("%d", &id);

            int pos = findPosById(products, id, numberOfProducts);
            int quantity;
            if (pos == -1)
            {
                printf("ERROR: No existe ningún elemento con el id proporcionado \n");
            }
            else
            {
                printf("Ingrese la cantidad de Productos a vender ");
                scanf("%d", &quantity);
                // verifying
                if (quantity > products[pos].numberOfUnits)
                {
                    printf("ERROR: El numero de elementos a vender excede los elementos en existencia\n");
                }
                else
                {
                    products[pos].numberOfUnits -= quantity;
                    // add transaction to modificaciones.txt
                    fprintf(modificacionesPtr, "%d 'V' %d\n", products[pos].id, quantity);
                }
            }
            break;
        }
        case 3:
        {
            printf("Ingrese el ID del producto: \n");
            scanf("%d", &id);

            int pos = findPosById(products, id, numberOfProducts);
            int quantity;
            if (pos == -1)
            {
                printf("ERROR: No existe ningún elemento con el id proporcionado \n");
            }
            else
            {
                printf("Ingrese la cantidad de Productos a devolver: ");
                scanf("%d", &quantity);
                products[pos].numberOfUnits += quantity;
                // add transaction to modificaciones.txt
                fprintf(modificacionesPtr, "%d 'D' %d\n", products[pos].id, quantity);
            }

            break;
        }
        case 4:
        {
            printf("Ingrese el ID del producto: \n");
            scanf("%d", &id);

            int pos = findPosById(products, id, numberOfProducts);
            // verifying existence of the product
            if (pos == -1)
            {
                printf("ERROR: No existe ningún elemento con el id proporcionado \n");
            }
            else
            {
                printf("ID: %d\n", products[pos].id);
                printf("Nombre del Producto: %s\n", products[pos].name);
                printf("Cantidad Disponible: %d\n", products[pos].numberOfUnits);
                printf("Precio: $%d: \n", products[pos].price);
            }
            break;
        }
        }

    } while (ins != 5);

    return 0;
}

int parseNumberOfProducts(FILE *fptr)
{
    int i = 0;
    char fileStr[MAX_LINE];
    while (fgets(fileStr, MAX_LINE, fptr))
    {
        i++;
    }
    fseek(fptr, 0, 0);
    return i;
}

void parseProductString(char *productString, Product *products, int pos)
{
    char *token = strtok(productString, " ");
    products[pos].id = atoi(token);

    token = strtok(NULL, " ");
    strncpy(products[pos].name, token, MAX_NAME);

    token = strtok(NULL, " ");
    products[pos].numberOfUnits = atoi(token);

    token = strtok(NULL, " ");
    products[pos].price = atoi(token);
}
void parseProducts(Product *products, FILE *fptr, int nProducts)
{
    char lineStr[MAX_LINE];
    int pos = 0;
    while (fgets(lineStr, MAX_LINE, fptr))
    {
        parseProductString(lineStr, products, pos);
        pos++;
    }
}

int findPosById(Product *products, int productID, int n)
{
    int pos = -1;
    for (int x = 0; x < n; x++)
    {
        if (products[x].id == productID)
        {
            pos = x;
        }
    }
    return pos;
}
