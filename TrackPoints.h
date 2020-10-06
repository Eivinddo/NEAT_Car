#pragma once

/*
This file contains hardcoded points used to design a simple track.
The track is made using Paint and measuring the pixels on the canvas.
Should make a track editor to test cars in different tracks.
*/

std::pair<int, int> leftBoundary[15] = {
    { 120, 720},
    { 120, 409},
    { 220, 167},
    { 473,  56},
    { 670,  56},
    { 779, 153},
    { 779, 332},
    { 598, 424},
    { 550, 485},
    { 550, 555},
    { 691, 594},
    { 880, 554},
    {1000, 472},
    {1072, 168},
    {1072,   0}
};

std::pair<int, int> rightBoundary[15] = {
    { 200, 720},
    { 200, 422},
    { 276, 233},
    { 476, 153},
    { 615, 153},
    { 650, 209},
    { 650, 276},
    { 440, 370},
    { 371, 541},
    { 458, 677},
    { 691, 701},
    { 908, 680},
    {1136, 552},
    {1215, 276},
    {1215,   0}
};