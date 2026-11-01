#include <vector>

class Tensor{
    private:
        std::vector<float> Data_;
        std::vector<size_t> Shape_;
        std::vector<size_t> Strides_;
        bool IsTransposed_ = false;
        void calculateStrides() // вызывается в сеттере формы для изменения Strides_
        {
            size_t size = Shape_.size();
            std::vector<size_t> resultStrides(size, 1);
        
            for(size_t i = 1; i < size; i++)
            {
                resultStrides.at(size - i - 1) = resultStrides.at(size - i) * Shape_.at(size - i);
            };

            Strides_ = resultStrides;
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
                Data_ = data;
                Shape_ = shape;
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
        
        //опасные методы без проверок
        void DangerSetData(std::vector<float> data);
        void DangerReshape(std::vector<size_t> shape); 
        void DangerEditOnIndex(std::vector<size_t> index, float value);
        void DangerTranspose(std::vector<size_t> permutations);
        const float& DangerGetDataAt(std::vector<size_t> index) const;
};

const std::vector<float>& Tensor::GetData () const
{
    return Data_;    
};
const std::vector<size_t>& Tensor::GetShape() const
{
    return Shape_;
};
const std::vector<size_t>& Tensor::GetStrides() const
{
    return Strides_;
};

void Tensor::SetData(std::vector<float> data)
{
    size_t sizeOfTensor = 1;
    for(size_t i = 0; i < Shape_.size(); i++) 
    {
        sizeOfTensor = sizeOfTensor * Shape_[i];
    };
    if(data.size() == sizeOfTensor){ Data_ = data;}
    else{throw "incorrect shape or data of tensor"; };
};

void Tensor::Reshape(std::vector<size_t> shape)
{
    size_t sizeOfTensor = 1;
    for(size_t i = 0; i < shape.size(); i++) 
    {
        sizeOfTensor = sizeOfTensor * shape[i];
    };
    if(sizeOfTensor == Data_.size() && !IsTransposed_)
    {
        Shape_ = shape;
        calculateStrides();
    }
    else
    {throw "Invalid size of tensor or tensor is transposed";};
};

const float& Tensor::GetDataAt(std::vector<size_t> index) const
{
    if(index.size() != Shape_.size()){throw "Invalid index";};
    for(size_t i = 0; i < index.size(); i++)
    {
        if(index[i] >= Shape_[i]){throw "Invalid index";}
    };
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides_.at(i);
    };
    return Data_.at(findex);
};

void Tensor::EditOnIndex(std::vector<size_t> index, float value)
{
    if(index.size() != Shape_.size()){throw "Invalid index";};
    for(size_t i = 0; i < index.size(); i++)
    {
        if(index[i] >= Shape_[i]){throw "Invalid index";}
    };
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides_.at(i);
    };
    Data_.at(findex) = value;
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
        fShape.at(i) = Shape_.at(permutations.at(i));
        fStrides.at(i) = Strides_.at(permutations.at(i));
    };
    Shape_ = fShape;
    Strides_ = fStrides;
    IsTransposed_ = true;
};

void Tensor::DangerSetData(std::vector<float> data)
{
    Data_ = data;
};

void Tensor::DangerReshape(std::vector<size_t> shape)
{
    Shape_ = shape;
    calculateStrides();
};

const float& Tensor::DangerGetDataAt(std::vector<size_t> index) const
{
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides_.at(i);
    };
    return Data_.at(findex);
};

void Tensor::DangerEditOnIndex(std::vector<size_t> index, float value)
{
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides_.at(i);
    };
    Data_.at(findex) = value;
};

void Tensor::DangerTranspose(std::vector<size_t> permutations)
{
    std::vector<size_t> fShape(permutations.size(), 0);
    std::vector<size_t> fStrides(permutations.size(), 0);
    for(size_t i = 0; i < permutations.size(); i++)
    {
        fShape.at(i) = Shape_.at(permutations.at(i));
        fStrides.at(i) = Strides_.at(permutations.at(i));
    };
    Shape_ = fShape;
    Strides_ = fStrides;
    IsTransposed_ = true;
};