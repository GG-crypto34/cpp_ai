#include "./tensor.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

float randomFloat()
{
    return (float)(rand()) / (float)(RAND_MAX);
}

const float e = 2.71;
void sigmoid(Tensor& z)
{
    std::vector<float> resData{};
    for(size_t i = 0; i < z.GetData().size(); i++)
    {
        resData.push_back(1 / (1 + pow(e, -z.GetData().at(i))));
    };
    z.SetData(resData);
};

int main()
{
    Tensor inp({0,0}, {2, 1});
    Tensor layer1({randomFloat(), randomFloat(), randomFloat(), randomFloat(), randomFloat(), randomFloat(), randomFloat(), randomFloat()}, {4, 2});
    Tensor layer2({randomFloat(), randomFloat(), randomFloat(), randomFloat()}, {1, 4});
    Tensor result({0}, {1, 1});
    float learningRate = 0.25f;
    int epochs = 100;

    std::ifstream f("./data.txt");
    std::ifstream f1("./dataRes.txt");

    std::vector<std::vector<float>> resData;
    std::cout << "init completed" << std::endl;
    for(int i = 0; i < epochs; i++)
    {
        std::string line;
        while (std::getline(f, line))
        {
            std::istringstream row(line);
            float x, y;
            row >> x >> y;
            std::cout << x << y << std::endl;
            std::vector<float> vec = {x, y};
            std::cout << 1 << std::endl;
            inp.SetData(vec);
            std::cout << 2 << std::endl;
            Tensor z1({0},{1});
            Tensor z2({0},{1});
            std::cout << 3 << std::endl;
            z1 = MatrixMult(layer1, inp);
            std::cout << 4 << std::endl;
            sigmoid(z1);
            std::cout << z1.GetShape().at(0) << " " << z1.GetShape().at(1) << std::endl;
            std::cout << layer2.GetShape().at(0) << " " << layer2.GetShape().at(1) << std::endl;
            z2 = MatrixMult(layer2, z1);
            std::cout << 6 << std::endl;
            sigmoid(z2); 
            std::cout << 6 << std::endl;
            vec.push_back(z2.GetData().at(0));
            std::cout << 7 << std::endl;
            resData.push_back(vec);
            std::cout << 8 << std::endl;
            std::cout << "Input: " << x << ", " << y << " Output: " << z2.GetData().at(0) << std::endl;
        };
    }
    for(size_t i = 0; i < resData.size(); i++)
    {
        std::ofstream out;
        out.open("dataRes.txt", std::ios::app);
        out << resData.at(i).at(0) << " " << resData.at(i).at(1) << " " << resData.at(i).at(2) << std::endl;
        out.close();
    };
};