#include "./tensor.h"

#include <iostream>
int main()
{
    Tensor tensor1({1, 2, 3, 4, 5, 6}, {3, 2});
    Tensor tensor2({3, 3, 0, 2, 4, 5}, {3, 2});

    try
    {
        float value = 1.2f;
        Tensor resultDiv = DivH(tensor1, tensor2);
        for (size_t i = 0; i < resultDiv.GetData().size(); i++)
        {
            std::cout << resultDiv.GetData().at(i) << std::endl;
        }
    }
    catch (const char* message)
    {
        std::cerr << message << '\n';
        return 1;
    }
    return 0;
};