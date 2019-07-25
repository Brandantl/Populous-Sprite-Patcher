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
    int8_t          byte, *curr;
    unsigned char   clr, rclr;
    int             num, j, y, x;

    auto hot_patch_spr_image = [&byte, &clr, &rclr, &curr, &num, &j, &y, &x](TbSprite & spr)
    {
        // Most of this code is just to scan the sprites X, Y locations and checking the byte
        // if its the color we want to replace. If it is we replace it in the j = 1 loop below.

        // This is also how you can draw sprites.
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
                    if (clr >= 216 && clr <= 223) // Blues pal color codes
                    {
                        rclr = clr - 76;
                        (*curr) = rclr; // Change pal color to this new pal color.
                    }

                    // RED
                    if (clr >= 240 && clr <= 247) // Reds pal color codes
                    {
                        rclr = clr - 32;
                        (*curr) = rclr; // Change pal color to this new pal color.
                    }

                    // YELLOW
                    if (clr >= 232 && clr <= 239) // Yellows pal color codes
                    {
                        rclr = clr - 72;
                        (*curr) = rclr; // Change pal color to this new pal color.
                    }

                    // GREEN 
                    if (clr >= 224 && clr <= 231) // Greens pal color codes
                    {
                        rclr = clr - 92;
                        (*curr) = rclr; // Change pal color to this new pal color.
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

