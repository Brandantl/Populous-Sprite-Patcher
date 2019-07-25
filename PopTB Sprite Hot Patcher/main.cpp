/*
    Populous The Beginning Sprite Patcher
    Brandan Tyler Lasley 2019 (7/25/2019)
*/
#include "Sprite.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        std::cout << argv[1] << " <---- " << argv[2];
    }
    else
    {
        std::cout << "Usage: <destnation> <source> \n";
        return -1;
    }

    std::string dest = argv[1];
    std::string source = argv[2];

    auto spr = Sprite(source);


    // Patch Image
    signed char     byte;
    unsigned char   clr;
    unsigned char   rclr;
    signed char *   curr;
    int             num, j, y, x;

    auto hot_patch_spr_image = [&byte, &clr, &rclr, &curr, &num, &j, &y, &x](TbSprite & spr)
    {
        curr = spr.data;
        y = 0;
        x = 0;
        while (y < spr.header->height)
        {
            byte = (*curr);
            curr++;
            if (byte == 0)
            {
                x = 0;
                y++;
            }
            else if (byte >= 127)
            {
                x = x + (256 - byte);
            }
            else
            {
                num = byte;
                for (j = 1; j <= num; j++)
                {
                    byte = *curr;
                    clr = *curr;
                    rclr = 0;

                    // BLUE
                    if (clr >= 216 && clr <= 223)
                    {
                        rclr = clr - 76;
                        (*curr) = rclr;
                    }

                    // RED
                    if (clr >= 240 && clr <= 247)
                    {
                        rclr = clr - 32;
                        (*curr) = rclr;
                    }

                    // YELLOW
                    if (clr >= 232 && clr <= 239)
                    {
                        rclr = clr - 72;
                        (*curr) = rclr;
                    }

                    // GREEN
                    if (clr >= 224 && clr <= 231)
                    {
                        rclr = clr - 92;
                        (*curr) = rclr;
                    }

                    curr++;
                    x++;
                }
            }
        }
        return true;
    };

    spr.processSprites(hot_patch_spr_image);
    spr.savespr(dest);

    return 0;
}

