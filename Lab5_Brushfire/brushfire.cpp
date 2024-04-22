
// BRUSHFIRE for eyeSim

// Author Joel Smith + Sylvia Ackermann

#include eyebot++.h
#include image.h
#include vector
using stdvector;

typedef struct Pixel
{
    int x;
    int y;
    int id;
    int lastChanged;
    int value;
    bool voroni;
    vectorint allObjectId;

    Pixel() x(-1), y(-1), id(-1), lastChanged(-1), value(-1), voroni(false){};
} Pixel;

typedef struct Object
{
    int id;
    vectorPixel allPixels;

    void addPixeltoLine(Pixel pixel)
    {
        allPixels.push_back(pixel);
    }

} Object;

typedef struct Walls
{
    vectorPixel top;
    vectorPixel bottom;
    vectorPixel left;
    vectorPixel right;

    void addPixeltoTop(Pixel pixel)
    {
        top.push_back(pixel);
    }
    void addPixeltoBottom(Pixel pixel)
    {
        bottom.push_back(pixel);
    }
    void addPixeltoLeft(Pixel pixel)
    {
        left.push_back(pixel);
    }
    void addPixeltoRight(Pixel pixel)
    {
        right.push_back(pixel);
    }
    vectorvectorPixel getAll()
    {
        vectorvectorPixel all;
        all.push_back(top);
        all.push_back(bottom);
        all.push_back(left);
        all.push_back(right);
        return all;
    }
} Walls;

World Name const char fileName = u.pbm;
vectorint Colours;

Globals
    BYTE image;
vectorPixel allPixels;
vectorPixel voroniPoints;
vectorObject allObject;
int numberOfObjects = 0;

int WORLDSIZE = 4000;
int IMAGESIZE = 128;

vectorint getReletiveGoalLocation(int x, int y, int curX, int curY)
{
    int dx = x - curX;
    int dy = y - curY;
    int dist = sqrt(dx dx + dy dy);
    int rot = atan2(dy, dx) 180 M_PI;
    vectorint callBack;
    callBack.push_back(dist);
    callBack.push_back(rot);
    return callBack;
}

float convertPointsX(float x)
{
    return (4000(1 - x128));
}
float convertPointsY(float y)
{
    return (4000(y128));
}

void driveToPoint(vectorPixel points)
{
    int curX, curY, curAng;
    vectorPixel path;
    VWSetPosition(500, 3500, 0);
    VWGetPosition(&curX, &curY, &curAng);

    find closest veroni point
        Pixel pixel;
    int dist = 10000;

    for (int i = 0; i points.size(); i++)
    {
        if ((10 points.at(i).x && points.at(i).x 118) && (10 points.at(i).y && points.at(i).y 118))
        {
            float x = convertPointsX(points.at(i).x) - curY;
            float y = convertPointsY(points.at(i).y) - curX;
            int newDist = sqrt(xx + yy);

            if (newDist dist)
            {
                pixel = points.at(i);
                dist = newDist;
            }
        }
    }
    float x = convertPointsX(pixel.x) - curY;
    float y = convertPointsY(pixel.y) - curX;

    path.push_back(pixel);

    int currentPointX = pixel.x;
    int currentPointY = pixel.y;
    int shortestDist = 100000;
    int distToGoal = 100000;
    Pixel goal;
    goal.x = 112;
    goal.y = 112;

    while (shortestDist 17)
    {
        shortestDist = 100000;
        distToGoal = 100000;
        Pixel nextPixel;

        for (int i = 0; i points.size(); i++)
        {  for each
                point

                    float x = points.at(i).x;
            float y = points.at(i).y;

            if (abs(x - currentPointX) 10 && abs(y - currentPointY) 10)
            {
                check all points with in 10 Image points float distToGoal = sqrt((goal.x - x)(goal.x - x) + (goal.y - y)(goal.y - y));
                if (distToGoal shortestDist)
                {
                    shortestDist = distToGoal;
                    nextPixel = points.at(i);
                }
            }
        }
        currentPointX = nextPixel.x;
        currentPointY = nextPixel.y;
        path.push_back(nextPixel);
    }
    path.push_back(goal);
    LCDCircle(16, 16, 5, SILVER, 1);
    for (int j = 0; j path.size(); j++)
    {
        LCDCircle(path.at(j).y, path.at(j).x, 5, SILVER, 1);
        float x = convertPointsX(path.at(j).x);
        float y = convertPointsY(path.at(j).y);
        printf(Pixel(% i, % i) = (% f, % f) n, path.at(j).x, path.at(j).y, y, x);
    }
    printf(nn);
    for (int k = 0; k path.size(); k++)
    {
        printf(Current Position(% i, % i, % i) n, curX, curY, curAng);

        float x = convertPointsX(path.at(k).x) - curY;
        float y = convertPointsY(path.at(k).y) - curX;
        printf(Moving(% f, % f) n, y, x);

        int rot = atan2(x, y) 180M_PI - curAng;
        int dist = sqrt(xx + yy);

        printf(now turning % in, rot);
        while (abs(rot - curAng) 2)
        {
            VWSetSpeed(0, 20((rot - curAng) abs(rot - curAng)));
            VWGetPosition(&curX, &curY, &curAng);
        }
        VWSetSpeed(0, 0);
        VWTurn(rot, 100);
        VWWait();
        printf(now driving % in, dist);
        while (abs(curX - y) 50 && abs(curY - x) 50)
        {
            printf(curX - y = % i, curY - x = % in, curX - y, curY - x);
            VWSetSpeed(50, 0);
            VWGetPosition(&curX, &curY, &curAng);
        }
        VWStraight(dist, 100);
        VWWait();
        VWGetPosition(&curX, &curY, &curAng);
    }

    return;
}

void readImage()
{
    read_pbm(fileName, &image);
    same as last week but you might want to add walls aswell
}

void groupPixel()
{
    for (int i = 0; i 128; i++)
    {
        for (int j = 0; j 128; j++)
        {
            Pixel newPixel;
            newPixel.x = i;
            newPixel.y = j;
            if (image[i128 + j])
            {

                int hasNeighbours = -1;
                stores neighbour id if has neighbour

                // neighbours = connected elements with the current element's value if (j 0)
                {
                    if (image[i 128 + j - 1])
                    {
                        pixel to the left
                            hasNeighbours = allPixels.at(i 128 + j - 1).id;
                    }
                    if (i 0)
                    {
                        if (image[(i - 1)128 + j - 1])
                        {
                            pixel to the top and left
                                hasNeighbours = allPixels.at((i - 1)128 + j - 1).id;
                        }
                    }
                }

                if (i 0)
                {
                    if (image[(i - 1)128 + j])
                    {
                        pixel above
                            hasNeighbours = allPixels.at((i - 1)128 + j).id;
                    }
                }

                if (j 127 && i 0)
                {
                    if (image[(i - 1)128 + j + 1])
                    {
                        pixel to the top and right
                            hasNeighbours = allPixels.at((i - 1)128 + j + 1).id;
                    }
                }

                if (hasNeighbours == -1)
                {

                    Object newObject;
                    newPixel.id = numberOfObjects;
                    newObject.id = numberOfObjects;
                    newObject.allPixels.push_back(newPixel);
                    allObject.push_back(newObject);
                    numberOfObjects++;
                }
                else
                {

                    newPixel.id = hasNeighbours;
                    allObject.at(newPixel.id).allPixels.push_back(newPixel);
                }
            }
            allPixels.push_back(newPixel);
        }
    }
    for (int size = 0; size allObject.size(); size++)
    {
        printf(Object % i, size = % lu, id = % in, size, allObject.at(size).allPixels.size(), allObject.at(size).id);
    }

    for (int i = 0; i 128; i++)
    {
        for (int j = 0; j 128; j++)
        {

            if (image[i 128 + j])
            {

                if (image[i 128 + j + 1] && (allPixels.at(i 128 + j).id != allPixels.at(i 128 + j + 1).id))
                {

                    int newId = allPixels.at(i 128 + j).id;
                    int oldId = allPixels.at(i 128 + j + 1).id;

                    printf(% merging now % i to % in, oldId, newId);

                    for (int k = 0; k allObject.at(oldId).allPixels.size(); k++)
                    {

                        int x = allObject.at(oldId).allPixels.at(k).x;
                        int y = allObject.at(oldId).allPixels.at(k).y;

                        allObject.at(oldId).allPixels.at(k).id = newId;
                        allPixels.at(x128 + y).id = newId;
                        allObject.at(newId).allPixels.push_back(allObject.at(oldId).allPixels.at(k));
                    }

                    printf(erasing object % i with size % lun, oldId, allObject.at(oldId).allPixels.size());
                    allObject.erase(allObject.begin() + oldId);
                }
            }
        }
    }

    printf(nnFound % lu objects now printing nn, allObject.size());
    printf(nn);
    for (int size = 0; size allObject.size(); size++)
    {
        printf(Object % i, size = % lu, id = % in, size, allObject.at(size).allPixels.size(), allObject.at(size).id);
    }
    printf(nn);

    for (int i = 0; i allObject.size(); i++)
    {  for each
            object in allObjects for (int j = 0; j allObject.at(i).allPixels.size(); j++)
            {  for each
                    pixel in object

                        int x = allObject.at(i).allPixels.at(j).x;
                int y = allObject.at(i).allPixels.at(j).y;
                LCDArea(y, x, y + 1, x + 1, Colours.at(i), 1);
            }
    }
}

void brushFire()
{
    printf(please wait... n);
    bool changes = true;
    int lastChanged = 0;
    while (changes)
    {
        changes = false;
        for (int i = 0; i 128; i++)
        {
            for (int j = 0; j 128; j++)
            {
                if (allPixels.at(i128 + j).id != -1 && (lastChanged != allPixels.at(i128 + j).lastChanged))
                {

                    int newId = allPixels.at(i 128 + j).id;

                    if (j 0 && i 0)
                    {

                        if (allPixels.at((i - 1)128 + j - 1).id == -1)
                        {
                            changes = true;
                            allPixels.at((i - 1)128 + j - 1).id = newId;
                            allPixels.at((i - 1)128 + j - 1).lastChanged = lastChanged;
                        }
                        else if (allPixels.at(i128 + j).id != allPixels.at((i - 1)128 + j - 1).id)
                        {

                            allPixels.at(i128 + j).voroni = true;
                            allPixels.at((i - 1)128 + j - 1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i128 + j));
                            voroniPoints.push_back(allPixels.at((i - 1)128 + j - 1));
                            allPixels.at((i - 1)128 + j - 1).lastChanged = lastChanged;
                        }
                    }

                    if (j 0)
                    {

                        if (allPixels.at((i)128 + j - 1).id == -1)
                        {
                            changes = true;
                            allPixels.at((i)128 + j - 1).id = newId;
                            allPixels.at((i)128 + j - 1).lastChanged = lastChanged;
                        }
                        else if (allPixels.at(i128 + j).id != allPixels.at((i)128 + j - 1).id)
                        {

                            allPixels.at(i128 + j).voroni = true;
                            allPixels.at((i)128 + j - 1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i128 + j));
                            voroniPoints.push_back(allPixels.at((i)128 + j - 1));

                            allPixels.at((i)128 + j - 1).lastChanged = lastChanged;
                        }
                    }

                    if (j 0 && i 127)
                    {

                        if (allPixels.at((i + 1)128 + j - 1).id == -1)
                        {
                            changes = true;
                            allPixels.at((i + 1)128 + j - 1).id = newId;
                            allPixels.at((i + 1)128 + j - 1).lastChanged = lastChanged;
                        }
                        else if (allPixels.at(i128 + j).id != allPixels.at((i + 1)128 + j - 1).id)
                        {

                            allPixels.at(i128 + j).voroni = true;
                            allPixels.at((i + 1)128 + j - 1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i128 + j));
                            voroniPoints.push_back(allPixels.at((i + 1)128 + j - 1));
                            allPixels.at((i + 1)128 + j - 1).lastChanged = lastChanged;
                        }
                    }

                    if (i 0)
                    {

                        if (allPixels.at((i - 1)128 + j).id == -1)
                        {
                            allPixels.at((i - 1)128 + j).id = newId;
                            changes = true;
                            allPixels.at((i - 1)128 + j).lastChanged = lastChanged;
                        }
                        else if (allPixels.at(i128 + j).id != allPixels.at((i - 1)128 + j).id)
                        {

                            allPixels.at(i128 + j).voroni = true;
                            allPixels.at((i - 1)128 + j).voroni = true;
                            voroniPoints.push_back(allPixels.at(i128 + j));
                            voroniPoints.push_back(allPixels.at((i - 1)128 + j));
                            allPixels.at((i - 1)128 + j).lastChanged = lastChanged;
                        }
                    }

                    if (i 127)
                    {

                        if (allPixels.at((i + 1)128 + j).id == -1)
                        {
                            changes = true;
                            allPixels.at((i + 1)128 + j).id = newId;
                            allPixels.at((i + 1)128 + j).lastChanged = lastChanged;
                        }
                        else if (allPixels.at(i128 + j).id != allPixels.at((i + 1)128 + j).id)
                        {

                            allPixels.at(i128 + j).voroni = true;
                            allPixels.at((i + 1)128 + j).voroni = true;
                            voroniPoints.push_back(allPixels.at(i128 + j));
                            voroniPoints.push_back(allPixels.at((i + 1)128 + j));
                            allPixels.at((i + 1)128 + j).lastChanged = lastChanged;
                        }
                    }

                    if (j 127 && i 0)
                    {

                        if (allPixels.at((i - 1)128 + j + 1).id == -1)
                        {
                            changes = true;
                            allPixels.at((i - 1)128 + j + 1).id = newId;
                            allPixels.at((i - 1)128 + j + 1).lastChanged = lastChanged;
                        }
                        else if (allPixels.at(i128 + j).id != allPixels.at((i - 1)128 + j + 1).id)
                        {

                            allPixels.at(i128 + j).voroni = true;
                            allPixels.at((i - 1)128 + j + 1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i128 + j));
                            voroniPoints.push_back(allPixels.at((i - 1)128 + j + 1));
                            allPixels.at((i - 1)128 + j + 1).lastChanged = lastChanged;
                        }
                    }

                    if (j 127)
                    {

                        if (allPixels.at((i)128 + j + 1).id == -1)
                        {
                            changes = true;
                            allPixels.at((i)128 + j + 1).id = newId;
                            allPixels.at((i)128 + j + 1).lastChanged = lastChanged;
                        }
                        else if (allPixels.at(i128 + j).id != allPixels.at((i)128 + j + 1).id)
                        {

                            allPixels.at(i128 + j).voroni = true;
                            allPixels.at((i)128 + j + 1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i128 + j));
                            voroniPoints.push_back(allPixels.at((i)128 + j + 1));
                            allPixels.at((i)128 + j + 1).lastChanged = lastChanged;
                        }
                    }

                    if (j 127 && i 127)
                    {

                        if (allPixels.at((i + 1)128 + j + 1).id == -1)
                        {
                            changes = true;
                            allPixels.at((i + 1)128 + j + 1).id = newId;
                            allPixels.at((i + 1)128 + j + 1).lastChanged = lastChanged;
                        }
                        else if (allPixels.at(i128 + j).id != allPixels.at((i + 1)128 + j + 1).id)
                        {

                            allPixels.at(i128 + j).voroni = true;
                            allPixels.at((i + 1)128 + j + 1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i128 + j));
                            voroniPoints.push_back(allPixels.at((i + 1)128 + j + 1));
                            allPixels.at((i + 1)128 + j + 1).lastChanged = lastChanged;
                        }
                    }
                }
            }
        }
        lastChanged++;
        for (int x = 0; x 128; x++)
        {
            for (int y = 0; y 128; y++)
            {
                int id = allPixels.at(x128 + y).id;
                if (allPixels.at(x128 + y).voroni)
                {
                    if (id != -1)
                    {
                        LCDArea(y, x, y + 1, x + 1, Colours.at(6), 1);
                    }
                }
                else
                {
                    if (id != -1)
                    {
                        LCDArea(y, x, y + 1, x + 1, Colours.at(id), 1);
                    }
                }
            }
        }
    }

    printf(nn Finished Burning nn);
}

int main()
{
    Colours.push_back(RED);
    Colours.push_back(GREEN);
    Colours.push_back(CYAN);
    Colours.push_back(MAGENTA);
    Colours.push_back(PURPLE);
    Colours.push_back(DARKGRAY);
    Colours.push_back(BLACK);
    Colours.push_back(SILVER);
    Colours.push_back(ORANGE);
    Colours.push_back(GREEN);

    SIMSetRobot(1, 500, 3500, 0, 1);

    readImage();
    LCDImageStart(0, 0, IMAGESIZE, IMAGESIZE);
    LCDImageBinary(image);
    Display image and objects with unique colours
        display voroni points and lines

            required path = path

                drive to goal,
                     following the path

                         LCDMenu(FIND, FIRE, DRIVE, EXIT);
    int endSim = 0;
    boolean to end sim 0 = false, 1 = true

                                  do
    {
        switch (KEYRead())
        {
        case KEY1
            printf(nn            Grouping nn);
            groupPixel();
            break;
        case KEY2
            printf(nn            Burning nn);
            brushFire();
            break;
        case KEY3
            printf(nn            Driving nn);
            driveToPoint(voroniPoints);
            break;
        case KEY4
        VWSetSpeed(0, 0);
            endSim = 1;
            break;
        }
    } while (!endSim);

}
