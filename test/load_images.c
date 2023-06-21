#include "image.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    if(argc <= 1)
    {
        return EXIT_FAILURE;
    }
    ImageFile *secretImage;
    ImageFile **carriers;
    unsigned carrierCount;
    loadImages("secret.bmp", "images/", 6, &secretImage, &carriers, &carrierCount);

    return EXIT_SUCCESS;
}
