#pragma once

int identity[9] =   {   
                        0, 0, 0, 
                        0, 1, 0, 
                        0, 0, 0 
                    };

int sobelX[9] =   {   
                        1, 0, -1, 
                        2, 0, -2, 
                        1, 0, -1 
                    };

int sobelY[9] =   {   
                        1,   2,  1, 
                        0,   0,  0, 
                       -1,  -2, -1 
                    };

int laplace[9] =   {
                        0,   1,  0, 
                        1,   -4,  1, 
                       0,  1, 0 
                    };


int prewittX[9] =   {
                        -1,   0,  1, 
                        -1,   0,  1, 
                        -1,   0,  1 
                    };
int prewittY[10] =   {
                        -1,   -1,  -1, 
                        0,   0,  0, 
                        1,   1,  1 
                    };
