#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AUTHOR_SIZE 25
#define TITLE_SIZE 50
#define LINE_SIZE 100

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

void saveToFile(TBook *p)
{
    FILE *file = fopen("output.txt", "w");
    fprintf(file, "%-*s | %-*s | %5s | %5s | %6s\n", AUTHOR_SIZE, "Author", TITLE_SIZE, "Title", "Year", "Pages", "Price");
    fprintf(file, "--------------------------------------------------------------------------------------------------------\n");
    while (p != NULL)
    {
        fprintf(file, "%-*s | %-*s | %5d | %5d | %6.2lf\n", AUTHOR_SIZE,
                p->author, TITLE_SIZE, p->title, p->yearPublished,
                p->numOfPages, p->price);
        p = p->next;
    }
    fclose(file);
}

TBook *scanListFromKeyboard()
{
    TBook *list = NULL;
    TBook *p = NULL;

    char authorTemp[AUTHOR_SIZE];
    char titleTemp[TITLE_SIZE];
    int yearTemp, pagesTemp;
    double priceTemp;

    printf("Enter <Author name>,<Book title>,<Year of publishing>,<Number of pages>,<Price>\n");
    printf("To stop adding items enter any symbol\n");
    char line[LINE_SIZE];
    while (fgets(line, sizeof line, stdin) != NULL)
    {
        if (sscanf(line, "%[^,],%[^,],%d,%d,%lf\n", authorTemp, titleTemp, &yearTemp,
                   &pagesTemp, &priceTemp) != 5)
            break;
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

    return list;
}

TBook *scanListFromFile()
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

    return list;
}

int main(int argc, char const *argv[])
{
    TBook *list = NULL;
    while (true)
    {
        printf("Enter a command:\n");
        printf(" k - enter list from keyboard\n");
        printf(" f - read list from a file\n");
        printf(" p - print list\n");
        printf(" o - output list to a file\n");
        printf(" r - remove all items which authors starts with 'P'\n");
        printf(" s - sort list\n");
        printf(" any other key to exit\n");

        char nextAction = getchar();
        getchar();

        if (nextAction == 'k')
        {
            list = scanListFromKeyboard();
        }
        else if (nextAction == 'f')
        {
            list = scanListFromFile();
        }
        else if (nextAction == 'p')
        {
            printList(list);
        }
        else if (nextAction == 'o')
        {
            saveToFile(list);
        }
        else if (nextAction == 'r')
        {
            list = removeElements(list, authorStartsWithP);
        }
        else if (nextAction == 's')
        {
            list = listSort(list, cmpBookAuthor);
        }
        else
        {
            break;
        }
    }
    return 0;
}
