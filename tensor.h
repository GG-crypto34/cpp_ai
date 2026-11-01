#include <vector>

class Tensor{
    private:
        std::vector<float> Data;
        std::vector<size_t> Shape;
        std::vector<size_t> Strides;
        
        void calculateStrides() // вызывается в сеттере формы для изменения Strides
        {
            size_t size = Shape.size();
            std::vector<size_t> resultStrides(size, 1);
        
            for(size_t i = 1; i < size; i++)
            {
                resultStrides.at(size - i - 1) = resultStrides.at(size - i) * Shape.at(size - i);
            };

            Strides = resultStrides;
        };
    public:
        Tensor(std::vector<float> data, std::vector<size_t> shape)
        {
            size_t sizeOfTensor = 1;
            for(size_t i = 0; i < shape.size(); i++) 
            {
                sizeOfTensor = sizeOfTensor * shape[i];
            };
            if(sizeOfTensor == data.size())
            {
                Data = data;
                Shape = shape;
                calculateStrides();
            }
            else
            {
                throw "Invalid arguments";
            };
        };

        // геттеры
        const std::vector<float>& GetData () const;
        const std::vector<size_t>& GetShape() const;
        const std::vector<size_t>& GetStrides() const;
        const float& GetDataAt(std::vector<size_t> index) const;

        //сеттеры
        void SetData(std::vector<float> data); // только если количество эл совпадает с shape
        void Reshape(std::vector<size_t> shape); // тоже проверить валидность преобразования или сделать автообрезание/дополнение даты
        void EditOnIndex(std::vector<size_t> index, float value);
        void Transpose(std::vector<size_t> permutations);
};

const std::vector<float>& Tensor::GetData () const
{
    return Data;    
};
const std::vector<size_t>& Tensor::GetShape() const
{
    return Shape;
};
const std::vector<size_t>& Tensor::GetStrides() const
{
    return Strides;
};

void Tensor::SetData(std::vector<float> data)
{
    size_t sizeOfTensor = 1;
    for(size_t i = 0; i < Shape.size(); i++) 
    {
        sizeOfTensor = sizeOfTensor * Shape[i];
    };
    if(data.size() == sizeOfTensor){ Data = data;}
    else{throw "incorrect shape or data of tensor"; };
};

void Tensor::Reshape(std::vector<size_t> shape)
{
    size_t sizeOfTensor = 1;
    for(size_t i = 0; i < shape.size(); i++) 
    {
        sizeOfTensor = sizeOfTensor * shape[i];
    };
    if(sizeOfTensor == Data.size())
    {
        Shape = shape;
        calculateStrides();
    }
    else
    {throw "Invalid size of tensor";};
};

const float& Tensor::GetDataAt(std::vector<size_t> index) const
{
    if(index.size() != Shape.size()){throw "Invalid index";};
    for(size_t i = 0; i < index.size(); i++)
    {
        if(index[i] >= Shape[i]){throw "Invalid index";}
    };
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides.at(i);
    };
    return Data.at(findex);
};

void Tensor::EditOnIndex(std::vector<size_t> index, float value)
{
    if(index.size() != Shape.size()){throw "Invalid index";};
    for(size_t i = 0; i < index.size(); i++)
    {
        if(index[i] >= Shape[i]){throw "Invalid index";}
    };
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides.at(i);
    };
    Data.at(findex) = value;
};

void Tensor::Transpose(std::vector<size_t> permutations)
{
    std::vector<int> seen(permutations.size(), 0);
    for(size_t i = 0; i < permutations.size(); i++)
    {
        seen.at(permutations.at(i)) += 1;
    };
    for(size_t i = 0; i < permutations.size(); i++)
    {
        if(seen.at(i) > 1){throw "Invalid argument";}
    };
    std::vector<size_t> fShape(permutations.size(), 0);
    std::vector<size_t> fStrides(permutations.size(), 0);
    for(size_t i = 0; i < permutations.size(); i++)
    {
        fShape.at(i) = Shape.at(permutations.at(i));
        fStrides.at(i) = Strides.at(permutations.at(i));
    };
    Shape = fShape;
    Strides = fStrides;
};