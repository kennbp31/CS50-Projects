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
     int temp_r;
     int temp_g;
     int temp_b;
     int temp_r1;
     int temp_g1;
     int temp_b1;
    

    for (int i = 0; i < height; i++)
    {
        for (int k = 0, l = width - 1; k < width / 2; k++, l--)
        {
            temp_r = image[i][k].rgbtRed;
            temp_g = image[i][k].rgbtGreen;
            temp_b = image[i][k].rgbtBlue;
            
            temp_r1 = image[i][l].rgbtRed;
            temp_g1 = image[i][l].rgbtGreen;
            temp_b1 = image[i][l].rgbtBlue;

            image[i][k].rgbtRed = temp_r1;
            image[i][k].rgbtGreen = temp_g1;
            image[i][k].rgbtBlue = temp_b1;

            image[i][l].rgbtRed = temp_r;
            image[i][l].rgbtGreen = temp_g;
            image[i][l].rgbtBlue = temp_b;
        }

    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int sum_r, sum_g, sum_b, count;
    count = 0;
    for (int i = 2; i < height; i = i + 3)
    {       
            
        for (int j = 2; j < width - 1; j = j + 3)
        {
            count = 0;
            sum_r = 0;
            sum_g = 0;
            sum_b = 0;
            
            for (int k = i - 1; k < i + 2 || k == height; k++)
            {
                if (k >= 0)
                {
                    for (int l = j - 1; l < j + 2 || l == width - 1; l++)
                    {
                        if (l >= 0)
                        {
                            sum_r = sum_r + image[k][l].rgbtRed;
                            sum_g = sum_g + image[k][l].rgbtGreen;
                            sum_b = sum_b + image[k][l].rgbtBlue;
                            count++;
                        }
                        
                        
                    }
                }    
                
  
            }
            image[i][j].rgbtRed = round((float)sum_r / count);
            image[i][j].rgbtGreen = round((float)sum_g / count);
            image[i][j].rgbtBlue = round((float)sum_b / count);      
        }
    }
    return;
}
