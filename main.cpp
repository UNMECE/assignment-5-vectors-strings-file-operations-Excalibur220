#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "pixel.h"

using namespace std;

Pixel parse_line(const string &line)
{
    Pixel p;

    size_t start = 0;
    size_t comma = line.find(',', start);
    string sx = line.substr(start, comma - start);

    start = comma + 1;
    comma = line.find(',', start);
    string sy = line.substr(start, comma - start);

    start = comma + 1;
    comma = line.find(',', start);
    string sr = line.substr(start, comma - start);

    start = comma + 1;
    comma = line.find(',', start);
    string sg = line.substr(start, comma - start);

    start = comma + 1;
    string sb = line.substr(start);

    p.x = stoi(sx);
    p.y = stoi(sy);
    p.r = stof(sr);
    p.g = stof(sg);
    p.b = stof(sb);

    return p;
}

void average_colors(vector<Pixel> &pixel_list)
{
    double sum_r = 0.0;
    double sum_g = 0.0;
    double sum_b = 0.0;

    for (size_t i = 0; i < pixel_list.size(); i++)
    {
        sum_r += pixel_list[i].r;
        sum_g += pixel_list[i].g;
        sum_b += pixel_list[i].b;
    }

    double n = (double)pixel_list.size();
    if (n == 0)
    {
        cout << "No pixels loaded.\n";
        return;
    }

    cout << "average r = " << (sum_r / n) << "\n";
    cout << "average g = " << (sum_g / n) << "\n";
    cout << "average b = " << (sum_b / n) << "\n";
}

void flip_vertically(vector<Pixel> &pixel_list)
{
    int max_y = 0;
    for (size_t i = 0; i < pixel_list.size(); i++)
    {
        if (pixel_list[i].y > max_y)
        {
            max_y = pixel_list[i].y;
        }
    }

    for (size_t i = 0; i < pixel_list.size(); i++)
    {
        pixel_list[i].y = max_y - pixel_list[i].y;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Run like: ./main pixels.dat\n";
        return 1;
    }

    string filename = argv[1];
    ifstream fin(filename);

    if (!fin.is_open())
    {
        cout << "Could not open file: " << filename << "\n";
        return 1;
    }

    vector<Pixel> pixel_list;

    string line;
    while (getline(fin, line))
    {
        if (line.size() == 0)
        {
            continue;
        }

        Pixel p = parse_line(line);
        pixel_list.push_back(p);
    }

    fin.close();

    cout << "pixels loaded = " << pixel_list.size() << "\n";

    average_colors(pixel_list);
    flip_vertically(pixel_list);

    ofstream fout("flipped.dat");
    if (!fout.is_open())
    {
        cout << "Could not create flipped.dat\n";
        return 1;
    }

    for (size_t i = 0; i < pixel_list.size(); i++)
    {
        fout << pixel_list[i].x << ","
             << pixel_list[i].y << ","
             << pixel_list[i].r << ","
             << pixel_list[i].g << ","
             << pixel_list[i].b << "\n";
    }

    fout.close();

    cout << "Wrote flipped.dat\n";
    return 0;
}