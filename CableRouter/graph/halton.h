class Halton
{
    pair<float, float> current;
    pair<float, float> f;
    int i;
public:
    Halton(pair<float, float> seed);
    pair<float, float> next();
};