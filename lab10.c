#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AUTHOR_SIZE 25
#define TITLE_SIZE 50

typedef struct book
{
    char author[AUTHOR_SIZE];
    char title[TITLE_SIZE];
    int yearPublished;
    int numOfPages;
    double price;
    struct book *next;
} TBook;

int cmpBookAuthor(const TBook *a, const TBook *b)
{
    return strcmp(a->author, b->author);
}

bool authorStartsWithP(const TBook *book)
{
    return (book->author[0] == 'P' || book->author[0] == 'p');
}

TBook *listInsert(TBook *list, TBook *book, int (*comp)(const TBook *, const TBook *))
{
    if (list == NULL)
    {
        book->next = NULL;
        return book;
    }

    TBook *p = list;
    TBook *pp = NULL;
    while (p != NULL)
    {
        if (comp(p, book) > 0)
        {
            book->next = p;
            if (pp != NULL)
            {
                pp->next = book;
                return list;
            }
            return book;
        }
        pp = p;
        p = p->next;
    }

    pp->next = book;
    book->next = NULL;

    return list;
}

TBook *listSort(TBook *list, int (*comp)(const TBook *, const TBook *))
{
    TBook *result = NULL;

    TBook *p = list;
    TBook *np = NULL;

    while (p != NULL)
    {
        np = p->next;
        result = listInsert(result, p, comp);
        p = np;
    }
    return result;
}

TBook *removeElements(TBook *list, bool (*filter)(const TBook *))
{
    TBook *p = list;
    TBook *pp = NULL;

    while (p != NULL)
    {
        if (filter(p))
        {
            if (pp != NULL)
            {
                pp->next = p->next;
            }
            else
            {
                list = p->next;
            }
            p = p->next;
        }
        else
        {
            pp = p;
            p = p->next;
        }
    }
    return list;
}

void printList(TBook *p)
{
    printf("%-*s | %-*s | %5s | %5s | %6s\n", AUTHOR_SIZE, "Author", TITLE_SIZE, "Title", "Year", "Pages", "Price");
    printf("--------------------------------------------------------------------------------------------------------\n");
    while (p != NULL)
    {
        printf("%-*s | %-*s | %5d | %5d | %6.2lf\n", AUTHOR_SIZE,
               p->author, TITLE_SIZE, p->title, p->yearPublished,
               p->numOfPages, p->price);
        p = p->next;
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    TBook *list = NULL;
    TBook *p = NULL;

    char authorTemp[AUTHOR_SIZE];
    char titleTemp[TITLE_SIZE];
    int yearTemp, pagesTemp;
    double priceTemp;

    FILE *file = fopen("books.csv", "r");
    while (fscanf(file, "%[^,],%[^,],%d,%d,%lf\n", authorTemp, titleTemp, &yearTemp,
                  &pagesTemp, &priceTemp) != EOF)
    {
        TBook *newBook = (TBook *)malloc(sizeof(TBook));
        strcpy(newBook->author, authorTemp);
        strcpy(newBook->title, titleTemp);
        newBook->yearPublished = yearTemp;
        newBook->numOfPages = pagesTemp;
        newBook->price = priceTemp;
        newBook->next = NULL;

        if (list == NULL)
        {
            list = newBook;
        }
        else
        {
            p->next = newBook;
        }
        p = newBook;
    }
    fclose(file);

    printList(list);

    list = listSort(list, cmpBookAuthor);
    printList(list);

    list = removeElements(list, authorStartsWithP);
    printList(list);

    return 0;
}
