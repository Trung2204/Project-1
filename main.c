#include<stdio.h>
#include<math.h>
#include<stdlib.h>

//Read images library
#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
//Write images library
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

unsigned char image_mask(unsigned char *foreground, int width, int height, int channel)
{
    int i,j,k;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (foreground[(i * width + j) * channel] >= 50 && foreground[(i * width + j) * channel + 1] >= 200 &&
                foreground[(i * width + j) * channel + 2] >= 5 && foreground[(i * width + j) * channel + 1]*1.5 > foreground[(i * width + j) * channel] + foreground[(i * width + j) * channel + 2])
            {
                for (k = 0; k < channel; k++)
                {
                    foreground[(i * width + j) * channel + k] = 0;
                }
            }
        }
    }
}

unsigned char image_combination(unsigned char *foreground, unsigned char *weather_forecast, int width, int height, int channel)
{
    int i,j,k;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            for (k = 0; k < channel; k++)
            {
                if(foreground[(i * width + j) * channel + k] == 0)
                    foreground[(i * width + j) * channel + k] = weather_forecast[(i * width + j) * channel + k];
            }
        }
    }
}

int main()
{
    // Declare variable
    int width, height, channel;
    char path_foreground [] = "./images/foreground.jpg";
    char path_background [] = "./images/background.jpg";
    char path_weather_forecast [] = "./images/weather_forecast.jpg";
    char save_path [] = "./images/Result.png";

    // Read image data
    unsigned char * foreground = stbi_load (path_foreground, &width, &height, &channel, 0);
    if (foreground == NULL )
    {
        printf("\nError in loading the foreground image.\n");
        exit(1);
    }
    printf("Foreground image:\nWidth = %d\nHeight = %d\nChannel = %d\n", width, height, channel);
    unsigned char * background = stbi_load (path_background, &width, &height, &channel, 0);
    if (background == NULL )
    {
        printf("\nError in loading the background image.\n");
        exit(1);
    }
    printf("\nBackground image:\nWidth = %d\nHeight = %d\nChannel = %d\n", width, height, channel);
    unsigned char * weather_forecast = stbi_load (path_weather_forecast, &width, &height, &channel, 0);
    if (weather_forecast == NULL )
    {
        printf("\nError in loading the observed image\n");
        exit(1);
    }
    printf("\nWeather forecast image:\nWidth = %d\nHeight = %d\nChannel = %d\n", width, height, channel);

    /* ~ ~ ~ Process ~ ~ ~ */
    
    image_mask(foreground, width, height, channel);
    image_combination(foreground, weather_forecast, width, height, channel);

    // Save image
    stbi_write_png(save_path, width, height, channel, foreground, width * channel);
    printf("New image saved to %s\n", save_path);

    return 0;
}