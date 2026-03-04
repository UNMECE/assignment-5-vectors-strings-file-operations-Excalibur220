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

void average_colors(const vector<Pixel> &pixel_list)
{
    if (pixel_list.size() == 0)
    {
        cout << "No pixels loaded.\n";
        return;
    }

    double sum_r = 0;
    double sum_g = 0;
    double sum_b = 0;

    for (size_t i = 0; i < pixel_list.size(); i++)
    {
        sum_r += pixel_list[i].r;
        sum_g += pixel_list[i].g;
        sum_b += pixel_list[i].b;
    }

    double n = pixel_list.size();

    cout << "average r = " << sum_r / n << endl;
    cout << "average g = " << sum_g / n << endl;
    cout << "average b = " << sum_b / n << endl;
}

void flip_vertically(vector<Pixel> &pixel_list)
{
    int max_x = 0;
    int max_y = 0;

    for (size_t i = 0; i < pixel_list.size(); i++)
    {
        if (pixel_list[i].x > max_x)
            max_x = pixel_list[i].x;

        if (pixel_list[i].y > max_y)
            max_y = pixel_list[i].y;
    }

    vector<vector<Pixel>> grid(max_y + 1, vector<Pixel>(max_x + 1));
    vector<vector<bool>> exists(max_y + 1, vector<bool>(max_x + 1, false));

    for (size_t i = 0; i < pixel_list.size(); i++)
    {
        int x = pixel_list[i].x;
        int y = pixel_list[i].y;

        grid[y][x] = pixel_list[i];
        exists[y][x] = true;
    }

    vector<Pixel> result;

    for (size_t i = 0; i < pixel_list.size(); i++)
    {
        Pixel p = pixel_list[i];

        int x = p.x;
        int y = p.y;
        int mirror_y = max_y - y;

        if (exists[mirror_y][x])
        {
            p.r = grid[mirror_y][x].r;
            p.g = grid[mirror_y][x].g;
            p.b = grid[mirror_y][x].b;
        }

        result.push_back(p);
    }

    pixel_list = result;
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
        cout << "Could not open file\n";
        return 1;
    }

    vector<Pixel> pixel_list;
    string line;

    while (getline(fin, line))
    {
        if (line.size() == 0)
            continue;

        Pixel p = parse_line(line);
        pixel_list.push_back(p);
    }

    fin.close();

    cout << "pixels loaded = " << pixel_list.size() << endl;

    average_colors(pixel_list);
    flip_vertically(pixel_list);

    ofstream fout("flipped.dat");

    if (!fout.is_open())
    {
        cout << "Could not create output file\n";
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