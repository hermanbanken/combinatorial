#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <bitset>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[]) {

    if(argc < 3){
        cout << "Usage: \ttobinary <file> <outfile>" << endl;
        cout << "\t\tWhere <file> is a file containing coordinates x, y, z, cable depth, soil type, obstacle type" << endl;
        exit(0);
    }
    string file = argv[1];
    string outfile = argv[2];

    ifstream in(file);
    if (!in.good()){
        cout << "The file is not readable.";
        exit(0);
    }

    FILE* out;
    out = fopen(outfile.c_str(), "wb");

    float x, y, z, depth, soil, obst;
    int points;
    while ( in >> x >> y >> z >> depth >> soil >> obst )
    {
        short d = depth;
        char s = soil;
        char o = obst;
        z = obst;
        fwrite(&x, sizeof(float), 1, out);
        fwrite(&y, sizeof(float), 1, out);
        fwrite(&z, sizeof(float), 1, out);
        fwrite(&d, sizeof(short), 1, out);
        fwrite(&s, sizeof(char),  1, out);
        fwrite(&o, sizeof(char),  1, out);

        points++;
    }
    fclose(out);

    cout << "Written " << points << " to " << outfile;
    exit(0);
}
