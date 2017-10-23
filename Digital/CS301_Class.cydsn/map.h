#ifndef MAP_H
#define MAP_H

#define PACMAN_MAP [15][19] = {\
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},\
    {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
    {1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},\
    {1,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1},\
    {1,0,1,0,1,0,0,0,1,1,1,0,1,0,1,1,1,0,1},\
    {1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1},\
    {1,1,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1},\
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},\
    {1,1,1,0,1,0,1,0,1,1,1,0,1,0,1,0,1,1,1},\
    {1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1},\
    {1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1},\
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1},\
    {1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1},\
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},\
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
#define PACMAN_FOOD_LIST [5][2] = {{3,1},\
    {5,5},\
    {3,5},\
    {1,3},\
    {6,4}\
}
#define PACMAN_FOOD_LIST_HEIGHT 0
    
//#define PACMAN_START_X 13
//#define PACMAN_START_Y 5
#define PACMAN_START_X 17
#define PACMAN_START_Y 13

#define PACMAN_START_ORIENTATION G_S

#endif /* MAP_H */
