/********************************
* Task name: image processing 	*
* Develoer: Inbal Elmalech		*
* Reviewer: Guy Barazany   		*
* Date: Aug 16, 2020			*
********************************/

/**********************   PREPROCESSOR DIRECTIVES   **************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "photoutils.h"

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
    /*pthread_t ptid;*/
    BMP_IMAGE *loaded_img;
    BMP_IMAGE *new_img;
    /*int j;
    int end;*/
}index_t;

/****************************   DECLARATIONS   *******************************/

void CopyAndFilter(void *param);
/*static void CopyImage(char *src, char *dest);
static void EmbossFilter(char *src);
static void EdgeFilter(char *src);
static void SharpFilter(char *src);*/

/********************************  MAIN  *************************************/

int main(int argc, char const **argv)
{
    char src[] = "tardis.bmp";
    char dest[] = "tardis_copy.bmp";
    index_t *pic_section = NULL;
    FILE *img_ptr = fopen(src, "r");
    FILE *new_img_ptr = fopen(dest, "w+");
    BMP_IMAGE *loaded_img = NULL;
    BMP_IMAGE *new_img = NULL;

    if ((NULL == img_ptr) || (NULL == (loaded_img = LoadImage(img_ptr))))
    {
        puts("no soup for you");
    }

    if (NULL == (pic_section = (index_t *)malloc(sizeof(index_t))))
    {
        puts("no soup for you");
    }

    new_img = ConstructImage(loaded_img);

    pic_section->loaded_img = loaded_img;
    pic_section->new_img = new_img;

    CopyAndFilter((void *)pic_section);

    StoreImage(new_img_ptr, new_img);
    DestructImage(loaded_img);
    DestructImage(new_img);
    fclose(img_ptr);
    fclose(new_img_ptr);

    return 0;
}

/***************************** Utilty Functions ******************************/

void CopyAndFilter(void *param)
{
    index_t *section = (index_t *)param;
    size_t y = 0;
    size_t x = 0;

    for (y = 0; y < section->loaded_img->Header.height; ++y)
    {
        for (x = 0; x < section->loaded_img->Header.width; ++x)
        {
            SetPixel(section->new_img,
                     section->new_img->Header.height - y - 1,
                     x,
                     GetPixel(section->loaded_img, y, x));
        }
    }

    /*for (y = 0; y < section->loaded_img->Header.height; ++y)
    {
        for (x = 0; x < section->loaded_img->Header.width; ++x)
        {
            SetPixel(section->new_img,
                     section->new_img->Header.height - y - 1,
                     x,
                     GetPixel(section->loaded_img, y, x));
        }
    }*/
}

/*---------------------------------------------------------------------------*/
#if(0)
static void EmbossFilter(char *src)
{
    FILE *img_ptr = fopen(src, "r");
    FILE *new_img_ptr = fopen("tardis_embossed.bmp", "w+");
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