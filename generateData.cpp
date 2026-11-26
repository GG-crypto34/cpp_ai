#include <iostream>
#include <fstream>
int main()
{
    std::ofstream out;
    out.open("data.txt");
    out << "0 0" << std::endl;
    out.close();
    out.open("data.txt", std::ios::app);
    int a;
    std::cin >> a;
    for(size_t i = 0; i < a; i++)
    {
        float x = (float)(rand()) / (float)(RAND_MAX);
        float y = (float)(rand()) / (float)(RAND_MAX);
        out << x << " " << y << std::endl;
    };
    out.close();
    return 0;
};