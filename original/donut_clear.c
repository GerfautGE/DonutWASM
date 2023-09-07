#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "Constant.h"

int main(void)
{
    // initial position
    float vertical_rotation = 0; // radians
    float horizontal_rotation = M_PI_2;

    // initial screen
    float distance_from_camera[HEIGHT * WIDTH]; // distance from camera
    char screen_buffer[HEIGHT * WIDTH];         // screen buffer

    printf("\x1b[2J"); // clear the screen and move cursor to top left

    for (;;) // always run
    {
        memset(screen_buffer, 32, HEIGHT * WIDTH);                       // clear the screen buffer
        memset(distance_from_camera, 0, HEIGHT * WIDTH * sizeof(float)); // clear the z buffer

        for (float alpha = 0; 2 * M_PI > alpha; alpha += 0.07) // alpha \in [0, 2*PI]
        {
            for (float beta = 0; 2 * M_PI > beta; beta += 0.02) // beta \in [0, 2*PI]
            {
                // calculate cos and sin of alpha, beta, vertical_rotation, horizontal_rotation
                float sin_i = sin(beta), cos_j = cos(alpha), sin_A = sin(vertical_rotation), sin_j = sin(alpha), cos_A = cos(vertical_rotation), cos_i = cos(beta), cos_B = cos(horizontal_rotation), sin_B = sin(horizontal_rotation);

                // coordinates of the point on the torus
                float x = R2 * cos(alpha) + R1; // abscissa of the point on the torus

                float z_inverse = 1 / (sin(beta) * x * sin(vertical_rotation) + R2 * sin(alpha) * cos(vertical_rotation) + DISTANCE);
                float t = sin(beta) * x * cos(vertical_rotation) - R2 * sin(alpha) * sin(vertical_rotation);

                // calculate x0, y0
                int x0 = WIDTH / 2 + 2 * ZOOM * z_inverse * (cos(beta) * x * cos(horizontal_rotation) - t * sin(horizontal_rotation));
                int y0 = HEIGHT / 2 + ZOOM * z_inverse * (cos(beta) * x * sin(horizontal_rotation) + t * cos(horizontal_rotation));

                // find which character corresponds to the point in the screen buffer
                int o = x0 + WIDTH * y0;

                // calculate the illumination
                int light_level = BRIGHTNESS * ((sin_j * sin_A - sin_i * cos_j * cos_A) * cos_B - sin_i * cos_j * sin_A - sin_j * cos_A - cos_i * cos_j * sin_B);

                if (HEIGHT > y0 && y0 > 0 && x0 > 0 && WIDTH > x0 && z_inverse > distance_from_camera[o])
                // assess the point (x, y) is in the sreen and is closer than the previous point from the camera
                {
                    distance_from_camera[o] = z_inverse;
                    screen_buffer[o] = illum[light_level > 0 ? light_level : 0];
                }
            }
        }
        printf("\x1b[H"); // move cursor to top left
        for (int index = 0; index <= HEIGHT * WIDTH; index++)
        {
            putchar(index % WIDTH ? screen_buffer[index] : 10); // print the screen buffer : 10 is the ASCII code for \n
        }
        vertical_rotation += VROT;
        horizontal_rotation += HROT;
        nanosleep((const struct timespec[]){{0, 50000000L}}, NULL); // sleep for 50ms
    }
}
