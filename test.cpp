#include "./tensor.h"
#include "./tensor.cpp"

#include <iostream>
int main()
{
    Tensor tensor1({1, 2, 3, 4, 5, 6}, {3, 2});
    Tensor tensor2({3, 3, 0, 2}, {2, 2});

    try
    {
        float value = 1.2f;
        Tensor resultMult = Mult(tensor1, value);
        for (size_t i = 0; i < resultMult.GetData().size(); i++)
        {
            std::cout << resultMult.GetData().at(i) << std::endl;
        }
    }
    catch (const char* message)
    {
        std::cerr << message << '\n';
        return 1;
    }
    return 0;
};