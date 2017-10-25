#ifndef MAP_H
#define MAP_H

//#define PACMAN_MAP [15][19] = {\
//    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},\
//    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
//    {1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},\
//    {1,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1},\
//    {1,0,1,0,1,0,0,0,1,1,1,0,1,0,1,1,1,0,1},\
//    {1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1},\
//    {1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1},\
//    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},\
//    {1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1},\
//    {1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1},\
//    {1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1},\
//    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},\
//    {1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1},\
//    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
//    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},\
//}
    
#define PACMAN_MAP [15][19] = {\
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},\
{1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,1},\
{1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1},\
{1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,1},\
{1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1},\
{1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},\
{1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1},\
{1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1},\
{1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1},\
{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1},\
{1,0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,0,1},\
{1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},\
{1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},\
{1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1},\
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},\
}
    
//#define PACMAN_MAP [15][19] = {\
// {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},\
// {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},\
// {1,1,1,1,0,0,0,1,1,0,1,1,0,1,0,1,1,0,1},\
// {1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,1,1,0,1},\
// {1,1,1,1,0,0,0,0,1,1,1,0,1,1,0,0,0,0,1},\
// {1,0,0,0,0,1,0,1,1,0,0,0,1,0,0,1,0,1,1},\
// {1,1,0,1,1,1,0,0,0,0,1,0,0,0,1,1,0,1,1},\
// {1,0,0,0,0,1,0,1,1,1,1,0,1,0,1,0,1,0,1},\
// {1,0,1,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,1},\
// {1,0,0,1,1,1,1,0,1,0,1,0,1,0,0,0,0,0,1},\
// {1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,1,1,1,1},\
// {1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,1},\
// {1,0,1,1,0,1,1,0,1,1,1,0,1,0,0,0,1,0,1},\
// {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},\
// {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}\
//}
#define PACMAN_MAP_WIDTH 19
#define PACMAN_MAP_HEIGHT 15

// Format {x, y}
#define PACMAN_FOOD_LIST [5][2] = {{7,4},\
    {16,11},\
    {13,12},\
    {17,5},\
    {3,9}\
}
#define PACMAN_FOOD_LIST_HEIGHT 5
    
//#define PACMAN_START_X 13
//#define PACMAN_START_Y 5
#define PACMAN_START_X 1
#define PACMAN_START_Y 7

#define PACMAN_START_ORIENTATION G_W

#endif /* MAP_H */
