#ifndef COLORS_H

#define COLORS_H

//Call printf(COLOR) to change following output to that color
//Call printf(RESET) when finshed printing to reset color

//Custom
#define ROCK_GRY   "\x1B[7;40m"

//Colors - No attributes
#define BLK   "\x1B[30m"
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"

//Colors - Bold
#define Bo_BLK   "\x1B[1;30m"
#define Bo_RED   "\x1B[1;31m"
#define Bo_GRN   "\x1B[1;32m"
#define Bo_YEL   "\x1B[1;33m"
#define Bo_BLU   "\x1B[1;34m"
#define Bo_MAG   "\x1B[1;35m"
#define Bo_CYN   "\x1B[1;36m"
#define Bo_WHT   "\x1B[1;37m"

//Colors - Dim
#define D_BLK   "\x1B[2;30m"
#define D_RED   "\x1B[2;31m"
#define D_GRN   "\x1B[2;32m"
#define D_YEL   "\x1B[2;33m"
#define D_BLU   "\x1B[2;34m"
#define D_MAG   "\x1B[2;35m"
#define D_CYN   "\x1B[2;36m"
#define D_WHT   "\x1B[2;37m"

//Colors - ???
#define Bl_BLK   "\x1B[3;30m"
#define Bl_RED   "\x1B[3;31m"
#define Bl_GRN   "\x1B[3;32m"
#define Bl_YEL   "\x1B[3;33m"
#define Bl_BLU   "\x1B[3;34m"
#define Bl_MAG   "\x1B[3;35m"
#define Bl_CYN   "\x1B[3;36m"
#define Bl_WHT   "\x1B[3;37m"

//Colors - Underline
#define U_BLK   "\x1B[4;30m"
#define U_RED   "\x1B[4;31m"
#define U_GRN   "\x1B[4;32m"
#define U_YEL   "\x1B[4;33m"
#define U_BLU   "\x1B[4;34m"
#define U_MAG   "\x1B[4;35m"
#define U_CYN   "\x1B[4;36m"
#define U_WHT   "\x1B[4;37m"

//Colors - Reverse
#define R_BLK   "\x1B[7;30m"
#define R_RED   "\x1B[7;31m"
#define R_GRN   "\x1B[7;32m"
#define R_YEL   "\x1B[7;33m"
#define R_BLU   "\x1B[7;34m"
#define R_MAG   "\x1B[7;35m"
#define R_CYN   "\x1B[7;36m"
#define R_WHT   "\x1B[7;37m"

//Reset
#define RESET "\x1B[0m"

#endif
