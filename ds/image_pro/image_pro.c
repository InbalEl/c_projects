/******************************************************************************
* Task name: image processing
* Develoer: Inbal Elmalech
* Reviewer: Guy Barazany
* Date: Aug 16, 2020
******************************************************************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "photoutils.h"
#define MIN(x, y) ((x) < (y) ? (x) : (y))
/******************************   STRUCTURES   *******************************/

const double emboss_filter[3][3] = {
                                    { -1,-1,+0 },
                                    { -1,+0,+1 },
                                    { +0,+1,+1 }
                                    };

const double edge_filter[3][3] = {
                                    {-1,-1,-1},
                                    {-1,+8,-1},
                                    {-1,-1,-1}
                                    };

const double sharp_filter[3][3] = {
                                    {+0,-1,+0},
                                    {-1,+5,-1},
                                    {+0,-1,+0}
                                    };

typedef struct index
{
    pthread_t ptid;
    BMP_IMAGE *img1;
    BMP_IMAGE *img2;
    int j;
    int end;
}index_t;

/****************************   DECLARATIONS   *******************************/
void *CopyAndApplyFilter(void *param);
/*static void CopyImage(char *src, char *dest);
static void EmbossFilter(char *src);
static void EdgeFilter(char *src);
static void SharpFilter(char *src);*/
/********************************  MAIN  *************************************/

int main(int argc, char const **argv)
{
    char src[] = "tardis.bmp";
    char dest[] = "tardis_copy.bmp";
	time_t start, end;
    FILE *img_ptr = fopen(src, "r");
    FILE *new_img_ptr = fopen(dest, "w+");
    BMP_IMAGE *loaded_img = NULL;
    BMP_IMAGE *new_img = NULL;
    index_t *index_arr = NULL;
    int nof_threads = 0;
    int i = 0;
    unsigned int section_size = 0;

    if (argc < 2)
    {
        printf("no number of threads received\n");
    }
    
    if (NULL == img_ptr || (NULL == (loaded_img = LoadImage(img_ptr))))
    {
        printf("no soup for you\n");
        return (1);
    }

    nof_threads = atoi(argv[1]);

    if (NULL == (index_arr = (index_t *)malloc((nof_threads) * sizeof(index_t))))
    {
        printf("no soup for you\n");
        return (1);
    }

    new_img = ConstructImage(loaded_img);

    section_size = loaded_img->Header.width / nof_threads;

    start = time(NULL);

    for (i = 0; i < nof_threads; ++i)
    {
        index_arr[i].img1 = loaded_img;
        index_arr[i].img2 = new_img;
        index_arr[i].j = i * section_size;
        index_arr[i].end = MIN(loaded_img->Header.width, (i + 1) * section_size);

        pthread_create(&(index_arr[i].ptid), NULL,
                       &CopyAndApplyFilter, (index_arr + i));
    }

    for (i = 0; i < nof_threads; ++i)
    {
        pthread_join(index_arr[i].ptid, NULL);
    }

    StoreImage(new_img_ptr, new_img);

    end = time(NULL);

    DestructImage(loaded_img);
    DestructImage(new_img);
    fclose(img_ptr);
    fclose(new_img_ptr);

    /*CopyImage(src, dest);
    EmbossFilter(dest);
    EdgeFilter(src);
    SharpFilter(src);*/

    printf("start: %ld\n", start);
    printf("end: %ld\n", end);

    return 0;
}

/***************************** Utilty Functions ******************************/

void *CopyAndApplyFilter(void *param)
{
    index_t *index = (index_t *)param;
    unsigned int i = 0;
    int j = 0;

    for (i = 0; i < index->img1->Header.height; ++i)
    {
        for(j = index->j; j < index->end; ++j)
        {
            SetPixel(index->img2,
                     index->img2->Header.height - i - 1,
                     j,
                     GetPixel(index->img1, i, j));
        }
    }

    SetFilterParams((double *)emboss_filter, 3, 3, 1.0, 60.0);
    /*SetFilterParams((double *)edge_filter, 3, 3, 2.0, -20.0);
    SetFilterParams((double *)sharp_filter, 3, 3, 1.0, 0.0);*/

    for (i = 1; i < index->img2->Header.height -1; ++i)
    {
        for(j = index->j;j < index->end -1; ++j)
        {
            SetPixel(index->img2,
                     index->img2->Header.height - i - 1 ,
                     j,
                     ApplyFilter(index->img1, i, j + 1));
        }
    }

    return (NULL);
}


/*static void CopyImage(char *src, char *dest)
{
    for (y = 0; y < loaded_img->Header.height; ++y)
    {
        for (x = 0; x < loaded_img->Header.width; ++x)
        {
            SetPixel(new_img,
                     loaded_img->Header.height - y - 1,
                     x,
                     GetPixel(loaded_img, y, x));
        }
    }
}*/

/*---------------------------------------------------------------------------*/
#if(0)
static void EmbossFilter(char *src)
{
    FILE *img_ptr = fopen(src, "r");
    FILE *new_img_ptr = fopen("africa_embossed.bmp", "w+");
    BMP_IMAGE *loaded_img = NULL;
    BMP_IMAGE *new_img = NULL;
    size_t x = 0;
    size_t y = 0;

    if (NULL == img_ptr)
    {
        printf("no soup for you\n");
    }

    loaded_img = LoadImage(img_ptr);

    if (NULL == loaded_img)
    {
        printf("no soup for you\n");
    }

    new_img = ConstructImage(loaded_img);

    /* copying src to dest */
    for (y = 0; y < loaded_img->Header.height; ++y)
    {
        for (x = 0; x < loaded_img->Header.width; ++x)
        {
            SetPixel(new_img, loaded_img->Header.height - y - 1,
                     x, GetPixel(loaded_img, y, x));
        }
    }

    /* setting filter */
    SetFilterParams((double *)emboss_filter, 3, 3, 1.0, 60.0);

    /* applying filter */
    for (y = 1; y < loaded_img->Header.height - 1; ++y)
    {
        for (x = 1; x < loaded_img->Header.width - 1; ++x)
        {
            SetPixel(new_img, loaded_img->Header.height - y - 1,
                     x, ApplyFilter(loaded_img, y, x));
        }
    }

    /* storing dest, deconstructing src and closing file ptrs */
    StoreImage(new_img_ptr, new_img);
    DestructImage(loaded_img);
    DestructImage(new_img);
    fclose(img_ptr);
    fclose(new_img_ptr);
}

/*---------------------------------------------------------------------------*/

static void EdgeFilter(char *src)
{

    FILE *img_ptr = fopen(src, "r");
    FILE *new_img_ptr = fopen("tardis_edged.bmp", "w+");
    BMP_IMAGE *loaded_img = NULL;
    BMP_IMAGE *new_img = NULL;
    size_t x = 0;
    size_t y = 0;

    if (NULL == img_ptr)
    {
        printf("no soup for you\n");
    }

    loaded_img = LoadImage(img_ptr);

    if (NULL == loaded_img)
    {
        printf("no soup for you\n");
    }

    new_img = ConstructImage(loaded_img);

    /* copying src to dest */
    for (y = 0; y < loaded_img->Header.height; ++y)
    {
        for (x = 0; x < loaded_img->Header.width; ++x)
        {
            SetPixel(new_img, loaded_img->Header.height - y - 1,
                     x, GetPixel(loaded_img, y, x));
        }
    }

    /* setting filter */
    SetFilterParams((double *)edge_filter, 3, 3, 2.0, -20.0);

    /* applying filter */
    for (y = 1; y < loaded_img->Header.height - 1; ++y)
    {
        for (x = 1; x < loaded_img->Header.width -1; ++x)
        {
            SetPixel(new_img, loaded_img->Header.height - y - 1,
                     x, ApplyFilter(loaded_img, y, x));
        }
    }

    /* storing dest, deconstructing src and closing file ptrs */
    StoreImage(new_img_ptr, new_img);
    DestructImage(loaded_img);
    DestructImage(new_img);
    fclose(img_ptr);
    fclose(new_img_ptr);
}

/*---------------------------------------------------------------------------*/

static void SharpFilter(char *src)
{
    FILE *img_ptr = fopen(src, "r");
    FILE *new_img_ptr = fopen("tardis_sharpened.bmp", "w+");
    BMP_IMAGE *loaded_img = NULL;
    BMP_IMAGE *new_img = NULL;
    size_t x = 0;
    size_t y = 0;

    if (NULL == img_ptr)
    {
        printf("no soup for you\n");
    }

    loaded_img = LoadImage(img_ptr);

    if (NULL == loaded_img)
    {
        printf("no soup for you\n");
    }

    new_img = ConstructImage(loaded_img);

    /* copying src to dest */
    for (y = 0; y < loaded_img->Header.height; ++y)
    {
        for (x = 0; x < loaded_img->Header.width; ++x)
        {
            SetPixel(new_img, loaded_img->Header.height - y - 1,
                     x, GetPixel(loaded_img, y, x));
        }
    }

    /* setting filter */
    SetFilterParams((double *)sharp_filter, 3, 3, 1.0, 0.0);

    /* applying filter */
    for (y = 1; y < loaded_img->Header.height - 1; ++y)
    {
        for (x = 1; x < loaded_img->Header.width - 1; ++x)
        {
            SetPixel(new_img, loaded_img->Header.height - y - 1,
                     x, ApplyFilter(loaded_img, y, x));
        }
    }

    /* storing dest, deconstructing src and closing file ptrs */
    StoreImage(new_img_ptr, new_img);
    DestructImage(loaded_img);
    DestructImage(new_img);
    fclose(img_ptr);
    fclose(new_img_ptr);
}
#endif

/*****************************************************************************/