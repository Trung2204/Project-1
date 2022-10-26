#include<stdio.h>
#include<stdlib.h>

//Read images library
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
//Write images library
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

/**
 * Create a new 1-dimensional array with the given size
 * @param[in] _size the size of the array
 * @param[out] _ empty 1-dimensional array filled with 0
 */
unsigned char *uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

unsigned char* image_mask(unsigned char *foreground, int width, int height, int channel)
{
    unsigned char *temp_array = uc_arrayNew_1d(width * height * channel);
    int i,j,k;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (foreground[(i * width + j) * channel + 1]*1.5 > foreground[(i * width + j) * channel] + foreground[(i * width + j) * channel + 2])
            {
                for (k = 0; k < channel; k++)
                {
                    temp_array[(i * width + j) * channel + k] = 0;
                }
            }
            else
            {
                for (k = 0; k < channel; k++)
                {
                    temp_array[(i * width + j) * channel + k] = foreground[(i * width + j) * channel + k];
                }
            }
        }
    }
    return temp_array;
}

unsigned char* image_combination(unsigned char *masked_image, unsigned char *weather_forecast, int width, int height, int channel)
{
    unsigned char *temp_array = uc_arrayNew_1d(width * height * channel);
    int i,j,k;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            for (k = 0; k < channel; k++)
            {
                if(masked_image[(i * width + j) * channel + k] == 0)
                    temp_array[(i * width + j) * channel + k] = weather_forecast[(i * width + j) * channel + k];
                else
                    temp_array[(i * width + j) * channel + k] = masked_image[(i * width + j) * channel + k];
            }
        }
    }
    return temp_array;
}

int main()
{
    // Declare variable
    int width, height, channel;
    char path_foreground [] = "./images/foreground.jpg";
    char path_weather_forecast [] = "./images/weather_forecast.jpg";
    char save_path [] = "./images/Result.jpg";

    // Read image data
    unsigned char * foreground = stbi_load (path_foreground, &width, &height, &channel, 0);
    if (foreground == NULL )
    {
        printf("\nError in loading the foreground image.\n");
        exit(1);
    }
    printf("Foreground image:\nWidth = %d\nHeight = %d\nChannel = %d\n", width, height, channel);

    unsigned char * weather_forecast = stbi_load (path_weather_forecast, &width, &height, &channel, 0);
    if (weather_forecast == NULL )
    {
        printf("\nError in loading the observed image\n");
        exit(1);
    }
    printf("\nWeather forecast image:\nWidth = %d\nHeight = %d\nChannel = %d\n", width, height, channel);

    /* ~ ~ ~ Process ~ ~ ~ */
    // Mask foreground image
    unsigned char* masked_image = image_mask(foreground, width, height, channel);
    // Combine masked image with background
    unsigned char* result = image_combination(masked_image, weather_forecast, width, height, channel);

    // Save image
    stbi_write_jpg(save_path, width, height, channel, result, width * channel);
    printf("Result image saved to %s\n", save_path);

    // Free allocated memory
    stbi_image_free(masked_image);
    stbi_image_free(result);

    return 0;
}