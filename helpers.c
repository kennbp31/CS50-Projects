#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, temp_c; j < width; j++)
        {
            temp_c = round(((float)image[i][j].rgbtRed + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtBlue) / 3);
            image[i][j].rgbtRed = temp_c;
            image[i][j].rgbtGreen = temp_c;
            image[i][j].rgbtBlue = temp_c;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, temp_r, temp_b, temp_g; j < width; j++)
        {
            temp_r = round(.393 * (float)image[i][j].rgbtRed + .769 * (float)image[i][j].rgbtGreen + .189 * (float)image[i][j].rgbtBlue);
            temp_g = round(.349 * (float)image[i][j].rgbtRed + .686 * (float)image[i][j].rgbtGreen + .168 * (float)image[i][j].rgbtBlue);
            temp_b = round(.272 * (float)image[i][j].rgbtRed + .534 * (float)image[i][j].rgbtGreen + .131 * (float)image[i][j].rgbtBlue);

            if (temp_r > 255)
            {
                image[i][j].rgbtRed = 255;
            }

            else
            {
                image[i][j].rgbtRed = temp_r;
            }

            if (temp_g > 255)
            {
                image[i][j].rgbtGreen = 255;
            }

            else
            {
                image[i][j].rgbtGreen = temp_g;
            }

            if (temp_b > 255)
            {
                image[i][j].rgbtBlue = 255;
            }

            else
            {
                image[i][j].rgbtBlue = temp_b;
            }

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
